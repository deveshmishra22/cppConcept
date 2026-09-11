// file: simple_3clients_win.cpp

// Compile (MSVC): cl /EHsc simple_3clients_win.cpp ws2_32.lib

// MinGW: g++ -std=c++11 simple_3clients_win.cpp -lws2_32 -o simple_3clients.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
 
// --- Added explanation: client_task(id, serverIP, serverPort) --------------
// Generic syntax:  client_task( int id, const char* serverIP, const char* serverPort )
//   id         - parameter #1: a simple integer label (1, 2, or 3, assigned
//                by main() below) used only for the printed log messages,
//                so output from the 3 concurrent client threads can be told
//                apart.
//   serverIP   - parameter #2: hostname/IP string of the server to connect
//                to (defaults to "127.0.0.1", localhost, if not given on the
//                command line).
//   serverPort - parameter #3: port number, as a string, to connect to
//                (defaults to "9000").
void client_task(int id, const char* serverIP, const char* serverPort) {

    WSADATA wsa;
    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       Winsock fills in.
    //   returns              0 on success, nonzero on failure (checked
    //                       below). Each of the 3 client threads calls this
    //                       independently, since each runs client_task() as
    //                       its own separate function invocation.
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cout << "[Client " << id << "] WSAStartup failed\n";
        return;
    }
 
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* result = nullptr;

    // --- Added explanation: getaddrinfo(serverIP, serverPort, &hints, &result)
    // Generic syntax:  getaddrinfo( const char* pNodeName, const char* pServiceName,
    //                                const addrinfo* pHints, addrinfo** ppResult )
    //   pNodeName    - parameter #1: hostname/IP to resolve (serverIP, e.g.
    //                  "127.0.0.1").
    //   pServiceName - parameter #2: port as a string (serverPort, e.g.
    //                  "9000").
    //   pHints       - parameter #3: describes what kind of address is
    //                  wanted (&hints - IPv4 TCP, set up above).
    //   ppResult     - parameter #4: address of a pointer that receives the
    //                  resolved addrinfo list (&result); must later be
    //                  released with freeaddrinfo().
    //   returns         0 on success, nonzero on failure (checked below).
    if (getaddrinfo(serverIP, serverPort, &hints, &result) != 0) {
        std::cout << "[Client " << id << "] getaddrinfo failed\n";
        WSACleanup();
        return;
    }
 
    // --- Added explanation: socket(family, type, protocol) ---------------------
    // Generic syntax:  socket( int af, int type, int protocol )
    //   af       - parameter #1: address family, from the resolved result
    //              (result->ai_family, AF_INET here).
    //   type     - parameter #2: socket type, from the resolved result
    //              (result->ai_socktype, SOCK_STREAM/TCP here).
    //   protocol - parameter #3: protocol, from the resolved result
    //              (result->ai_protocol).
    //   returns     a new SOCKET handle (sock), not yet connected.
    SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET) {
        std::cout << "[Client " << id << "] socket() failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
        return;

    }
 
    // --- Added explanation: connect(sock, ai_addr, ai_addrlen) -----------------
    // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the not-yet-connected socket (sock).
    //   name    - parameter #2: address/port to connect to, from the
    //             resolved result (result->ai_addr).
    //   namelen - parameter #3: size of that address structure
    //             ((int)result->ai_addrlen).
    // Blocks until the TCP handshake with the server completes or fails.
    if (connect(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        std::cout << "[Client " << id << "] connect() failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        freeaddrinfo(result);
        WSACleanup();
        return;

    }
    // WSAGetLastError() returns the error code for the last Windows Sockets operation that failed.
    // It returns an integer value representing the specific error.
    // You can use this error code to diagnose and handle errors in your network programming.
    // like 10061 for connection refused.
    // It is useful for debugging and error handling in socket programming.
    // Similar to this other errors can be:
    // 10060 - Connection timed out
    // 10054 - Connection reset by peer
    // 10051 - Network is unreachable
    // 10065 - No route to host
    // 10048 - Address already in use
    // 10022 - Invalid argument
    // 10035 - Resource temporarily unavailable (non-blocking socket)
    // 10036 - Operation now in progress (non-blocking socket)

    // --- Added explanation: freeaddrinfo(result) --------------------------------
    // Generic syntax:  freeaddrinfo( addrinfo* ai )
    //   ai - parameter #1 (only parameter): the addrinfo list from
    //        getaddrinfo() (result); releases its memory now that connect()
    //        has read what it needed from it.
    freeaddrinfo(result);
    for (int i = 1; i <= 5; ++i) {
        std::string msg = "Client " + std::to_string(id) + " message " + std::to_string(i) + "\n";
        // --- Added explanation: send(sock, msg.c_str(), len, flags) ------------
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to write to (sock).
        //   buf   - parameter #2: bytes to send (msg.c_str()).
        //   len   - parameter #3: how many bytes to send ((int)msg.size()).
        //   flags - parameter #4: 0 means normal blocking send.
        //   returns  bytes actually sent, or SOCKET_ERROR.
        int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            std::cout << "[Client " << id << "] send() error: " << WSAGetLastError() << "\n";
            break;
        }
 
        char buf[512];
        // --- Added explanation: recv(sock, buf, len, flags) ---------------------
        // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to read from (sock).
        //   buf   - parameter #2: destination buffer (buf, 512 bytes).
        //   len   - parameter #3: max bytes to read (sizeof(buf) - 1, room
        //           left for the '\0' terminator added below).
        //   flags - parameter #4: 0 means normal blocking receive.
        //   returns  >0 bytes received, 0 = server closed the connection,
        //           <0 = error (handled by the if/else if/else below).
        int recvd = recv(sock, buf, sizeof(buf) - 1, 0);
        if (recvd > 0) {
            buf[recvd] = '\0';
            std::cout << "[Client " << id << "] Received: " << buf;
        } else if (recvd == 0) {
            std::cout << "[Client " << id << "] Server closed connection\n";
            break;

        } else {
            std::cout << "[Client " << id << "] recv() failed: " << WSAGetLastError() << "\n";
            break;
        }
 
        // --- Added explanation: sleep_for(milliseconds(300)) --------------------
        // Generic syntax:  std::this_thread::sleep_for( std::chrono::duration )
        //   duration - parameter #1 (only parameter): how long to pause THIS
        //              thread (std::chrono::milliseconds(300), i.e. 0.3
        //              seconds) before looping to send the next message -
        //              simply paces the 5 messages out instead of firing
        //              them all instantly.
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

    }
 
    // --- Added explanation: shutdown(sock, SD_BOTH) -----------------------------
    // Generic syntax:  shutdown( SOCKET s, int how )
    //   s   - parameter #1: the socket to shut down (sock).
    //   how - parameter #2: SD_BOTH disables further sends AND receives,
    //         gracefully signaling "no more data" before closesocket()
    //         releases the handle.
    shutdown(sock, SD_BOTH);
    // --- Added explanation: closesocket(sock) -----------------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release
    //       (sock).
    closesocket(sock);
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes this thread's WSAStartup() call, releasing
    //   Winsock's internal resources for this thread/process.
    WSACleanup();
    std::cout << "[Client " << id << "] Finished\n";

}
 
int main(int argc, char* argv[]) {

    const char* serverIP = (argc >= 2) ? argv[1] : "127.0.0.1";
    const char* serverPort = (argc >= 3) ? argv[2] : "9000";
    std::cout << "Starting 3 client threads connecting to " << serverIP << ":" << serverPort << "\n";
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i) {
        // --- Added explanation: threads.emplace_back(client_task, i, serverIP, serverPort)
        // Generic syntax:  vectorOfThread.emplace_back( callableFunction, arg1, arg2, arg3 )
        //   callableFunction - parameter #1: the function the new thread
        //                      runs (client_task).
        //   arg1 (i)         - parameter #2: forwarded into client_task's
        //                      `id` parameter (1, 2, then 3 across the 3
        //                      loop iterations).
        //   arg2 (serverIP)  - parameter #3: forwarded into client_task's
        //                      `serverIP` parameter.
        //   arg3 (serverPort)- parameter #4: forwarded into client_task's
        //                      `serverPort` parameter.
        // Constructs a NEW std::thread in-place inside the `threads` vector,
        // starting it immediately; unlike detach()-based examples in this
        // folder, these threads are kept (not detached) so main() can
        // .join() them below.
        threads.emplace_back(client_task, i, serverIP, serverPort);
    }
 
    // --- Added explanation: t.joinable() / t.join() -----------------------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() returns true if `t` still
    //   represents a live, not-yet-joined thread of execution; join()
    //   blocks the calling (main) thread until `t` finishes running
    //   client_task(). Looping this over every entry in `threads` waits for
    //   all 3 client threads to finish before main() prints the final
    //   message below.
    for (auto &t : threads) if (t.joinable()) t.join();
    std::cout << "All clients finished.\n";
    return 0;

}
 

// #include <winsock2.h>
// #include <windows.h>
// #include <iostream>
// #pragma comment(lib, "ws2_32.lib")
 
// DWORD WINAPI ClientHandler(LPVOID clientSocket) {

//     SOCKET client = (SOCKET)clientSocket;
//     char buffer[1024];
//     int bytesReceived;
 
//     while ((bytesReceived = recv(client, buffer, sizeof(buffer), 0)) > 0) {
//         buffer[bytesReceived] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
 
//         // Echo back to client
//         send(client, buffer, bytesReceived, 0);
//     }
 
//     std::cout << "Client disconnected." << std::endl;
//     closesocket(client);
//     return 0;
// }
 
// int main() {

//     WSADATA wsaData;
//     SOCKET listenSocket, clientSocket;
//     struct sockaddr_in serverAddr;
 
//     // Initialize Winsock
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         std::cerr << "WSAStartup failed." << std::endl;
//         return 1;
//     }
 
//     // Create socket
//     listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//     if (listenSocket == INVALID_SOCKET) {
//         std::cerr << "Socket creation failed." << std::endl;
//         WSACleanup();
//         return 1;
//     }
 
//     // Bind socket
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_port = htons(54000);

//     if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//         std::cerr << "Bind failed." << std::endl;
//         closesocket(listenSocket);
//         WSACleanup();
//         return 1;
//     }
 
//     // Listen
//     if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
//         std::cerr << "Listen failed." << std::endl;
//         closesocket(listenSocket);
//         WSACleanup();
//         return 1;

//     }
//     std::cout << "Server listening on port 54000..." << std::endl;
 
//     // Accept clients
//     while (true) {
//         clientSocket = accept(listenSocket, NULL, NULL);

//         if (clientSocket == INVALID_SOCKET) {
//             std::cerr << "Accept failed." << std::endl;
//             continue;
//         }
 
//         std::cout << "Client connected." << std::endl;
//         CreateThread(NULL, 0, ClientHandler, (LPVOID)clientSocket, 0, NULL);
//     }
 
//     // Cleanup
//     closesocket(listenSocket);
//     WSACleanup();
//     return 0;

// }
 
 


// #include <iostream>

// #include <cstring>

// #include <unistd.h>

// #include <arpa/inet.h>
// int main() {

//     int server_fd, client_fd;

//     sockaddr_in server_addr{}, client_addr{};

//     socklen_t addr_len = sizeof(client_addr);
 
   

//     //Creates a TCP socket (SOCK_STREAM) using IPv4 (AF_INET).

//     //Returns a file descriptor (server_fd) for the socket.

//     //If it fails, prints an error and exits.

//     server_fd = socket(AF_INET, SOCK_STREAM, 0);

//     if (server_fd == -1) {

//         perror("socket failed");

//         return 1;

//     }
 
//     // Bind

//     //Sets up the server address:

//    //AF_INET: IPv4

//   //INADDR_ANY: Accept connections on any local IP address

// //htons(8081): Converts port 8081 to network byte order
 
// //Binds the socket to the specified IP and port.

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
//     server_addr.sin_port = htons(8080);
 
//     if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
//         perror("bind failed");
//         close(server_fd);
//         return 1;
//     }
 
//     //  Listen
//     //Puts the socket into listening mode.
//     //The 5 is the backlog: max number of pending connection

//     if (listen(server_fd, 5) == -1) {
//         perror("listen failed");
//         close(server_fd);
//         return 1;

//     }
//     std::cout << "Server listening on port 8081...\n";
 
//     // Accept client
//    //Waits for a client to connect.
//    //Returns a new socket (client_fd) for communication with the client.
//     client_fd = accept(server_fd, (sockaddr*)&client_addr, &addr_len);

//     if (client_fd == -1) {
//         perror("accept failed");
//         close(server_fd);
//         return 1;
//     }
 
// //Reads up to 1023 bytes from the client.
// //Stores the data in buffer and null-terminates it.
//     char buffer[1024];
//     ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

//     if (bytes_read < 0) {
//         perror("recv failed");
//     } else {
//         buffer[bytes_read] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
 
//         // Echo back
//         // Sends the received message back to the client.
//         send(client_fd, buffer, bytes_read, 0);
//     }
 
//     // Close sockets
//     close(client_fd);
//     close(server_fd);
//     return 0;

// }
 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() reads serverIP/serverPort from argv (or defaults to
//     127.0.0.1:9000), then creates a std::vector<std::thread> and, in a
//     loop, emplace_back()s 3 NEW threads (i = 1, 2, 3), each immediately
//     running client_task(i, serverIP, serverPort) concurrently.
// #2  Inside each of the 3 concurrently-running client_task() calls:
//       #2a  WSAStartup() initializes Winsock for that thread.
//       #2b  getaddrinfo(serverIP, serverPort, ...) resolves the target
//            address; socket() creates a TCP socket; connect() performs the
//            TCP handshake to the server (this assumes some server, such as
//            1_simple_server.cpp or 4_simple_echo_server_win.cpp, is already
//            listening on that IP:port).
//       #2c  freeaddrinfo() releases the resolved address list.
//       #2d  A for-loop runs 5 times: send() transmits a labeled message
//            ("Client <id> message <i>"), then recv() blocks waiting for
//            the server's reply and prints it, then sleep_for(300ms) pauses
//            briefly before the next iteration.
//       #2e  After 5 messages (or an early break on error/disconnect),
//            shutdown() and closesocket() close the connection, WSACleanup()
//            releases Winsock, and "[Client <id>] Finished" is printed.
// #3  Back in main(), threads[i].joinable()/.join() waits for each of the
//     3 threads to finish, in vector order - main blocks on thread 1 until
//     it's done, then thread 2, then thread 3 (even though all 3 actually
//     RUN concurrently, since they were all started before any join() call).
// #4  Once all 3 have finished, main() prints "All clients finished." and
//     returns 0.
// #5  Net effect: this program simulates 3 independent clients hitting the
//     same server at (nearly) the same time, each exchanging 5 ping-pong
//     messages with it, useful for testing how a server (like
//     1_simple_server.cpp's per-client detached-thread design) handles
//     multiple simultaneous connections.
// #6  Everything from line ~118 onward (the large block of `//`-commented
//     code) is inert reference/scratch material, not part of this program's
//     compiled logic - it is left untouched and unexplained here since it
//     never executes.
// ============================================================================

