// Compile (MSVC):

//   cl /EHsc poll_echo_server_win.cpp ws2_32.lib
// MinGW:
//   g++ -std=c++11 poll_echo_server_win.cpp -lws2_32 -o poll_echo_server_win.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;

void safe_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::flush;
}
 
std::atomic<bool> running(true); // what it means: atomic boolean flag to control the server running state across multiple threads. Analogy: like a shared on/off switch that multiple threads can check and update safely without causing conflicts.
int main(int argc, char* argv[]) {

    const char* port = (argc >= 2) ? argv[1] : "9000";
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
 
    // Create listening socket
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
 
    // reuse address
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
 
    safe_print("poll-echo-server listening on port " + std::string(port) + "\n");
    safe_print("Press ENTER to stop the server.\n");
 
    // Poll fds container
    std::vector<WSAPOLLFD> pollfds;
    WSAPOLLFD listenfd{};
    listenfd.fd = listenSock;
    listenfd.events = POLLIN; // wait for incoming connections
    pollfds.push_back(listenfd);
 
    // stopper thread: press Enter to stop
    std::thread stopper([](){
        std::string s;
        std::getline(std::cin, s);
        running = false;
    });
 
    const int POLL_TIMEOUT_MS = 1000; // 1 second
    while (running) {
        int nfds = (int)pollfds.size();
        int rc = WSAPoll(pollfds.data(), nfds, POLL_TIMEOUT_MS);
        if (rc == SOCKET_ERROR) {
            safe_print("WSAPoll failed: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }

        if (rc == 0) {
            // timeout, loop again
            continue;
        }
 
        // Check listening socket first (index 0)

        if (pollfds.size() > 0 && (pollfds[0].revents & POLLIN)) {
            sockaddr_in clientAddr;
            int addrlen = sizeof(clientAddr);
            SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrlen);
            if (clientSock != INVALID_SOCKET) {
                // Add to poll set (monitor for read)
                WSAPOLLFD pfd{};
                pfd.fd = clientSock;
                pfd.events = POLLIN;
                pfd.revents = 0;
                pollfds.push_back(pfd);
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
                safe_print("[Server] New client: " + std::string(ip) + ":" + std::to_string(ntohs(clientAddr.sin_port)) + "\n");
            } else {
                safe_print("[Server] accept failed: " + std::to_string(WSAGetLastError()) + "\n");
            }
        }
 
        // Handle client sockets (start from 1)
        for (int i = 1; i < (int)pollfds.size(); ++i) {
            WSAPOLLFD &p = pollfds[i];
            if (p.revents & POLLIN) {
                char buf[1024];
                int n = recv(p.fd, buf, sizeof(buf), 0);
                if (n > 0) {
                    // Echo back
                    int sent = 0;
                    while (sent < n) {
                        int s = send(p.fd, buf + sent, n - sent, 0);
                        if (s == SOCKET_ERROR) {
                            safe_print("[Server] send error: " + std::to_string(WSAGetLastError()) + "\n");
                            break;
                        }
                        sent += s;
                    }

                    // Print received (safe substring)
                    std::string msg(buf, buf + n);
                    safe_print("[Server] Received from fd " + std::to_string((int)p.fd) + ": " + msg + "\n");

                } else if (n == 0) {
                    // connection closed by client
                    safe_print("[Server] Client closed (fd " + std::to_string((int)p.fd) + ")\n");
                    closesocket(p.fd);

                    // remove this fd from vector
                    pollfds.erase(pollfds.begin() + i);
                    --i; // adjust index after erase

                } else {
                    int err = WSAGetLastError();
                    safe_print("[Server] recv error: " + std::to_string(err) + "\n");
                    closesocket(p.fd);
                    pollfds.erase(pollfds.begin() + i);
                    --i;
                }

            } else if (p.revents & (POLLERR | POLLHUP | POLLNVAL)) {

                // error/hangup: close and remove
                safe_print("[Server] Socket event error/hangup on fd " + std::to_string((int)p.fd) + "\n");
                closesocket(p.fd);
                pollfds.erase(pollfds.begin() + i);
                --i;

            }

            // Reset revents for next poll iteration

            p.revents = 0;

        }

    }
 
    safe_print("Server shutting down...\n");
 
    // Cleanup sockets

    for (auto &p : pollfds) {

        if (p.fd != INVALID_SOCKET) closesocket(p.fd);

    }

    pollfds.clear();
    closesocket(listenSock);
    if (stopper.joinable()) stopper.join();
    WSACleanup();
    return 0;

}

 // Explanation in detail about the code:
 // This code implements a simple echo server using Winsock on Windows. It listens for incoming TCP
    // connections on a specified port (default 9000) and echoes back any data received from clients.
    // The server uses the WSAPoll function to monitor multiple sockets for incoming data or new connections.
    // Key components of the code:
    // 1. Winsock Initialization: The code initializes the Winsock library using WSAStartup.
    // 2. Listening Socket: It creates a listening socket, binds it to the specified port, and starts listening for incoming connections.
    // 3. Polling Mechanism: The server maintains a vector of WSAPOLL
    // FD structures to monitor the listening socket and connected client sockets.
    // 4. Accepting Connections: When a new connection is detected on the listening socket
    //    (indicated by POLLIN event), the server accepts the connection and adds the new client socket to the poll set.
    // 5. Echoing Data: For each client socket, if data is available to read (POLLIN event), the server reads the data,
    //    echoes it back to the client, and prints the received message to the console.
    // 6. Handling Disconnections and Errors: If a client disconnects or an error occurs on a socket,
    //    the server closes the socket and removes it from the poll set.
    // 7. Graceful Shutdown: A separate thread waits for the user to press ENTER to signal the server to stop.
    //    Upon shutdown, the server cleans up all sockets and Winsock resources.


// Explaining each function used in the code and parameters used with their purpose:
// 1. WSAStartup(MAKEWORD(2,2), &wsa): This function initializes the Winsock library and must be called before any other Winsock functions. The MAKEWORD(2,2) parameter specifies the version of Winsock to use (2.2 in this case), and &wsa is a pointer to a WSADATA structure that will receive information about the Winsock implementation.
// 2. getaddrinfo(nullptr, port, &hints, &result): This function resolves the server address and port for binding. The first parameter is nullptr to indicate that the server should bind to all available interfaces. The second parameter is the port number as a string. The third parameter is a pointer to an addrinfo structure that specifies criteria for selecting the socket address structures returned. The fourth parameter is a pointer to a linked list of addrinfo structures that will hold the results.
// 3. socket(result->ai_family, result->ai_socktype, result->ai_protocol): This function creates a new socket. The parameters specify the address family (AF_INET for IPv4), the socket type (SOCK_STREAM for TCP), and the protocol (usually 0 to select the default protocol for the given socket type).
// 4. setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)): This function sets options on the socket. In this case, it sets the SO_REUSEADDR option to allow the socket to bind to an address that is already in use. This is useful for quickly restarting the server without waiting for the OS to release the port.
// 5. bind(listenSock, result->ai_addr, (int)result->ai_addrlen): This function binds the socket to the specified address and port. The first parameter is the socket to bind, the second parameter is a pointer to the sockaddr structure containing the address to bind to, and the third parameter is the length of that structure.
// 6. listen(listenSock, SOMAXCONN): This function marks the socket as a listening socket that will accept incoming connection requests. The second parameter specifies the maximum length of the queue of pending connections (SOMAXCONN is a constant that indicates the maximum allowed by the system).
// 7. WSAPoll(pollfds.data(), nfds, POLL_TIMEOUT_MS): This function monitors multiple sockets for events. The first parameter is a pointer to an array of WSAPOLLFD structures that specify the sockets to monitor. The second parameter is the number of sockets in the array. The third parameter is the timeout in milliseconds (POLL_TIMEOUT_MS) to wait for an event before returning.
// 8. accept(listenSock, (sockaddr*)&clientAddr, &addrlen): This function accepts an incoming connection on the listening socket. The first parameter is the listening socket, the second parameter is a pointer to a sockaddr structure that will receive the address of the connecting client, and the third parameter is a pointer to an integer that specifies the size of that structure.
// 9. recv(p.fd, buf, sizeof(buf), 0): This function receives data from a connected socket. The first parameter is the socket to read from, the second parameter is a buffer to store the received data, the third parameter is the size of the buffer, and the fourth parameter specifies flags (0 means no special options).
// 10. send(p.fd, buf + sent, n - sent, 0): This function sends data to a connected socket. The first parameter is the socket to send data to, the second parameter is a pointer to the data to send, the third parameter is the number of bytes to send, and the fourth parameter specifies flags (0 means no special options).
// 11. closesocket(p.fd): This function closes a socket. The parameter is the socket to close.
// 12. WSACleanup(): This function terminates the use of the Winsock library. It should be called when the application is done using Winsock functions. 
