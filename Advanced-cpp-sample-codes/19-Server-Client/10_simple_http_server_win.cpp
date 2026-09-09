// Compile (MSVC):

//   cl /EHsc simple_http_server_win.cpp ws2_32.lib

// MinGW:

//   g++ simple_http_server_win.cpp -lws2_32 -o simple_http_server_win.exe

//

// Usage:

//   simple_http_server_win.exe [port]

// Example:

//   simple_http_server_win.exe 8080
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#include <ws2tcpip.h>
 
#include <iostream>

#include <string>

#include <vector>

#include <mutex>

#include <atomic>
#include <thread>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;

void safe_print(const std::string &s) {

    std::lock_guard<std::mutex> lk(cout_mtx);

    std::cout << s << std::endl;

}
 
std::atomic<bool> running(true);
 
std::string build_response() {

    const std::string body = "<html><body><h1>Simple Win HTTP Server</h1><p>Hello from server.</p></body></html>\n";

    std::string resp = "HTTP/1.0 200 OK\r\n";

    resp += "Content-Type: text/html; charset=utf-8\r\n";

    resp += "Content-Length: " + std::to_string(body.size()) + "\r\n";

    resp += "Connection: close\r\n";

    resp += "\r\n";

    resp += body;

    return resp;

}
 
int main(int argc, char* argv[]) {

    const char* port = (argc >= 2) ? argv[1] : "8080";
 
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {

        std::cerr << "WSAStartup failed\n";

        return 1;

    }
 
    addrinfo hints{};

    hints.ai_family = AF_INET;

    hints.ai_socktype = SOCK_STREAM;

    hints.ai_flags = AI_PASSIVE;

    addrinfo* result = nullptr;

    if (getaddrinfo(nullptr, port, &hints, &result) != 0) {

        std::cerr << "getaddrinfo failed\n";

        WSACleanup();

        return 1;

    }
 
    SOCKET listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSock == INVALID_SOCKET) {

        std::cerr << "socket() failed: " << WSAGetLastError() << "\n";

        freeaddrinfo(result);

        WSACleanup();

        return 1;

    }
 
    int opt = 1;

    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
 
    if (bind(listenSock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {

        std::cerr << "bind() failed: " << WSAGetLastError() << "\n";

        closesocket(listenSock);

        freeaddrinfo(result);

        WSACleanup();

        return 1;

    }

    freeaddrinfo(result);
 
    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {

        std::cerr << "listen() failed: " << WSAGetLastError() << "\n";

        closesocket(listenSock);

        WSACleanup();

        return 1;

    }
 
    safe_print("Simple HTTP server listening on port " + std::string(port));

    safe_print("Press ENTER to stop.");
 
    // Pre-build response

    const std::string response = build_response();
 
    // Thread to stop the server with ENTER

    std::thread stopper([](){

        std::string s; std::getline(std::cin, s);

        running = false;

        // nothing else here; main loop will check running and exit

    });
 
    while (running) {

        sockaddr_in clientAddr{};

        int addrLen = sizeof(clientAddr);

        // Use accept with a short timeout alternative is complex; simple blocking accept is OK here

        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);

        if (clientSock == INVALID_SOCKET) {

            int err = WSAGetLastError();

            if (!running) break;

            safe_print("accept() failed: " + std::to_string(err));

            continue;

        }
 
        // Read request (simple): read up to CRLF CRLF or until buffer full

        std::string request;

        char buf[1024];

        int total = 0;

        // set recv timeout so slow clients won't hang server forever

        int timeout_ms = 2000;

        setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(timeout_ms));
 
        while (true) {

            int n = recv(clientSock, buf, sizeof(buf), 0);

            if (n > 0) {

                request.append(buf, buf + n);

                total += n;

                if (request.find("\r\n\r\n") != std::string::npos) break; // simple end of headers

                if (total > 16*1024) break;

            } else {

                break;

            }

        }
 
        // Optionally print a short summary

        safe_print("[Server] Got request (" + std::to_string(total) + " bytes), replying...");
 
        // Send response (single send; OK for small response)

        int sent = 0;

        const char* data = response.c_str();

        int len = (int)response.size();

        while (sent < len) {

            int s = send(clientSock, data + sent, len - sent, 0);

            if (s == SOCKET_ERROR) {

                safe_print("[Server] send error: " + std::to_string(WSAGetLastError()));

                break;

            }

            sent += s;

        }
 
        shutdown(clientSock, SD_BOTH);

        closesocket(clientSock);

    }
 
    // cleanup

    closesocket(listenSock);

    if (stopper.joinable()) stopper.join();

    WSACleanup();

    safe_print("Server stopped.");

    return 0;

}

 