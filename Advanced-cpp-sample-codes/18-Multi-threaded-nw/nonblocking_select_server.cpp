// Compile: cl /EHsc nonblocking_select_server.cpp ws2_32.lib

// or: g++ -std=c++11 nonblocking_select_server.cpp -lws2_32 -o nonblocking_select_server.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <vector>
 
 
int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9000);
    bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSock, 5);

    std::cout << "Server listening on port 9000...\n";
 
    // List of sockets (first is the listening socket)

    std::vector<SOCKET> sockets;
    sockets.push_back(listenSock);
    while (true) {
        std::cout << "Waiting for activity...Entered While Loop!\n";
        fd_set readfds;
        FD_ZERO(&readfds); 
        // Add all sockets to the set
        for (SOCKET s : sockets)
            FD_SET(s, &readfds);
 
        // Wait (1 sec timeout)
        timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int activity = select(0, &readfds, nullptr, nullptr, &tv);
        if (activity == SOCKET_ERROR) break;
 
        // Check if new client is connecting

        if (FD_ISSET(listenSock, &readfds)) {
            SOCKET clientSock = accept(listenSock, nullptr, nullptr);
            sockets.push_back(clientSock);
            std::cout << "New client connected.\n";
        }
 
        // Check for data on client sockets
        for (size_t i = 1; i < sockets.size(); ++i) {
            SOCKET s = sockets[i];
            if (FD_ISSET(s, &readfds)) {
                char buffer[512];
                int bytes = recv(s, buffer, sizeof(buffer) - 1, 0);
                if (bytes <= 0) {

                    std::cout << "Client disconnected.\n";
                    closesocket(s);
                    sockets.erase(sockets.begin() + i);
                    --i;
                } else {

                    buffer[bytes] = '\0';
                    std::cout << "Received: " << buffer;
                    send(s, buffer, bytes, 0); // echo back
                }
            }
        }
    }
 
    for (SOCKET s : sockets)
        closesocket(s);
    WSACleanup();
    return 0;

}

 