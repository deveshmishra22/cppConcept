// file: simple_echo_server_win.cpp

// Compile (MSVC): cl /EHsc simple_echo_server_win.cpp ws2_32.lib

// MinGW: g++ -std=c++11 simple_echo_server_win.cpp -lws2_32 -o simple_echo_server.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <mutex>
#pragma comment(lib, "Ws2_32.lib")
std::mutex cout_mtx;

void ts_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::endl;
}
 
// Handles a single client: echoes back received data

void client_handler(SOCKET clientSock, sockaddr_in clientAddr) {
    char *clientIp = inet_ntoa(clientAddr.sin_addr);
    int clientPort = ntohs(clientAddr.sin_port);
    ts_print("[Worker] Connected: " + std::string(clientIp) + ":" + std::to_string(clientPort));
    const int BUFSIZE = 1024;
    char buf[BUFSIZE];
 
    while (true) {
        int received = recv(clientSock, buf, BUFSIZE, 0);

        if (received > 0) {
            // Echo back
            int sent_total = 0;
            while (sent_total < received) {
                int s = send(clientSock, buf + sent_total, received - sent_total, 0);
                if (s == SOCKET_ERROR) {
                    ts_print("[Worker] send() error: " + std::to_string(WSAGetLastError()));
                    break;
                }
                sent_total += s;
            }

            // Print what we got (safe substring)
            std::string msg(buf, buf + received);
            ts_print("[Worker] From " + std::string(clientIp) + ":" + std::to_string(clientPort) + " -> " + msg);
        } else if (received == 0) {

            ts_print("[Worker] Client disconnected: " + std::string(clientIp) + ":" + std::to_string(clientPort));
            break;
        } else {
            ts_print("[Worker] recv() failed: " + std::to_string(WSAGetLastError()));
            break;
        }
    }
 
    shutdown(clientSock, SD_BOTH);
    closesocket(clientSock);
    ts_print("[Worker] Closed: " + std::string(clientIp) + ":" + std::to_string(clientPort));

}
 
int main(int argc, char* argv[]) {
    const char* portStr = (argc >= 2) ? argv[1] : "9000";
    ts_print("Simple multithreaded echo server (Windows) - listening on port " + std::string(portStr));
 
    // Init Winsock
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        ts_print("WSAStartup failed");
        return 1;
    }
 
    // Create socket
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addrinfo* result = nullptr;

    if (getaddrinfo(NULL, portStr, &hints, &result) != 0) {
        ts_print("getaddrinfo failed");
        WSACleanup();
        return 1;
    }

    SOCKET listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSock == INVALID_SOCKET) {
        ts_print("socket() failed: " + std::to_string(WSAGetLastError()));
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
 
    // reuse addr
    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
 
    if (bind(listenSock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        ts_print("bind() failed: " + std::to_string(WSAGetLastError()));
        closesocket(listenSock);
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        ts_print("listen() failed: " + std::to_string(WSAGetLastError()));
        closesocket(listenSock);
        WSACleanup();
        return 1;
    } 
    ts_print("Server ready. Connect clients (e.g. run the provided client). Ctrl+C to stop.");

    // Accept loop: spawn a thread per client (detached)

    while (true) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);

        if (clientSock == INVALID_SOCKET) {
            ts_print("accept() failed: " + std::to_string(WSAGetLastError()));
            break;
        }
 
        // Spawn worker thread and detach it (simple)
        std::thread(worker_thread, client_handler, clientSock, clientAddr).detach();

        // Note: lambda alternative if compiler complains about mismatch:
        // std::thread([clientSock, clientAddr]{ client_handler(clientSock, clientAddr); }).detach();
    }
 
    closesocket(listenSock);
    WSACleanup();
    return 0;

}

 
