// file: simple_3clients_win.cpp

// Compile (MSVC): cl /EHsc simple_3clients_win.cpp ws2_32.lib

// MinGW: g++ -std=c++11 simple_3clients_win.cpp -lws2_32 -o simple_3clients.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
 
void client_task(int id, const char* serverIP, const char* serverPort) {

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cout << "[Client " << id << "] WSAStartup failed\n";
        return;
    }
 
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* result = nullptr;

    if (getaddrinfo(serverIP, serverPort, &hints, &result) != 0) {
        std::cout << "[Client " << id << "] getaddrinfo failed\n";
        WSACleanup();
        return;
    }
 
    SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET) {
        std::cout << "[Client " << id << "] socket() failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
        return;

    }
 
    if (connect(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        std::cout << "[Client " << id << "] connect() failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        freeaddrinfo(result);
        WSACleanup();
        return;

    }
    // WSAGetLastError() returns the error code for the last Windows Sockets operation that failed.
    // It returns an integer value representing the specific error.
    // You can use this error code to diagnose and handle errors in your network programming.
    // like 10061 for connection refused.
    // It is useful for debugging and error handling in socket programming.
    // Similar to this other errors can be:
    // 10060 - Connection timed out
    // 10054 - Connection reset by peer
    // 10051 - Network is unreachable
    // 10065 - No route to host
    // 10048 - Address already in use
    // 10022 - Invalid argument
    // 10035 - Resource temporarily unavailable (non-blocking socket)
    // 10036 - Operation now in progress (non-blocking socket)

    freeaddrinfo(result);
    for (int i = 1; i <= 5; ++i) {
        std::string msg = "Client " + std::to_string(id) + " message " + std::to_string(i) + "\n";
        int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            std::cout << "[Client " << id << "] send() error: " << WSAGetLastError() << "\n";
            break;
        }
 
        char buf[512];
        int recvd = recv(sock, buf, sizeof(buf) - 1, 0);
        if (recvd > 0) {
            buf[recvd] = '\0';
            std::cout << "[Client " << id << "] Received: " << buf;
        } else if (recvd == 0) {
            std::cout << "[Client " << id << "] Server closed connection\n";
            break;

        } else {
            std::cout << "[Client " << id << "] recv() failed: " << WSAGetLastError() << "\n";
            break;
        }
 
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

    }
 
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    WSACleanup();
    std::cout << "[Client " << id << "] Finished\n";

}
 
int main(int argc, char* argv[]) {

    const char* serverIP = (argc >= 2) ? argv[1] : "127.0.0.1";
    const char* serverPort = (argc >= 3) ? argv[2] : "9000";
    std::cout << "Starting 3 client threads connecting to " << serverIP << ":" << serverPort << "\n";
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(client_task, i, serverIP, serverPort);
    }
 
    for (auto &t : threads) if (t.joinable()) t.join();
    std::cout << "All clients finished.\n";
    return 0;

}
 

// #include <winsock2.h>
// #include <windows.h>
// #include <iostream>
// #pragma comment(lib, "ws2_32.lib")
 
// DWORD WINAPI ClientHandler(LPVOID clientSocket) {

//     SOCKET client = (SOCKET)clientSocket;
//     char buffer[1024];
//     int bytesReceived;
 
//     while ((bytesReceived = recv(client, buffer, sizeof(buffer), 0)) > 0) {
//         buffer[bytesReceived] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
 
//         // Echo back to client
//         send(client, buffer, bytesReceived, 0);
//     }
 
//     std::cout << "Client disconnected." << std::endl;
//     closesocket(client);
//     return 0;
// }
 
// int main() {

//     WSADATA wsaData;
//     SOCKET listenSocket, clientSocket;
//     struct sockaddr_in serverAddr;
 
//     // Initialize Winsock
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         std::cerr << "WSAStartup failed." << std::endl;
//         return 1;
//     }
 
//     // Create socket
//     listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//     if (listenSocket == INVALID_SOCKET) {
//         std::cerr << "Socket creation failed." << std::endl;
//         WSACleanup();
//         return 1;
//     }
 
//     // Bind socket
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(54000);

//     if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//         std::cerr << "Bind failed." << std::endl;
//         closesocket(listenSocket);
//         WSACleanup();
//         return 1;
//     }
 
//     // Listen
//     if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
//         std::cerr << "Listen failed." << std::endl;
//         closesocket(listenSocket);
//         WSACleanup();
//         return 1;

//     }
//     std::cout << "Server listening on port 54000..." << std::endl;
 
//     // Accept clients
//     while (true) {
//         clientSocket = accept(listenSocket, NULL, NULL);

//         if (clientSocket == INVALID_SOCKET) {
//             std::cerr << "Accept failed." << std::endl;
//             continue;
//         }
 
//         std::cout << "Client connected." << std::endl;
//         CreateThread(NULL, 0, ClientHandler, (LPVOID)clientSocket, 0, NULL);
//     }
 
//     // Cleanup
//     closesocket(listenSocket);
//     WSACleanup();
//     return 0;

// }
 
 


// #include <iostream>

// #include <cstring>

// #include <unistd.h>

// #include <arpa/inet.h>
// int main() {

//     int server_fd, client_fd;

//     sockaddr_in server_addr{}, client_addr{};

//     socklen_t addr_len = sizeof(client_addr);
 
   

//     //Creates a TCP socket (SOCK_STREAM) using IPv4 (AF_INET).

//     //Returns a file descriptor (server_fd) for the socket.

//     //If it fails, prints an error and exits.

//     server_fd = socket(AF_INET, SOCK_STREAM, 0);

//     if (server_fd == -1) {

//         perror("socket failed");

//         return 1;

//     }
 
//     // Bind

//     //Sets up the server address:

//    //AF_INET: IPv4

//   //INADDR_ANY: Accept connections on any local IP address

// //htons(8081): Converts port 8081 to network byte order
 
// //Binds the socket to the specified IP and port.

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
//     server_addr.sin_port = htons(8080);
 
//     if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
//         perror("bind failed");
//         close(server_fd);
//         return 1;
//     }
 
//     //  Listen
//     //Puts the socket into listening mode.
//     //The 5 is the backlog: max number of pending connection

//     if (listen(server_fd, 5) == -1) {
//         perror("listen failed");
//         close(server_fd);
//         return 1;

//     }
//     std::cout << "Server listening on port 8081...\n";
 
//     // Accept client
//    //Waits for a client to connect.
//    //Returns a new socket (client_fd) for communication with the client.
//     client_fd = accept(server_fd, (sockaddr*)&client_addr, &addr_len);

//     if (client_fd == -1) {
//         perror("accept failed");
//         close(server_fd);
//         return 1;
//     }
 
// //Reads up to 1023 bytes from the client.
// //Stores the data in buffer and null-terminates it.
//     char buffer[1024];
//     ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

//     if (bytes_read < 0) {
//         perror("recv failed");
//     } else {
//         buffer[bytes_read] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
 
//         // Echo back
//         // Sends the received message back to the client.
//         send(client_fd, buffer, bytes_read, 0);
//     }
 
//     // Close sockets
//     close(client_fd);
//     close(server_fd);
//     return 0;

// }
 
