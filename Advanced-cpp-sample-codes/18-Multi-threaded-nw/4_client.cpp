#include <iostream>
#include <winsock2.h>
using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    cout << "Connected to Ritu! ✅" << endl;

    // making this two-way communication
   // char buffer[1024];
    string message;

    // while (true) {
    //     cout << "You: ";
    //     getline(cin, message);
    //     send(clientSocket, message.c_str(), message.size(), 0);
    //     if (message == "exit") break;

    //     int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    //     if (bytesReceived <= 0) break;
    //     buffer[bytesReceived] = '\0';
    //     cout << "Ritu: " << buffer << endl;

    //     if (string(buffer) == "exit") break;
    // }
    string message = "Hello Ritu!";
    send(clientSocket, message.c_str(), message.size(), 0);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Ritu replies: " << buffer << endl;

    closesocket(clientSocket);
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