#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

// --- Added explanation: handle_client(clientSocket, id) --------------------
// Generic syntax:  handle_client( SOCKET clientSocket, int id )
//   clientSocket - parameter #1: the connected client's socket handle,
//                  returned by accept() in main() below; recv()/send() on
//                  this handle talk to that one specific client.
//   id           - parameter #2: a simple integer label for this client
//                  (assigned by main() as an incrementing counter), used only
//                  in the printed log messages below.
void handle_client(SOCKET clientSocket, int id) {
    char buf[512];
    int bytesReceived;
    std::cout << "[Server] Client " << id << " connected\n";

    // --- Added explanation: recv(clientSocket, buf, len, flags) -----------------
    // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
    //   s     - parameter #1: the connected socket to read bytes from
    //           (clientSocket, the client this worker thread is handling).
    //   buf   - parameter #2: destination buffer that receives the bytes
    //           (buf, a local 512-byte char array).
    //   len   - parameter #3: maximum number of bytes to write into buf
    //           (sizeof(buf) - 1, leaving 1 byte of room for a '\0'
    //           terminator added on the next line).
    //   flags - parameter #4: special receive options; 0 means "normal
    //           blocking receive, no special behavior".
    //   returns  the number of bytes actually received (used here as the
    //           while-loop condition: >0 keeps looping, 0 means the client
    //           closed the connection, <0 means an error).
    while ((bytesReceived = recv(clientSocket, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[bytesReceived] = '\0';
        std::cout << "[Server] Client " << id << " says: " << buf;

        // Echo message back to client
        // --- Added explanation: send(clientSocket, buf, bytesReceived, flags) ---
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: destination socket to write bytes to
        //           (clientSocket, the same client this data came from).
        //   buf   - parameter #2: source buffer holding the bytes to send
        //           (buf, containing whatever the client just sent).
        //   len   - parameter #3: how many bytes from buf to send
        //           (bytesReceived, so exactly what was read back is echoed).
        //   flags - parameter #4: special send options; 0 means "normal
        //           blocking send, no special behavior".
        send(clientSocket, buf, bytesReceived, 0);
    }

    std::cout << "[Server] Client " << id << " disconnected\n";
    // --- Added explanation: closesocket(clientSocket) --------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to close
    //       (clientSocket). Releases the OS resources for this connection;
    //       after this call, clientSocket must not be used again.
    closesocket(clientSocket);
}

int main() {
    WSADATA wsa;
    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: the Winsock version this program
    //                       wants, packed into a WORD by the MAKEWORD(2, 2)
    //                       macro (requesting Winsock version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       that Winsock fills in with details about the
    //                       implementation it actually loaded.
    //   returns              0 on success, nonzero on failure (checked below).
    // This must be the first Winsock call in the program; it initializes the
    // Winsock library for this process.
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    // --- Added explanation: socket(family, type, protocol) ---------------------
    // Generic syntax:  socket( int af, int type, int protocol )
    //   af       - parameter #1: address family; AF_INET means IPv4.
    //   type     - parameter #2: socket type; SOCK_STREAM means a reliable,
    //              connection-oriented TCP socket.
    //   protocol - parameter #3: specific protocol; 0 lets the OS pick the
    //              default protocol for the given family/type (TCP, here).
    //   returns     a new SOCKET handle (serverSocket) representing this
    //              endpoint, not yet bound to any address/port.
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    // --- Added explanation: htons(9000) -----------------------------------------
    // Generic syntax:  htons( u_short hostshort )
    //   hostshort - parameter #1 (only parameter): a 16-bit port number in
    //               the host machine's byte order (9000, the port this
    //               server will listen on); htons() converts it to network
    //               byte order (big-endian), which is what sin_port requires.
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // --- Added explanation: bind(serverSocket, addr, addrlen) ------------------
    // Generic syntax:  bind( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the socket to bind (serverSocket, created
    //             above).
    //   name    - parameter #2: pointer to the address/port to bind it to
    //             ((sockaddr*)&serverAddr - cast from sockaddr_in - meaning
    //             "any local IPv4 address, port 9000").
    //   namelen - parameter #3: size in bytes of the address structure
    //             (sizeof(serverAddr)).
    // After this call, serverSocket is associated with port 9000 on this
    // machine.
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    // --- Added explanation: listen(serverSocket, backlog) ----------------------
    // Generic syntax:  listen( SOCKET s, int backlog )
    //   s       - parameter #1: the bound socket to start listening on
    //             (serverSocket).
    //   backlog - parameter #2: maximum number of pending (not-yet-accepted)
    //             incoming connections the OS should queue up (5, here).
    // After this call, serverSocket is ready for accept() to pull incoming
    // connections off that queue.
    listen(serverSocket, 5);
    std::cout << "[Server] Listening on port 9000...\n";

    int clientCount = 0;
    while (true) {
        // --- Added explanation: accept(serverSocket, addr, addrlen) ------------
        // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
        //   s       - parameter #1: the listening socket to pull the next
        //             pending connection from (serverSocket).
        //   addr    - parameter #2: optional output buffer to receive the
        //             connecting client's address; nullptr here means "don't
        //             bother telling me who connected."
        //   addrlen - parameter #3: optional in/out size of that buffer;
        //             nullptr here since addr is also nullptr.
        //   returns    a NEW SOCKET handle (clientSocket) representing this
        //             one accepted connection, or INVALID_SOCKET if something
        //             went wrong. Blocks until a client connects.
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            clientCount++;
            // --- Added explanation: thread(handle_client, clientSocket, clientCount).detach()
            // Generic syntax:  std::thread( callableFunction, arg1, arg2 ).detach()
            //   callableFunction - constructor parameter #1: the function the
            //                      new thread runs (handle_client).
            //   arg1             - constructor parameter #2: forwarded into
            //                      handle_client's `clientSocket` parameter
            //                      (the socket just accepted).
            //   arg2             - constructor parameter #3: forwarded into
            //                      handle_client's `id` parameter
            //                      (clientCount, this client's sequence
            //                      number).
            //   .detach()        - severs this thread object from the thread
            //                      it's running; the thread keeps running
            //                      independently in the background and
            //                      cleans itself up when handle_client
            //                      returns, with no need (or ability) to
            //                      .join() it later.
            std::thread(handle_client, clientSocket, clientCount).detach();
        }
    }

    // --- Added explanation: closesocket(serverSocket) - same as explained
    // above for closesocket(clientSocket); closes the listening socket.
    closesocket(serverSocket);
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); tells Winsock this process is
    //   done using sockets so it can release its internal resources. Should
    //   be called once per successful WSAStartup() before the program exits.
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
// #1  main() calls WSAStartup(MAKEWORD(2,2), &wsa) to initialize Winsock 2.2
//     for this process; if it fails the program prints an error and returns.
// #2  socket(AF_INET, SOCK_STREAM, 0) creates a new TCP socket handle,
//     serverSocket, not yet bound to anything.
// #3  serverAddr is filled in: IPv4 family, port 9000 (in network byte order
//     via htons), address INADDR_ANY (accept connections on any local
//     network interface).
// #4  bind(serverSocket, &serverAddr, ...) attaches serverSocket to port 9000
//     on this machine; listen(serverSocket, 5) then puts it into listening
//     mode with room for up to 5 pending connections in the OS backlog.
// #5  main() enters an infinite while(true) loop and calls accept(), which
//     BLOCKS until some client program (e.g. 3_client.cpp) calls connect()
//     to this machine's port 9000.
// #6  When a client connects, accept() returns a new SOCKET (clientSocket)
//     dedicated to that one connection; clientCount is incremented, and a
//     brand-new std::thread is created running handle_client(clientSocket,
//     clientCount), then immediately .detach()'d so main's loop can go
//     straight back to accept() and wait for the NEXT client without
//     waiting for this one to finish.
// #7  Inside each detached thread, handle_client() loops on recv(), blocking
//     until that specific client sends data. Each time bytes arrive, it
//     null-terminates them, prints "[Server] Client <id> says: <text>", and
//     calls send() to echo the exact same bytes back to that same client.
// #8  This recv/send loop continues until the client disconnects (recv()
//     returns 0) or an error occurs (recv() returns <0), at which point the
//     loop exits, "disconnected" is printed, and closesocket(clientSocket)
//     releases that connection's resources. The thread function returns,
//     ending that detached thread.
// #9  Steps #5-#8 repeat forever for each new client - the server can have
//     many clients connected simultaneously, each served by its own
//     detached thread, all funneling through the single serverSocket for
//     new connections.
// #10 The closesocket(serverSocket)/WSACleanup()/return 0 lines after the
//     while(true) loop are unreachable in practice, since the loop never
//     breaks - this program is only stopped externally (e.g. closing the
//     console window or Ctrl+C), which the OS then cleans up after.
// ============================================================================
