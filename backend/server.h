#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstdlib>
#include <tlhelp32.h> // Cho GetProcessList
#include <sstream>    // Cho GetProcessList

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "9999"
#define DEFAULT_BUFLEN 4096

std::string GetProcessList();
void HandleClient(SOCKET ClientSocket);

#endif // SERVER_H