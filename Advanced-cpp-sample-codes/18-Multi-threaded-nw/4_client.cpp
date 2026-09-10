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
    cout << "Connected to server." << endl;

    // 3_server.cpp echoes back whatever it receives, so this client just
    // sends one message and prints the echoed reply.
    string message = "Hello from client!";
    send(clientSocket, message.c_str(), (int)message.size(), 0);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Server echoed: " << buffer << endl;

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

// To test this server-client pair:
// 1. Compile both: g++ -std=c++17 3_server.cpp -o server -lws2_32
//                  g++ -std=c++17 4_client.cpp -o client -lws2_32
// 2. Run server.exe first, in its own terminal - it stays running, accepting
//    one connection after another (each on its own detached thread).
// 3. Run client.exe (in a second terminal) as many times as you like, even
//    at the same time from several terminals - the server handles each one
//    concurrently instead of making the others wait.
// Note: both programs must run on the same machine, or the same network for
// localhost (127.0.0.1); make sure your firewall allows port 8080.