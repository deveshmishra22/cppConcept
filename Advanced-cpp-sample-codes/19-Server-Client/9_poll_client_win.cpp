// Compile (MSVC):
//   cl /EHsc poll_client_win.cpp ws2_32.lib
// MinGW:
//   g++ poll_client_win.cpp -lws2_32 -o poll_client_win.exe
//
// Usage:
//   poll_client_win.exe <server_ip> <port> <num_threads> <messages_per_thread>
//
// Example:
//   poll_client_win.exe 127.0.0.1 9000 4 5
//   -> spawns 4 threads, each sends 5 messages
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;
void safe_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::flush;
}
 
void client_worker(const std::string host, const std::string port, int id, int messages) {

    // Resolve
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* res = nullptr;
    int rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (rc != 0 || !res) {
        safe_print("[C" + std::to_string(id) + "] getaddrinfo failed\n");
        if (res) freeaddrinfo(res);
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
 
    safe_print("[C" + std::to_string(id) + "] Connected\n");
    for (int i = 1; i <= messages; ++i) {
        std::string msg = "Client " + std::to_string(id) + " message " + std::to_string(i) + "\n";
        int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            safe_print("[C" + std::to_string(id) + "] send failed: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }
 
        // Use WSAPoll to wait for server response (timeout 2s)
        WSAPOLLFD pfd{};
        pfd.fd = sock;
        pfd.events = POLLIN;
        int r = WSAPoll(&pfd, 1, 2000); // 2000 ms
        if (r > 0 && (pfd.revents & POLLIN)) {
            char buf[1024];
            int n = recv(sock, buf, sizeof(buf)-1, 0);
            if (n > 0) {
                buf[n] = '\0';
                safe_print("[C" + std::to_string(id) + "] got reply: " + std::string(buf));
            } else if (n == 0) {
                safe_print("[C" + std::to_string(id) + "] server closed connection\n");
                break;
            } else {
                safe_print("[C" + std::to_string(id) + "] recv error\n");
                break;
            }
        } else {
            safe_print("[C" + std::to_string(id) + "] no reply (timeout or error)\n");
        }
 
        // small pause
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
 
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    safe_print("[C" + std::to_string(id) + "] finished\n");
}
 
int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " <server_ip> <port> <num_threads> <messages_per_thread>\n";
        return 1;
    }
 
    std::string host = argv[1];
    std::string port = argv[2];
    int threads = atoi(argv[3]);
    int msgs = atoi(argv[4]);
    if (threads <= 0) threads = 1;
    if (msgs <= 0) msgs = 1;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    } 
    std::vector<std::thread> workers;
    for (int i = 1; i <= threads; ++i) {
        workers.emplace_back(client_worker, host, port, i, msgs);
    }
 
    for (auto &t : workers) if (t.joinable()) t.join();
    WSACleanup();
    return 0;

}
 
// =============================================================================================

// poll_echo_server_win.exe 9000
 
// poll_client_win.exe 127.0.0.1 9000 4 5
 
// 4 threads, 5 messages each.

// ---------------------------------------------------------------------------------------------

 