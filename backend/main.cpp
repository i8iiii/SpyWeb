#include "server.h"
int main() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    listen(ListenSocket, SOMAXCONN);

    std::cout << "Server C++ (Agent) dang lang nghe tren cong " << DEFAULT_PORT << "..." << std::endl;

    // Vòng lặp chính: Chấp nhận client mới
    while (true) {
        SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            continue; // Thử lại
        }
        std::cout << "Client (Bridge) da ket noi!" << std::endl;
        // Xử lý client này (trong 1 luồng riêng sẽ tốt hơn, nhưng tạm làm đơn giản)
        HandleClient(ClientSocket);
    }

    // Dọn dẹp (code sẽ không bao giờ chạy đến đây trong vòng lặp vô tận)
    closesocket(ListenSocket);
    WSACleanup();
    return 0;
}