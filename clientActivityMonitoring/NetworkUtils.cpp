#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#include "NetworkUtils.h"

std::wstring NetworkUtils::GetIPAddress() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return L"";
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        WSACleanup();
        return L"";
    }

    addrinfo hints = { 0 };
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result = NULL;
    if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
        WSACleanup();
        return L"";
    }

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &((sockaddr_in*)result->ai_addr)->sin_addr, ipStr, sizeof(ipStr));

    std::wstring ipAddress(ipStr, ipStr + strlen(ipStr));

    freeaddrinfo(result);
    WSACleanup();

    return ipAddress;
}
