#include "server.h"
/**
 * @brief Lấy danh sách các tiến trình đang chạy trên hệ thống.
 * @return Một chuỗi (string) chứa danh sách các tiến trình.
 */
std::string GetProcessList() {
    std::stringstream ss;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        ss << "ERROR: Khong the tao snapshot.";
        return ss.str();
    }

    if (!Process32First(hProcessSnap, &pe32)) {
        ss << "ERROR: Khong the lay tien trinh dau tien.";
        CloseHandle(hProcessSnap);
        return ss.str();
    }

    // Gửi dữ liệu dưới dạng CSV (dễ xử lý hơn)
    // PID,Tên,Username (Username cần API phức tạp hơn, tạm bỏ qua)
    ss << "PID,MEM_MB,NAME\n"; // Header

    // Lặp qua tất cả các tiến trình
    do {
        // Lấy thông tin bộ nhớ (Phần này phức tạp, tạm thời bỏ qua)
        ss << pe32.th32ProcessID << "," << 0 << "," << pe32.szExeFile << "\n";
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return ss.str();
}

/**
 * @brief Hàm chính của server, xử lý từng kết nối client
 */
void HandleClient(SOCKET ClientSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;

    // Vòng lặp nhận dữ liệu
    do {
        iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            std::string command = std::string(recvbuf);
            std::cout << "Nhan lenh: " << command << std::endl;

            std::string response = "";

            // --- XỬ LÝ LỆNH ---
            if (command == "LIST_PROCESS") {
                response = GetProcessList();
            }
            else if (command == "SHUTDOWN") {
                response = "OK: Dang thuc hien shutdown...";
                system("shutdown /s /f /t 0");
            }
            else if (command == "RESTART") {
                response = "OK: Dang thuc hien restart...";
                system("shutdown /r /f /t 0");
            }
            // (Bạn sẽ cần thêm lệnh SCREENSHOT ở đây)
            else {
                response = "ERROR: Lenh khong hop le.";
            }

            // Gửi phản hồi lại cho client (Python bridge)
            send(ClientSocket, response.c_str(), (int)response.length(), 0);

        } else if (iResult == 0) {
            std::cout << "Client (Bridge) da ngat ket noi." << std::endl;
        } else {
            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        }
    } while (iResult > 0);

    closesocket(ClientSocket);
}