#include <iostream>
#include <thread>
#include <vector>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Client says: " << buffer << std::endl;
        send(clientSocket, buffer, bytesReceived, 0); // echo back
    }
    closesocket(clientSocket);
    std::cout << "Client disconnected.\n";
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    std::cout << "Server started on port 8080...\n";

    std::vector<std::thread> threads;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        std::cout << "Client connected!\n";
        threads.emplace_back(handleClient, clientSocket);  // spawn thread
        threads.back().detach();  // let it run independently
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
