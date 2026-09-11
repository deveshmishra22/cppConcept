#include <iostream>
#include <winsock2.h>
using namespace std;

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
    //              this af/type combination (TCP, here). Returns a new,
    //              unconnected SOCKET handle.
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    // --- Added explanation: htons(8080) ---------------------------------------
    // Generic syntax:  htons( u_short hostShort )
    //   hostShort - parameter #1 (only parameter): a 16-bit port number in
    //               the CPU's native ("host") byte order (8080). htons
    //               converts it to the big-endian byte order TCP/IP
    //               requires on the wire - must match the port 3_server.cpp
    //               bound with the same htons(8080) call.
    serverAddr.sin_port = htons(8080);
    // --- Added explanation: inet_addr("127.0.0.1") ----------------------------
    // Generic syntax:  inet_addr( const char* cp )
    //   cp - parameter #1 (only parameter): a dotted-decimal IPv4 address
    //        string ("127.0.0.1", i.e. localhost). Returns that address
    //        packed into the 32-bit binary form sin_addr.s_addr needs.
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // --- Added explanation: connect(clientSocket, &serverAddr, sizeof(...))
    // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the unconnected socket to connect
    //             (clientSocket, created above).
    //   name    - parameter #2: address of the server to connect to
    //             ((sockaddr*)&serverAddr - 127.0.0.1:8080, matching where
    //             3_server.cpp binds and listens).
    //   namelen - parameter #3: size in bytes of that address structure
    //             (sizeof(serverAddr)). Blocks until the TCP handshake with
    //             the server completes (or fails, e.g. if no server is
    //             listening on that port).
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    cout << "Connected to server." << endl;

    // 3_server.cpp echoes back whatever it receives, so this client just
    // sends one message and prints the echoed reply.
    string message = "Hello from client!";
    // --- Added explanation: send(clientSocket, message.c_str(), size, 0) -----
    // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
    //   s     - parameter #1: the connected socket to write to (clientSocket).
    //   buf   - parameter #2: pointer to the bytes to send
    //           (message.c_str(), the C-string form of "Hello from client!").
    //   len   - parameter #3: how many bytes to send
    //           ((int)message.size(), the message's length).
    //   flags - parameter #4: 0, no special send behavior.
    send(clientSocket, message.c_str(), (int)message.size(), 0);

    char buffer[1024] = {0};
    // --- Added explanation: recv(clientSocket, buffer, sizeof(buffer), 0) ----
    // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
    //   s     - parameter #1: the connected socket to read bytes from
    //           (clientSocket - same socket the message was just sent on).
    //   buf   - parameter #2: destination buffer for received bytes
    //           (buffer, a 1024-byte array pre-zeroed by `= {0}` above).
    //   len   - parameter #3: max bytes to read (sizeof(buffer), i.e. 1024).
    //   flags - parameter #4: 0, normal blocking receive.
    //   This call BLOCKS until 3_server.cpp's handleClient thread echoes
    //   the message back (or the connection is closed/errors). Unlike
    //   3_server.cpp, this is a single recv() call, not a loop - it reads
    //   ONE reply and moves on, whatever byte count comes back.
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Server echoed: " << buffer << endl;

    // --- Added explanation: closesocket(clientSocket) --------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release back
    //       to the OS (clientSocket). After this call it must not be reused.
    closesocket(clientSocket);
    // --- Added explanation: WSACleanup() ---------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - releases the Winsock resources WSAStartup acquired
    //   at the top of main(). Should be the last Winsock-related call.
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

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() initializes Winsock (WSAStartup), creates a TCP socket
//     (clientSocket), fills serverAddr with 127.0.0.1:8080 (via htons(8080)
//     and inet_addr("127.0.0.1")), then connect()s clientSocket to that
//     address - this BLOCKS until a server is listening on 127.0.0.1:8080
//     and accepts (e.g. 3_server.cpp, run first in its own terminal) or
//     fails/errors if nothing is listening there.
// #2  Once connected, "Connected to server." is printed.
// #3  send(clientSocket, "Hello from client!", 19, 0) writes that message's
//     bytes onto the socket. On the server side (3_server.cpp), whichever
//     handleClient thread owns this connection is blocked in its own
//     recv() call and wakes up with these bytes.
// #4  recv(clientSocket, buffer, 1024, 0) BLOCKS this client until the
//     server's handleClient thread echoes the same bytes back via its own
//     send() call. Once they arrive, buffer holds "Hello from client!"
//     again (echoed), and "Server echoed: Hello from client!" is printed.
// #5  closesocket(clientSocket) closes this end of the connection; on the
//     server side, the corresponding recv() in handleClient will then
//     return 0 (graceful close), which is what causes THAT server thread to
//     exit its while-loop, print "Client disconnected.", and end.
// #6  WSACleanup() releases Winsock resources and main() returns 0, ending
//     this one-shot client run. Unlike the server, this client sends
//     exactly one message and reads exactly one reply, then exits - running
//     this executable again starts a brand-new, separate connection.
// ============================================================================
