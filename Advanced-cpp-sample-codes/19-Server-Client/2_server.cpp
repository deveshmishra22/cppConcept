#include <iostream>
#include <winsock2.h>
using namespace std;

int main() {
    WSADATA wsaData;
    // --- Added explanation: WSAStartup(versionRequested, &wsaData) -------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, packed
    //                       via MAKEWORD(2, 2) (asking for version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct
    //                       (&wsaData) that Winsock fills in with details
    //                       about the loaded implementation.
    // Must be the first Winsock call; its return value (0 = success) isn't
    // checked here, unlike in 1_simple_server.cpp.
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // --- Added explanation: socket(family, type, protocol) ---------------------
    // Generic syntax:  socket( int af, int type, int protocol )
    //   af       - parameter #1: address family; AF_INET means IPv4.
    //   type     - parameter #2: socket type; SOCK_STREAM means a reliable,
    //              connection-oriented TCP socket.
    //   protocol - parameter #3: 0 lets the OS pick the default protocol
    //              (TCP) for the given family/type.
    //   returns     a new SOCKET handle (serverSocket), not yet bound.
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    // --- Added explanation: htons(8080) -----------------------------------------
    // Generic syntax:  htons( u_short hostshort )
    //   hostshort (8080) - parameter #1 (only parameter): the port number in
    //   host byte order; htons() converts it to the network byte order that
    //   sin_port requires.
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // --- Added explanation: bind(serverSocket, addr, addrlen) ------------------
    // Generic syntax:  bind( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the socket to bind (serverSocket).
    //   name    - parameter #2: address/port to bind it to
    //             ((sockaddr*)&serverAddr - any local IPv4 address, port
    //             8080).
    //   namelen - parameter #3: size in bytes of the address structure
    //             (sizeof(serverAddr)).
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    // --- Added explanation: listen(serverSocket, backlog) ----------------------
    // Generic syntax:  listen( SOCKET s, int backlog )
    //   s       - parameter #1: the bound socket to listen on (serverSocket).
    //   backlog - parameter #2: max pending (not-yet-accepted) connections
    //             the OS should queue (5, here).
    listen(serverSocket, 5);

    cout << "(Server) waiting for Raushan to connect..." << endl;

    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    // --- Added explanation: accept(serverSocket, &clientAddr, &clientSize) -----
    // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
    //   s       - parameter #1: the listening socket to pull the next
    //             pending connection from (serverSocket).
    //   addr    - parameter #2: output buffer that receives the connecting
    //             client's address ((sockaddr*)&clientAddr - unlike
    //             1_simple_server.cpp, this one DOES record who connected).
    //   addrlen - parameter #3: in/out size of that buffer (&clientSize,
    //             initialized to sizeof(clientAddr) before the call, updated
    //             by accept() to the actual size written).
    //   returns    a NEW SOCKET (clientSocket) dedicated to this connection;
    //             blocks until a client calls connect().
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    cout << "Client connected!" << endl;

    char buffer[1024] = {0};

    // turing this to two-way communication
    string message;

    while (true) {
        // --- Added explanation: recv(clientSocket, buffer, len, flags) ---------
        // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to read from
        //           (clientSocket).
        //   buf   - parameter #2: destination buffer (buffer, 1024 bytes).
        //   len   - parameter #3: max bytes to write into buf
        //           (sizeof(buffer)).
        //   flags - parameter #4: 0 means normal blocking receive.
        //   returns  bytes actually received; <= 0 means the client
        //           disconnected or an error occurred (loop breaks below).
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;
        buffer[bytesReceived] = '\0';
        cout << "Client: " << buffer << endl;

        if (string(buffer) == "exit") break;

        cout << "Server: ";
        // --- Added explanation: getline(cin, message) ---------------------------
        // Generic syntax:  getline( istream& is, string& str )
        //   is  - parameter #1: input stream to read a line from (cin, the
        //         keyboard/console).
        //   str - parameter #2: string variable that receives the line typed
        //         (message), overwriting whatever it held before. Blocks
        //         until the person at the server's keyboard presses Enter.
        getline(cin, message);
        // --- Added explanation: send(clientSocket, message.c_str(), len, flags)
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: destination socket (clientSocket).
        //   buf   - parameter #2: bytes to send (message.c_str(), the
        //           null-terminated C-string view of what was just typed).
        //   len   - parameter #3: how many bytes to send (message.size()).
        //   flags - parameter #4: 0 means normal blocking send.
        send(clientSocket, message.c_str(), message.size(), 0);
        if (message == "exit") break;
    }

    // one-way communication
    // recv(clientSocket, buffer, sizeof(buffer), 0);
    // cout << "Client says: " << buffer << endl;

    // string reply = "Hi Client, message received!";
    // send(clientSocket, reply.c_str(), reply.size(), 0);

    // --- Added explanation: closesocket(s) --------------------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to close.
    //   Called twice below: first for clientSocket (this one connection),
    //   then for serverSocket (the listening socket) - same explanation
    //   applies to both, just a different handle each time.
    closesocket(clientSocket);
    closesocket(serverSocket);
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process. Called once, right before exiting.
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
// #1  WSAStartup() initializes Winsock; socket() creates serverSocket (TCP,
//     IPv4).
// #2  serverAddr is set to port 8080, any local address; bind() attaches
//     serverSocket to that port; listen() puts it into listening mode with
//     a backlog of 5.
// #3  "(Server) waiting for Raushan to connect..." is printed, then
//     accept() BLOCKS until a client (e.g. 3_client.cpp) connects, returning
//     clientSocket and filling in clientAddr with that client's address.
// #4  "Client connected!" is printed. buffer and message are prepared for
//     the chat loop.
// #5  The while(true) loop alternates directions each iteration:
//       #5a  recv() blocks waiting for the CLIENT to send a message; if the
//            client disconnects (bytesReceived <= 0) the loop breaks.
//       #5b  the received text is null-terminated and printed as
//            "Client: <text>"; if it equals "exit" the loop breaks
//            immediately (server does not reply to an "exit" message).
//       #5c  otherwise the server prompts "Server: " and getline(cin, ...)
//            BLOCKS waiting for a human at the server's own keyboard to type
//            a reply.
//       #5d  that typed reply is sent back to the client via send(); if the
//            reply text itself was "exit" the loop breaks too.
// #6  Once the loop breaks (either side typed "exit", or the client
//     disconnected), closesocket() is called on clientSocket then
//     serverSocket, WSACleanup() releases Winsock, and main() returns 0.
// #7  Net effect: this is a synchronous, turn-based two-way chat between one
//     server (typed at its own console) and one client - the server waits
//     for the client to speak, replies, then waits again, for as long as
//     neither side types "exit".
// ============================================================================