#include <iostream>
#include <thread>
#include <vector>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// --- Added explanation: handleClient(clientSocket) --------------------------
// Generic syntax:  void handleClient( SOCKET clientSocket )
//   clientSocket - parameter #1 (only parameter): the SOCKET handle for ONE
//                  already-accepted, already-connected client (handed in by
//                  accept() in main() below). Each client gets its own
//                  thread running this function with its own clientSocket.
void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;
    // --- Added explanation: recv(clientSocket, buffer, sizeof(buffer), 0) ----
    // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
    //   s     - parameter #1: the connected socket to read bytes from
    //           (clientSocket, this client's connection).
    //   buf   - parameter #2: destination buffer to write received bytes
    //           into (buffer, a 1024-byte local array).
    //   len   - parameter #3: max bytes to read in this call
    //           (sizeof(buffer), i.e. 1024).
    //   flags - parameter #4: receive flags; 0 means normal blocking receive.
    //   returns - number of bytes actually read (could be less than len),
    //             0 if the client closed the connection gracefully, or a
    //             negative value on error. This call BLOCKS until at least
    //             one byte arrives, the peer closes, or an error occurs -
    //             that is what lets the while-loop below wait for messages.
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Client says: " << buffer << std::endl;
        // --- Added explanation: send(clientSocket, buffer, bytesReceived, 0)
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: the socket to write to (clientSocket, the
        //           SAME client the bytes were just read from - this is
        //           what makes it an "echo").
        //   buf   - parameter #2: the bytes to send back (buffer, still
        //           holding what was just received).
        //   len   - parameter #3: how many bytes to send (bytesReceived,
        //           the exact count recv() just returned).
        //   flags - parameter #4: 0, no special send behavior.
        send(clientSocket, buffer, bytesReceived, 0); // echo back
    }
    // --- Added explanation: closesocket(clientSocket) --------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release
    //       (clientSocket). The while-loop above only exits once recv()
    //       returns <= 0 (client disconnected or an error), so this line is
    //       only reached after that happens.
    closesocket(clientSocket);
    std::cout << "Client disconnected.\n";
}

int main() {
    WSADATA wsaData;
    // --- Added explanation: WSAStartup(MAKEWORD(2, 2), &wsaData) -------------
    // Generic syntax:  WSAStartup( WORD versionRequested, LPWSADATA lpWSAData )
    //   versionRequested - parameter #1: Winsock version to request, packed
    //                      by MAKEWORD(2, 2) into "version 2.2".
    //   lpWSAData        - parameter #2: address of a WSADATA struct
    //                      (&wsaData) that Winsock fills in. Must be called
    //                      before ANY other Winsock function used below.
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // --- Added explanation: socket(AF_INET, SOCK_STREAM, 0) -------------------
    // Generic syntax:  SOCKET socket( int af, int type, int protocol )
    //   af       - parameter #1: address family; AF_INET means IPv4.
    //   type     - parameter #2: socket type; SOCK_STREAM means a reliable,
    //              connection-oriented TCP socket.
    //   protocol - parameter #3: 0 lets the OS pick the default protocol for
    //              this af/type combination (TCP, for AF_INET+SOCK_STREAM).
    //   returns a new, unbound, unconnected SOCKET handle - this will become
    //   the server's LISTENING socket once bind()/listen() are called below.
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    // --- Added explanation: htons(8080) ---------------------------------------
    // Generic syntax:  htons( u_short hostShort )
    //   hostShort - parameter #1 (only parameter): a 16-bit port number in
    //               the CPU's native ("host") byte order (8080). htons
    //               ("host to network short") converts it to the big-endian
    //               byte order TCP/IP requires on the wire.
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // --- Added explanation: bind(serverSocket, &serverAddr, sizeof(serverAddr))
    // Generic syntax:  bind( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the socket to bind (serverSocket, created
    //             above, not yet associated with any local address/port).
    //   name    - parameter #2: the local address/port to bind it to
    //             ((sockaddr*)&serverAddr - INADDR_ANY on port 8080, meaning
    //             "accept connections on any local network interface").
    //   namelen - parameter #3: size in bytes of that address structure
    //             (sizeof(serverAddr)).
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    // --- Added explanation: listen(serverSocket, 5) ---------------------------
    // Generic syntax:  listen( SOCKET s, int backlog )
    //   s       - parameter #1: the now-bound socket to start listening on
    //             (serverSocket).
    //   backlog - parameter #2: max number of pending (not-yet-accept()ed)
    //             connections the OS should queue up (5 here). Once this
    //             call returns, serverSocket is a passive listening socket -
    //             it can no longer send/recv directly; only accept() on it.
    listen(serverSocket, 5);

    std::cout << "Server started on port 8080...\n";

    std::vector<std::thread> threads;

    while (true) {
        // --- Added explanation: accept(serverSocket, nullptr, nullptr) -------
        // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
        //   s       - parameter #1: the listening socket to accept a pending
        //             connection from (serverSocket).
        //   addr    - parameter #2: optional out-parameter to receive the
        //             connecting client's address; nullptr here means "we
        //             don't care who's connecting, just accept it."
        //   addrlen - parameter #3: optional out-parameter for addr's size;
        //             also nullptr since addr itself is unused.
        //   This call BLOCKS until a client actually connects, then returns
        //   a brand-new SOCKET handle (clientSocket) dedicated to that one
        //   client - serverSocket itself keeps listening for the next one.
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        std::cout << "Client connected!\n";
        // --- Added explanation: threads.emplace_back(handleClient, clientSocket)
        // Generic syntax:  vectorObj.emplace_back( callableFunction, arg1 )
        //   callableFunction - parameter #1: the function the new thread
        //                      runs (handleClient, here).
        //   arg1 (clientSocket) - parameter #2: forwarded into handleClient's
        //                      `clientSocket` parameter. This constructs a
        //                      NEW std::thread in-place inside `threads`,
        //                      and it starts running handleClient(clientSocket)
        //                      immediately, concurrently with the while(true)
        //                      loop that keeps accepting further clients.
        threads.emplace_back(handleClient, clientSocket);  // spawn thread
        // --- Added explanation: threads.back().detach() ---------------------
        // Generic syntax:  threadObj.detach()
        //   (no parameters) - threads.back() (no parameters either) returns
        //   a reference to the std::thread just emplaced above; detach()
        //   severs this program's ability to join() that thread later, and
        //   lets it keep running fully independently in the background even
        //   after `threads` (and eventually the whole vector) goes out of
        //   scope. This is why the server never needs to join() its worker
        //   threads - but it also means main has NO way to wait for, or even
        //   know when, any given client's thread actually finishes.
        threads.back().detach();  // let it run independently
    }

    // --- Added explanation: closesocket(serverSocket) --------------------------
    // Generic syntax: same as closesocket(clientSocket) explained above (one
    // parameter: the socket handle to release) - here it's the listening
    // socket instead of a per-client one. In practice this line is
    // UNREACHABLE, since the while(true) loop above never breaks/returns.
    closesocket(serverSocket);
    // --- Added explanation: WSACleanup() ---------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - releases the Winsock resources WSAStartup acquired.
    //   Also unreachable here for the same reason as closesocket() above.
    WSACleanup();
    return 0;
}

// #include <iostream>
// #include <winsock2.h>
// #include <ws2tcpip.h>

// #pragma comment(lib, "ws2_32.lib") // Link Winsock library

// int main() {
//     WSADATA wsaData;
//     WSAStartup(MAKEWORD(2,2), &wsaData);

//     SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

//     sockaddr_in serverAddr{};
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(8080);

//     bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
//     listen(serverSocket, SOMAXCONN);

//     SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

//     char buffer[1024] = {0};
//     recv(clientSocket, buffer, sizeof(buffer), 0);
//     std::cout << "Message received: " << buffer << "\n";

//     closesocket(clientSocket);
//     closesocket(serverSocket);
//     WSACleanup();
//     return 0;
// }

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() initializes Winsock (WSAStartup), creates a TCP socket
//     (serverSocket), binds it to 0.0.0.0:8080 (INADDR_ANY + htons(8080)),
//     then listen()s on it with a backlog of 5 pending connections. "Server
//     started on port 8080..." is printed. An empty `threads` vector is
//     declared to hold one thread per connected client.
// #2  main() enters `while (true)` and immediately calls accept(), which
//     BLOCKS the main thread until some client connects (e.g. running
//     4_client.cpp, or several copies of it, pointed at this same machine's
//     port 8080).
// #3  When a client connects: accept() returns a NEW clientSocket dedicated
//     to that client. "Client connected!" prints, threads.emplace_back(...)
//     starts a NEW thread running handleClient(clientSocket), and
//     threads.back().detach() immediately releases that thread to run fully
//     independently. Control returns to the top of the while(true) loop,
//     where main calls accept() AGAIN right away - it does NOT wait for the
//     just-created client thread to do anything.
// #4  Because of #3, main can accept a second, third, ... client while the
//     first client's handleClient thread is still running - this is what
//     makes the server handle MULTIPLE clients CONCURRENTLY instead of one
//     at a time.
// #5  Inside each detached handleClient(clientSocket) thread (running
//     independently of main and of every other client's thread):
//       #5a  recv() blocks until that specific client sends bytes.
//       #5b  the received bytes are NUL-terminated and printed as
//            "Client says: <text>".
//       #5c  send() echoes those exact bytes back to that same client.
//       #5d  the while-loop goes back to recv() and repeats #5a-#5c for as
//            long as that client keeps sending messages.
//       #5e  once recv() returns <= 0 (the client closed its connection, or
//            an error occurred), the loop exits, closesocket(clientSocket)
//            releases that client's socket, and "Client disconnected." is
//            printed. This thread then ends (and, since it was detach()ed,
//            simply vanishes - nothing in main is waiting on it or will
//            ever know it finished).
// #6  This server has NO shutdown path: the `while (true)` loop in main()
//     never breaks, so accept() keeps blocking/accepting forever, and the
//     closesocket(serverSocket)/WSACleanup()/return 0 lines after the loop
//     are UNREACHABLE code as written - the only way to stop this program
//     is to kill the process (e.g. Ctrl+C).
// #7  Because `threads` is a std::vector that every accepted connection
//     pushes into (via emplace_back) but NOTHING ever erases from, its
//     std::thread entries (each already detach()ed and therefore no longer
//     joinable/manageable) accumulate for as long as the server runs and
//     keeps accepting clients - a slow, unbounded memory growth in a
//     long-lived server, though harmless for a short teaching demo.
// ============================================================================
