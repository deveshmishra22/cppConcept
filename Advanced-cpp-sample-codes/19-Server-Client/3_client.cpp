#include <iostream>
#include <winsock2.h>
using namespace std;

int main() {
    WSADATA wsaData;
    // --- Added explanation: WSAStartup(versionRequested, &wsaData) -------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct
    //                       (&wsaData) Winsock fills in. Must be the first
    //                       Winsock call in the program.
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // --- Added explanation: socket(family, type, protocol) ---------------------
    // Generic syntax:  socket( int af, int type, int protocol )
    //   af       - parameter #1: address family; AF_INET means IPv4.
    //   type     - parameter #2: SOCK_STREAM means a connection-oriented TCP
    //              socket.
    //   protocol - parameter #3: 0 lets the OS pick the default protocol
    //              (TCP) for this family/type.
    //   returns     a new SOCKET handle (clientSocket), not yet connected.
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    // --- Added explanation: htons(8080) -----------------------------------------
    // Generic syntax:  htons( u_short hostshort )
    //   hostshort (8080) - parameter #1 (only parameter): port number in
    //   host byte order; converted to network byte order for sin_port - must
    //   match the port the server (2_server.cpp) is listening on.
    serverAddr.sin_port = htons(8080);
    // --- Added explanation: inet_addr("127.0.0.1") ------------------------------
    // Generic syntax:  inet_addr( const char* cp )
    //   cp - parameter #1 (only parameter): a dotted-decimal IPv4 address
    //        string ("127.0.0.1", the loopback address - meaning "this same
    //        machine"). Returns that address packed into the 32-bit binary
    //        form sin_addr.s_addr requires.
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // --- Added explanation: connect(clientSocket, addr, addrlen) ---------------
    // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the not-yet-connected socket (clientSocket).
    //   name    - parameter #2: address/port to connect to
    //             ((sockaddr*)&serverAddr - 127.0.0.1:8080).
    //   namelen - parameter #3: size in bytes of the address structure
    //             (sizeof(serverAddr)).
    // Blocks until the TCP handshake with the server completes (or fails).
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    cout << "Connected to Server!" << endl;

    // making this two-way communication
    char buffer[1024];
    string message;

    while (true) {
        cout << "You: ";
        // --- Added explanation: getline(cin, message) ---------------------------
        // Generic syntax:  getline( istream& is, string& str )
        //   is  - parameter #1: input stream to read from (cin, the
        //         keyboard/console).
        //   str - parameter #2: string variable that receives the typed line
        //         (message). Blocks until Enter is pressed.
        getline(cin, message);
        // --- Added explanation: send(clientSocket, message.c_str(), len, flags)
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: destination socket (clientSocket).
        //   buf   - parameter #2: bytes to send (message.c_str()).
        //   len   - parameter #3: how many bytes to send (message.size()).
        //   flags - parameter #4: 0 means normal blocking send.
        send(clientSocket, message.c_str(), message.size(), 0);
        if (message == "exit") break;

        // --- Added explanation: recv(clientSocket, buffer, len, flags) ---------
        // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to read from
        //           (clientSocket).
        //   buf   - parameter #2: destination buffer (buffer, 1024 bytes).
        //   len   - parameter #3: max bytes to write into buf
        //           (sizeof(buffer)).
        //   flags - parameter #4: 0 means normal blocking receive.
        //   returns  bytes actually received; <= 0 means the server closed
        //           the connection or an error occurred.
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        cout << "Server: " << buffer << endl;

        if (string(buffer) == "exit") break;
    }

    // one-way communication
    // string message = "Hello Server!";
    // send(clientSocket, message.c_str(), message.size(), 0);

    // char buffer[1024] = {0};
    // recv(clientSocket, buffer, sizeof(buffer), 0);
    // cout << "Server replies: " << buffer << endl;

    // --- Added explanation: closesocket(clientSocket) --------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket to close
    //       (clientSocket). Releases the OS resources for this connection.
    closesocket(clientSocket);
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process.
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

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  WSAStartup() initializes Winsock; socket() creates clientSocket (TCP,
//     IPv4).
// #2  serverAddr is set to 127.0.0.1 (localhost) port 8080 - the address the
//     server (2_server.cpp) is expected to be listening on.
// #3  connect() performs the TCP handshake to that address; it BLOCKS until
//     it succeeds (server must already be running and past its accept()
//     call) or fails. "Connected to Server!" is printed once it returns.
// #4  The while(true) loop alternates directions each iteration, in the
//     OPPOSITE order from the server's loop:
//       #4a  prompts "You: " and getline(cin, ...) BLOCKS for the human at
//            this client's keyboard to type a message.
//       #4b  send() transmits that typed message to the server; if it was
//            "exit" the loop breaks immediately (no reply expected).
//       #4c  recv() BLOCKS waiting for the server's reply; if the server
//            disconnected (bytesReceived <= 0) the loop breaks.
//       #4d  the reply is null-terminated and printed as "Server: <text>";
//            if it equals "exit" the loop breaks too.
// #5  Once the loop breaks, closesocket(clientSocket) releases the
//     connection, WSACleanup() releases Winsock, and main() returns 0.
// #6  Net effect: this client always speaks FIRST each round (send before
//     recv), while 2_server.cpp always listens FIRST each round (recv
//     before send) - together they form one synchronous, turn-based chat:
//     client types -> server sees it and replies -> client sees the reply ->
//     repeat, until either side types "exit".
// ============================================================================