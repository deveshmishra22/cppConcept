// // Compile (MSVC): cl /EHsc mutex_socket_example.cpp ws2_32.lib
// // or (MinGW): g++ -std=c++11 mutex_socket_example.cpp -lws2_32 -o mutex_socket_example.exe
// Both threads (t1 and t2) share the same socket.
// Each thread tries to send messages.
// The std::lock_guard<std::mutex> ensures:
// Only one thread enters the send() section at a time.
// Others wait until the mutex is released.
// This prevents mixed-up data on the socket.
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex send_mutex; // 🔒 protects socket send()
void send_messages(SOCKET sock, int thread_id) {
    for (int i = 1; i <= 5; ++i) {
        std::string msg = "Thread " + std::to_string(thread_id) +
                          " says hello " + std::to_string(i) + "\n";
        {
            std::lock_guard<std::mutex> lock(send_mutex); // only one thread sends at a time
            send(sock, msg.c_str(), (int)msg.size(), 0);
            std::cout << "[Thread " << thread_id << "] Sent: " << msg;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}
 
int main() {

    // Initialize Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
 
    // Connect to local echo server
    const char* server = "127.0.0.1";
    const char* port = "9000";
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* result = nullptr;
    getaddrinfo(server, port, &hints, &result);
    SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    connect(sock, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result); 
    std::cout << "Connected to " << server << ":" << port << "\n";

    // Start two threads sharing one socket
    std::thread t1(send_messages, sock, 1);
    std::thread t2(send_messages, sock, 2);
 
    t1.join();
    t2.join();
 
    std::cout << "Both threads finished sending.\n";
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    WSACleanup();
    return 0;

}
 
 