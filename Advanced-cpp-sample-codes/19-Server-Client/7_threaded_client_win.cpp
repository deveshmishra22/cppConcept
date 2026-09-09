// threaded_client_win.cpp
// Compile (MSVC): cl /EHsc threaded_client_win.cpp ws2_32.lib
// MinGW: g++ threaded_client_win.cpp -lws2_32 -o threaded_client_win.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
 
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <chrono>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;
void safe_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::flush;
}
 
void client_task(const std::string server, const std::string port, int id, int messages) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* res = nullptr;
    if (getaddrinfo(server.c_str(), port.c_str(), &hints, &res) != 0) {
        safe_print("[C" + std::to_string(id) + "] getaddrinfo failed\n");
        return;
    }
 
    SOCKET sock = INVALID_SOCKET;
    for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) continue;
        if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == 0) break;
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    freeaddrinfo(res);
 
    if (sock == INVALID_SOCKET) {
        safe_print("[C" + std::to_string(id) + "] connect failed\n");
        return;
    }
    safe_print("[C" + std::to_string(id) + "] Connected to " + server + ":" + port + "\n");
 
    for (int i = 1; i <= messages; ++i) {
        std::string msg = "Client " + std::to_string(id) + " msg " + std::to_string(i) + "\n";
        int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            safe_print("[C" + std::to_string(id) + "] send error: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }
 
        char buf[1024];
        int recvd = recv(sock, buf, sizeof(buf)-1, 0);
        if (recvd > 0) {
            buf[recvd] = '\0';
            safe_print("[C" + std::to_string(id) + "] Received: " + std::string(buf));
        } else if (recvd == 0) {
            safe_print("[C" + std::to_string(id) + "] Server closed connection\n");
            break;
        } else {
            safe_print("[C" + std::to_string(id) + "] recv error: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }
 
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
 
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    safe_print("[C" + std::to_string(id) + "] Finished\n");
}
 
int main(int argc, char* argv[]) {
    const char* server = (argc >= 2) ? argv[1] : "127.0.0.2";
    const char* port   = (argc >= 3) ? argv[2] : "90001";
    int threads = (argc >= 4) ? std::stoi(argv[3]) : 3;
    int messages = (argc >= 5) ? std::stoi(argv[4]) : 5;
 
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
 
    std::cout << "Launching " << threads << " client threads to " << server << ":" << port
<< " (" << messages << " messages each)\n";
 
    std::vector<std::thread> pool;
    for (int i = 1; i <= threads; ++i) {
        pool.emplace_back(client_task, server, port, i, messages);
    }
 
    for (auto &t : pool) if (t.joinable()) t.join();
 
    WSACleanup();
    std::cout << "All client threads finished.\n";
    return 0;
}
 
//==================================================================================================
//threaded_client_win.exe 127.0.0.1 9000 5 10