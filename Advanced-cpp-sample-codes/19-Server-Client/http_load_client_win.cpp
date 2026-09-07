// Compile (MSVC):

//   cl /EHsc http_load_client_win.cpp ws2_32.lib

// MinGW:

//   g++ http_load_client_win.cpp -lws2_32 -o http_load_client_win.exe

//

// Usage:

//   http_load_client_win.exe <host> <port> <path> <num_threads> <requests_per_thread>

//

// Example:

//   http_load_client_win.exe 127.0.0.1 8080 / 10 5

//   -> 10 threads, 5 requests per thread to http://127.0.0.1:8080/
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#include <ws2tcpip.h>
 
#include <iostream>

#include <thread>

#include <vector>

#include <mutex>

#include <chrono>

#include <sstream>

#include <atomic>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;

std::atomic<int> succ{0}, failc{0};
 
void safe_print(const std::string &s) {

    std::lock_guard<std::mutex> lk(cout_mtx);

    std::cout << s << std::flush;

}
 
std::string make_request(const std::string &host, const std::string &path) {

    std::ostringstream oss;

    oss << "GET " << path << " HTTP/1.0\r\n"
<< "Host: " << host << "\r\n"
<< "Connection: close\r\n"
<< "User-Agent: http-load-client/1.0\r\n"
<< "\r\n";

    return oss.str();

}
 
void worker(const std::string host, const std::string port, const std::string path, int id, int reqsPerThread) {

    for (int r = 1; r <= reqsPerThread; ++r) {

        addrinfo hints{}; hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;

        addrinfo *res = nullptr;

        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0 || !res) {

            safe_print("[T" + std::to_string(id) + "] getaddrinfo failed\n");

            failc++;

            if (res) freeaddrinfo(res);

            continue;

        }
 
        SOCKET sock = INVALID_SOCKET;

        for (addrinfo* p = res; p != nullptr; p = p->ai_next) {

            sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

            if (sock == INVALID_SOCKET) continue;

            if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == 0) break;

            closesocket(sock); sock = INVALID_SOCKET;

        }

        freeaddrinfo(res);
 
        if (sock == INVALID_SOCKET) {

            safe_print("[T" + std::to_string(id) + "] connect failed\n");

            failc++;

            continue;

        }
 
        std::string req = make_request(host, path);

        auto t0 = std::chrono::high_resolution_clock::now();

        int sent = send(sock, req.c_str(), (int)req.size(), 0);

        if (sent == SOCKET_ERROR) {

            safe_print("[T" + std::to_string(id) + "] send error: " + std::to_string(WSAGetLastError()) + "\n");

            closesocket(sock);

            failc++;

            continue;

        }
 
        // read until connection closed

        std::string resp;

        char buf[1024];

        while (true) {

            int n = recv(sock, buf, sizeof(buf), 0);

            if (n > 0) {

                resp.append(buf, buf + n);

            } else if (n == 0) {

                break; // server closed

            } else {

                safe_print("[T" + std::to_string(id) + "] recv error: " + std::to_string(WSAGetLastError()) + "\n");

                break;

            }

        }
 
        auto t1 = std::chrono::high_resolution_clock::now();

        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
 
        // simple validation: look for HTTP status line

        if (resp.find("HTTP/1.0") != std::string::npos || resp.find("HTTP/1.1") != std::string::npos) {

            succ++;

            std::ostringstream out;

            out << "[T" << id << " R" << r << "] OK " << (int)ms << " ms, bytes=" << resp.size() << "\n";

            safe_print(out.str());

        } else {

            failc++;

            safe_print("[T" + std::to_string(id) + " R" + std::to_string(r) + "] invalid response\n");

        }
 
        closesocket(sock);

        // small pause to avoid perfect sync

        std::this_thread::sleep_for(std::chrono::milliseconds(30 + (id*7)%100));

    }

}
 
int main(int argc, char* argv[]) {

    if (argc < 6) {

        std::cout << "Usage: " << argv[0] << " <host> <port> <path> <num_threads> <requests_per_thread>\n"
<< "Example: " << argv[0] << " 127.0.0.1 8080 / 10 5\n";

        return 1;

    }
 
    std::string host = argv[1];

    std::string port = argv[2];

    std::string path = argv[3];

    int threads = atoi(argv[4]);

    int reqs = atoi(argv[5]);

    if (threads <= 0) threads = 1;

    if (reqs <= 0) reqs = 1;
 
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {

        std::cerr << "WSAStartup failed\n";

        return 1;

    }
 
    std::cout << "HTTP load client -> host=" << host << " port=" << port << " path=" << path
<< " threads=" << threads << " reqs/thread=" << reqs << "\n";
 
    std::vector<std::thread> pool;

    for (int i = 1; i <= threads; ++i) pool.emplace_back(worker, host, port, path, i, reqs);
 
    for (auto &t : pool) if (t.joinable()) t.join();
 
    std::cout << "\nDone. success=" << succ.load() << " failures=" << failc.load() << "\n";
 
    WSACleanup();

    return 0;

}
 
// ==========================================================================================

// simple_http_server_win.exe 8080
 
// http_load_client_win.exe 127.0.0.1 8080 / 10 5
 
 