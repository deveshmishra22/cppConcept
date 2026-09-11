// Companion file for 4_nimbus_fleet_command_challenge.cpp
// Start this FIRST, in its own terminal, before testing Feature 4
// (sendReportOverTcp). It listens once, prints whatever report it
// receives, then exits — restart it before every test run.
#include <iostream>
#include <winsock2.h>
using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    listen(serverSocket, 1);

    cout << "[Relay] Listening on port 9090...\n";
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    cout << "[Relay] Agent connected.\n";

    char buffer[4096] = {0};
    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        cout << "[Relay] Report received:\n" << buffer << "\n";
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
