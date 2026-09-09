// threaded_server_win.cpp

// Compile (MSVC): cl /EHsc threaded_server_win.cpp ws2_32.lib

// MinGW: g++ threaded_server_win.cpp -lws2_32 -o threaded_server_win.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

std::mutex cout_mtx;
std::atomic<bool> running{ true };
void ts_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::endl;
}
 
void handle_client(SOCKET clientSock, sockaddr_in clientAddr) {
    char ip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
    int port = ntohs(clientAddr.sin_port);
    ts_print("[Worker] Connected: " + std::string(ip) + ":" + std::to_string(port)); 
    const int BUFSZ = 1024;
    char buf[BUFSZ];
 
    while (running) {
        int bytes = recv(clientSock, buf, BUFSZ, 0);
        if (bytes > 0) {
            // echo back
            int sent = 0;

            while (sent < bytes) {
                int s = send(clientSock, buf + sent, bytes - sent, 0);
                if (s == SOCKET_ERROR) {
                    ts_print("[Worker] send error: " + std::to_string(WSAGetLastError()));
                    break;

                }
                sent += s;
            }

            // print received message (safe)
            std::string msg(buf, buf + bytes);
            ts_print("[Worker] From " + std::string(ip) + ":" + std::to_string(port) + " => " + msg);

        } else if (bytes == 0) {
            ts_print("[Worker] Client disconnected: " + std::string(ip) + ":" + std::to_string(port));
            break;

        } else {
            ts_print("[Worker] recv error: " + std::to_string(WSAGetLastError()));
            break;
        }
    }
    shutdown(clientSock, SD_BOTH);
    closesocket(clientSock);
    ts_print("[Worker] Closed: " + std::string(ip) + ":" + std::to_string(port));
}
 
int main(int argc, char* argv[]) {
    const char* port = (argc >= 2) ? argv[1] : "9001";
    ts_print("Simple threaded echo server (Windows) - port " + std::string(port));
    ts_print("Press ENTER to stop the server.");
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        ts_print("WSAStartup failed");
        return 1;
    }
 
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addrinfo* result = nullptr;
    if (getaddrinfo(nullptr, port, &hints, &result) != 0) {
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
 
    // Thread that waits for ENTER to stop the server

    std::thread stopper([](){
        std::string line;
        std::getline(std::cin, line);
        running = false;
    });
 
    std::vector<std::thread> workers;
    workers.reserve(32);
 
    while (running) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);

        if (clientSock == INVALID_SOCKET) {
            int err = WSAGetLastError();
            if (!running) break; // normal shutdown (listen socket closed)
            ts_print("accept() failed: " + std::to_string(err));
            continue;
        }
 
        // spawn worker and detach (for simplicity). Alternatively, push and join on shutdown.
        workers.emplace_back(handle_client, clientSock, clientAddr);
        workers.back().detach();
    }
 
    // cleanup
    closesocket(listenSock);
    if (stopper.joinable()) stopper.join();
    ts_print("Server shutdown. Cleaning up Winsock.");
    WSACleanup();
    return 0;
}

 