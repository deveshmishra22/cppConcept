#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

void handle_client(SOCKET clientSocket, int id) {
    char buf[512];
    int bytesReceived;
    std::cout << "[Server] Client " << id << " connected\n";

    while ((bytesReceived = recv(clientSocket, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[bytesReceived] = '\0';
        std::cout << "[Server] Client " << id << " says: " << buf;

        // Echo message back to client
        send(clientSocket, buf, bytesReceived, 0);
    }

    std::cout << "[Server] Client " << id << " disconnected\n";
    closesocket(clientSocket);
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    std::cout << "[Server] Listening on port 9000...\n";

    int clientCount = 0;
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            clientCount++;
            std::thread(handle_client, clientSocket, clientCount).detach();
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}


// To test this server-client program:
// 1. Compile both server.cpp and client.cpp separately.
// 2. Run server.exe first, then client.exe.
// 3. You should see the connection messages on both sides.
// 4. You can type messages in the client console, and they will appear on the server console, and vice versa.
// 5. Type "exit" on either side to close the connection.
// Note: Make sure your firewall allows the program to use the specified port (8080).
// Also, both programs should be run on the same machine or within the same network for localhost (127.0.0.1).

// If you want to test over the internet, replace INADDR_ANY with your public IP address in server.cpp
// and use that IP address in client.cpp instead of "127.0.0.1".

// run g++ server.cpp -o server.exe -lws2_32 
// run g++ client.cpp -o client.exe -lws2_32
// Then run server.exe first, followed by client.exe in separate command prompts. As -lws2_32 is required for linking Winsock library on Windows. Because we are not using any IDE here and Pragma comment for linking is not added in code.
// Pragma comment way to link ws2_32.lib
// #pragma comment(lib, "ws2_32.lib") or you can add it in project settings if using an IDE.
