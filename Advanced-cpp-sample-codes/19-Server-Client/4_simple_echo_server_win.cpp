// file: simple_echo_server_win.cpp

// Compile (MSVC): cl /EHsc simple_echo_server_win.cpp ws2_32.lib

// MinGW: g++ -std=c++11 simple_echo_server_win.cpp -lws2_32 -o simple_echo_server.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <mutex>
#pragma comment(lib, "Ws2_32.lib")
std::mutex cout_mtx;

// --- Added explanation: ts_print(s) -----------------------------------------
// Generic syntax:  ts_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so that multiple worker
//   threads (one per connected client) never interleave their output
//   mid-line - each call to ts_print() prints one complete, uninterrupted
//   line before releasing the mutex.
void ts_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::endl;
}
 
// Handles a single client: echoes back received data

// --- Added explanation: client_handler(clientSock, clientAddr) -------------
// Generic syntax:  client_handler( SOCKET clientSock, sockaddr_in clientAddr )
//   clientSock - parameter #1: the connected client's socket handle,
//                returned by accept() in main() below.
//   clientAddr - parameter #2: a COPY of that client's address structure
//                (also filled in by accept()); used here only to log the
//                client's IP/port, not to identify the connection (that's
//                what clientSock is for).
void client_handler(SOCKET clientSock, sockaddr_in clientAddr) {
    // --- Added explanation: inet_ntoa(clientAddr.sin_addr) --------------------
    // Generic syntax:  inet_ntoa( struct in_addr in )
    //   in - parameter #1 (only parameter): a binary IPv4 address
    //        (clientAddr.sin_addr) to convert into a human-readable
    //        dotted-decimal string (e.g. "127.0.0.1").
    char *clientIp = inet_ntoa(clientAddr.sin_addr);
    // --- Added explanation: ntohs(clientAddr.sin_port) ------------------------
    // Generic syntax:  ntohs( u_short netshort )
    //   netshort - parameter #1 (only parameter): a 16-bit port number in
    //              network byte order (clientAddr.sin_port); ntohs()
    //              converts it back to host byte order so it prints/compares
    //              as the expected decimal port number.
    int clientPort = ntohs(clientAddr.sin_port);
    ts_print("[Worker] Connected: " + std::string(clientIp) + ":" + std::to_string(clientPort));
    const int BUFSIZE = 1024;
    char buf[BUFSIZE];
 
    while (true) {
        // --- Added explanation: recv(clientSock, buf, len, flags) ---------------
        // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to read from (clientSock).
        //   buf   - parameter #2: destination buffer (buf, BUFSIZE bytes).
        //   len   - parameter #3: max bytes to read (BUFSIZE, 1024).
        //   flags - parameter #4: 0 means normal blocking receive.
        //   returns  >0 bytes received, 0 = client disconnected cleanly,
        //           <0 = error (handled by the if/else if/else below).
        int received = recv(clientSock, buf, BUFSIZE, 0);

        if (received > 0) {
            // Echo back
            int sent_total = 0;
            while (sent_total < received) {
                // --- Added explanation: send(clientSock, buf+off, len, flags) -------
                // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
                //   s     - parameter #1: destination socket (clientSock).
                //   buf   - parameter #2: pointer into the receive buffer,
                //           offset by how much has already been sent this
                //           round (buf + sent_total) - since send() is not
                //           guaranteed to send everything in one call, this
                //           loop resumes from where the last partial send()
                //           left off.
                //   len   - parameter #3: remaining bytes still to send
                //           (received - sent_total).
                //   flags - parameter #4: 0 means normal blocking send.
                //   returns  bytes actually sent this call, or SOCKET_ERROR.
                int s = send(clientSock, buf + sent_total, received - sent_total, 0);
                if (s == SOCKET_ERROR) {
                    ts_print("[Worker] send() error: " + std::to_string(WSAGetLastError()));
                    break;
                }
                sent_total += s;
            }

            // Print what we got (safe substring)
            std::string msg(buf, buf + received);
            ts_print("[Worker] From " + std::string(clientIp) + ":" + std::to_string(clientPort) + " -> " + msg);
        } else if (received == 0) {

            ts_print("[Worker] Client disconnected: " + std::string(clientIp) + ":" + std::to_string(clientPort));
            break;
        } else {
            ts_print("[Worker] recv() failed: " + std::to_string(WSAGetLastError()));
            break;
        }
    }
 
    // --- Added explanation: shutdown(clientSock, SD_BOTH) ----------------------
    // Generic syntax:  shutdown( SOCKET s, int how )
    //   s   - parameter #1: the socket to shut down (clientSock).
    //   how - parameter #2: which directions to disable; SD_BOTH disables
    //         both further sends AND receives on this socket (a graceful
    //         notice to the peer that no more data is coming), before the
    //         handle itself is released by closesocket() below.
    shutdown(clientSock, SD_BOTH);
    // --- Added explanation: closesocket(clientSock) -----------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release
    //       (clientSock).
    closesocket(clientSock);
    ts_print("[Worker] Closed: " + std::string(clientIp) + ":" + std::to_string(clientPort));

}
 
int main(int argc, char* argv[]) {
    const char* portStr = (argc >= 2) ? argv[1] : "9000";
    ts_print("Simple multithreaded echo server (Windows) - listening on port " + std::string(portStr));
 
    // Init Winsock
    WSADATA wsa;

    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       Winsock fills in.
    //   returns              0 on success, nonzero on failure (checked
    //                       below).
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        ts_print("WSAStartup failed");
        return 1;
    }
 
    // Create socket
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addrinfo* result = nullptr;

    // --- Added explanation: getaddrinfo(node, service, &hints, &result) --------
    // Generic syntax:  getaddrinfo( const char* pNodeName, const char* pServiceName,
    //                                const addrinfo* pHints, addrinfo** ppResult )
    //   pNodeName    - parameter #1: hostname/IP to resolve; NULL here (with
    //                  AI_PASSIVE in hints) means "any local address I can
    //                  bind/listen on".
    //   pServiceName - parameter #2: port number or service name as a string
    //                  (portStr, e.g. "9000" - taken from argv[1] or
    //                  defaulted above).
    //   pHints       - parameter #3: an addrinfo describing what KIND of
    //                  result is wanted (&hints - IPv4, TCP, passive/server
    //                  mode, set up above).
    //   ppResult     - parameter #4: address of a pointer that receives a
    //                  linked list of matching addrinfo results (&result);
    //                  must be released later with freeaddrinfo().
    //   returns         0 on success, nonzero on failure (checked below).
    if (getaddrinfo(NULL, portStr, &hints, &result) != 0) {
        ts_print("getaddrinfo failed");
        WSACleanup();
        return 1;
    }

    // --- Added explanation: socket(family, type, protocol) ---------------------
    // Generic syntax:  socket( int af, int type, int protocol )
    //   af       - parameter #1: address family, taken from the resolved
    //              result (result->ai_family, AF_INET here).
    //   type     - parameter #2: socket type, taken from the resolved result
    //              (result->ai_socktype, SOCK_STREAM/TCP here).
    //   protocol - parameter #3: protocol, taken from the resolved result
    //              (result->ai_protocol).
    //   returns     a new SOCKET handle (listenSock), not yet bound.
    SOCKET listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSock == INVALID_SOCKET) {
        ts_print("socket() failed: " + std::to_string(WSAGetLastError()));
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
 
    // reuse addr
    int opt = 1;
    // --- Added explanation: setsockopt(listenSock, level, optname, optval, optlen)
    // Generic syntax:  setsockopt( SOCKET s, int level, int optname,
    //                               const char* optval, int optlen )
    //   s       - parameter #1: the socket to configure (listenSock).
    //   level   - parameter #2: which option group; SOL_SOCKET means a
    //             generic socket-level option (not protocol-specific).
    //   optname - parameter #3: which option; SO_REUSEADDR lets this socket
    //             bind to a port that's still in a brief post-close
    //             "lingering" state from a previous run, so restarting the
    //             server quickly doesn't fail with "address in use".
    //   optval  - parameter #4: pointer to the option's new value
    //             ((const char*)&opt, opt = 1 meaning "enable").
    //   optlen  - parameter #5: size in bytes of that value (sizeof(opt)).
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
 
    // --- Added explanation: bind(listenSock, ai_addr, ai_addrlen) ---------------
    // Generic syntax:  bind( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the socket to bind (listenSock).
    //   name    - parameter #2: address/port to bind to, taken from the
    //             resolved result (result->ai_addr).
    //   namelen - parameter #3: size of that address structure
    //             ((int)result->ai_addrlen).
    if (bind(listenSock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        ts_print("bind() failed: " + std::to_string(WSAGetLastError()));
        closesocket(listenSock);
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    // --- Added explanation: freeaddrinfo(result) --------------------------------
    // Generic syntax:  freeaddrinfo( addrinfo* ai )
    //   ai - parameter #1 (only parameter): the addrinfo list returned by
    //        getaddrinfo() (result); releases the memory it allocated, now
    //        that bind() has read what it needed from it.
    freeaddrinfo(result);

    // --- Added explanation: listen(listenSock, backlog) -------------------------
    // Generic syntax:  listen( SOCKET s, int backlog )
    //   s       - parameter #1: the bound socket to listen on (listenSock).
    //   backlog - parameter #2: max pending connections to queue;
    //             SOMAXCONN asks the OS for its maximum reasonable value
    //             instead of a fixed small number like 5.
    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        ts_print("listen() failed: " + std::to_string(WSAGetLastError()));
        closesocket(listenSock);
        WSACleanup();
        return 1;
    } 
    ts_print("Server ready. Connect clients (e.g. run the provided client). Ctrl+C to stop.");

    // Accept loop: spawn a thread per client (detached)

    while (true) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        // --- Added explanation: accept(listenSock, &clientAddr, &addrLen) ------
        // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
        //   s       - parameter #1: the listening socket to pull the next
        //             pending connection from (listenSock).
        //   addr    - parameter #2: output buffer that receives the
        //             connecting client's address ((sockaddr*)&clientAddr).
        //   addrlen - parameter #3: in/out size of that buffer (&addrLen,
        //             initialized to sizeof(clientAddr)).
        //   returns    a NEW SOCKET (clientSock) for this one connection, or
        //             INVALID_SOCKET on failure. Blocks until a client
        //             connects.
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);

        if (clientSock == INVALID_SOCKET) {
            ts_print("accept() failed: " + std::to_string(WSAGetLastError()));
            break;
        }
 
        // Spawn worker thread and detach it (simple)
        // --- Added explanation: thread(worker_thread, client_handler, clientSock, clientAddr).detach()
        // Generic syntax:  std::thread( callableFunction, arg1, arg2, ... ).detach()
        //   callableFunction - constructor parameter #1: intended to be the
        //                      function the new thread runs.
        //   arg1..argN       - constructor parameters #2+: forwarded to
        //                      callableFunction.
        //   .detach()        - lets the thread run independently in the
        //                      background with no need to .join() it later.
        //   NOTE (pre-existing bug, left as-is per instructions not to alter
        //   code logic): `worker_thread` is not a declared identifier
        //   anywhere in this file - only `client_handler` is defined above.
        //   This line as written does NOT compile ('worker_thread' was not
        //   declared in this scope). The comment on the line right below
        //   this call even shows the presumably-intended fix: construct the
        //   thread with client_handler as parameter #1 instead, e.g.
        //   std::thread(client_handler, clientSock, clientAddr).detach();
        std::thread(worker_thread, client_handler, clientSock, clientAddr).detach();

        // Note: lambda alternative if compiler complains about mismatch:
        // std::thread([clientSock, clientAddr]{ client_handler(clientSock, clientAddr); }).detach();
    }
 
    // --- Added explanation: closesocket(listenSock) / WSACleanup() - same
    // explanations as given earlier in this file for closesocket(clientSock)
    // and WSAStartup()'s counterpart; closes the listening socket and
    // releases Winsock's resources before the program exits.
    closesocket(listenSock);
    WSACleanup();
    return 0;

}
 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() picks the port from argv[1] (or defaults to "9000"), then
//     WSAStartup() initializes Winsock.
// #2  getaddrinfo(NULL, portStr, &hints, &result) resolves "listen on any
//     local address, this port, TCP/IPv4" into a `result` addrinfo list.
// #3  socket() creates listenSock using the family/type/protocol from that
//     resolved result; setsockopt(SO_REUSEADDR) allows fast restarts;
//     bind() attaches listenSock to the resolved address; freeaddrinfo()
//     releases the now-unneeded result list; listen(SOMAXCONN) puts
//     listenSock into listening mode with the OS's max backlog.
// #4  main() enters the accept loop: accept() BLOCKS until a client
//     connects, returning a new clientSock and filling in clientAddr.
// #5  On each accepted connection, main() attempts to spawn a detached
//     std::thread intended to run client_handler(clientSock, clientAddr)
//     concurrently, so the accept loop can immediately go back to
//     accept()ing the NEXT client. NOTE: as written, this line references
//     the undeclared name `worker_thread` and will not compile (see the
//     comment placed directly above that line) - so in its current form
//     this program cannot actually build or run past this point.
// #6  (Intended behavior, once the bug above is fixed to construct the
//     thread with client_handler directly): each worker thread would print
//     the client's IP:port, then loop on recv() - printing and echoing back
//     via send() whatever bytes arrive - until the client disconnects
//     (recv() returns 0) or errors (recv() < 0), at which point it calls
//     shutdown(SD_BOTH) then closesocket() and the thread ends.
// #7  Step #4-#6 would repeat for every new client, each served by its own
//     detached worker thread, all funneling through the single listenSock.
// #8  closesocket(listenSock) and WSACleanup() after the loop only run if
//     accept() itself fails (the loop's only break); otherwise the server
//     runs until stopped externally (e.g. Ctrl+C).
// ============================================================================


