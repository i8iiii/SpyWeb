import os
import socket
from flask import Flask, render_template, jsonify, send_from_directory, request
from flask_cors import CORS

# --- Cấu hình ---
# Chỉ cho Flask biết thư mục 'public' ở đâu
app = Flask(__name__,
            template_folder='public',  # Thư mục chứa index.html
            static_folder='public'     # Thư mục chứa css, js
            )
CORS(app)

# Địa chỉ của Agent C++ (từ backend/main.cpp)
AGENT_HOST = '127.0.0.1'  # Chạy chung máy (localhost)
AGENT_PORT = 9999         # Phải khớp với cổng của main.cpp (DEFAULT_PORT)

def send_command_to_agent(command):
    """
    Hàm cốt lõi: Kết nối đến Agent C++, gửi lệnh, và nhận kết quả.
    """
    try:
        # Tạo một socket mới cho mỗi lệnh
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(5.0) # 5 giây timeout
            s.connect((AGENT_HOST, AGENT_PORT))
            
            # 1. Gửi lệnh
            s.sendall(command.encode('utf-8'))
            s.shutdown(socket.SHUT_WR) # Báo cho server C++ là đã gửi xong
            
            # 2. Nhận phản hồi
            response_data = b""
            while True:
                chunk = s.recv(4096)
                if not chunk:
                    break
                response_data += chunk
            
            return (True, response_data.decode('utf-8')) # Trả về (Thành công, Dữ liệu)

    except ConnectionRefusedError:
        return (False, "ERROR: Khong the ket noi den C++ Agent. Agent (main.exe) da chay chua?")
    except Exception as e:
        return (False, f"ERROR: {str(e)}")

# --- CÁC ROUTE ĐỂ PHỤC VỤ GIAO DIỆN (UI) ---

@app.route('/')
def index():
    """Phục vụ file index.html chính."""
    # Flask sẽ tự động tìm 'index.html' trong thư mục 'public'
    return render_template('index.html')

# Flask cũng sẽ tự động phục vụ các file trong 'static_folder' (public)
# ví dụ: /css/style.css và /js/matching.js

# --- CÁC ROUTE API (Giao diện sẽ gọi) ---

@app.route('/api/list_processes')
def list_processes():
    """API: Lấy danh sách tiến trình."""
    success, data = send_command_to_agent("LIST_PROCESS")
    if not success:
        return jsonify(success=False, error=data), 500
    
    # Xử lý dữ liệu CSV từ C++
    processes = []
    lines = data.strip().split('\n')
    
    # Bỏ qua header
    if lines and "PID,MEM_MB,NAME" in lines[0]:
        lines.pop(0) 
    
    for line in lines:
        try:
            parts = line.split(',')
            if len(parts) >= 3:
                processes.append({
                    'pid': parts[0],
                    'memory_mb': parts[1],
                    'name': parts[2]
                })
        except:
            continue # Bỏ qua nếu dòng bị lỗi
            
    return jsonify(success=True, processes=processes)

@app.route('/api/shutdown')
def api_shutdown():
    """API: Tắt máy."""
    success, data = send_command_to_agent("SHUTDOWN")
    if not success:
        return jsonify(success=False, error=data), 500
    return jsonify(success=True, message=data)

@app.route('/api/restart')
def api_restart():
    """API: Khởi động lại."""
    success, data = send_command_to_agent("RESTART")
    if not success:
        return jsonify(success=False, error=data), 500
    return jsonify(success=True, message=data)

# (API cho Screenshot/Quản lý App... sẽ cần thêm ở đây)

# --- Chạy Server ---
if __name__ == '__main__':
    print("-----------------------------------------------------------------")
    print("Web Server (Bridge) dang chay tai http://127.0.0.1:5000")
    print("Dam bao C++ Agent (main.exe) cung dang chay!")
    print("-----------------------------------------------------------------")
    # Chạy Web Server trên cổng 5000
    app.run(host='127.0.0.1', port=5000, debug=False)