// threaded_server_win.cpp

// Compile (MSVC): cl /EHsc threaded_server_win.cpp ws2_32.lib

// MinGW: g++ threaded_server_win.cpp -lws2_32 -o threaded_server_win.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

std::mutex cout_mtx;
std::atomic<bool> running{ true };
// --- Added explanation: ts_print(s) -----------------------------------------
// Generic syntax:  ts_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so multiple worker threads
//   never interleave their output mid-line.
void ts_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::endl;
}
 
// --- Added explanation: handle_client(clientSock, clientAddr) ---------------
// Generic syntax:  handle_client( SOCKET clientSock, sockaddr_in clientAddr )
//   clientSock - parameter #1: the connected client's socket handle,
//                returned by accept() in main() below.
//   clientAddr - parameter #2: a COPY of that client's address structure,
//                used here only to log the client's IP/port.
void handle_client(SOCKET clientSock, sockaddr_in clientAddr) {
    char ip[INET_ADDRSTRLEN] = {0};
    // --- Added explanation: inet_ntop(family, addr, dst, size) ---------------
    // Generic syntax:  inet_ntop( int af, const void* src, char* dst, socklen_t size )
    //   af   - parameter #1: address family of the source address; AF_INET
    //          means IPv4.
    //   src  - parameter #2: pointer to the binary address to convert
    //          (&clientAddr.sin_addr).
    //   dst  - parameter #3: output buffer that receives the human-readable
    //          dotted-decimal string (ip).
    //   size - parameter #4: size of that output buffer (sizeof(ip)).
    //   This is the modern, IPv6-capable replacement for inet_ntoa() used in
    //   4_simple_echo_server_win.cpp.
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
    // --- Added explanation: ntohs(clientAddr.sin_port) ------------------------
    // Generic syntax:  ntohs( u_short netshort )
    //   netshort - parameter #1 (only parameter): a 16-bit port number in
    //              network byte order (clientAddr.sin_port); converted back
    //              to host byte order for printing.
    int port = ntohs(clientAddr.sin_port);
    ts_print("[Worker] Connected: " + std::string(ip) + ":" + std::to_string(port)); 
    const int BUFSZ = 1024;
    char buf[BUFSZ];
 
    while (running) {
        // --- Added explanation: recv(clientSock, buf, len, flags) --------------
        // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to read from (clientSock).
        //   buf   - parameter #2: destination buffer (buf, BUFSZ bytes).
        //   len   - parameter #3: max bytes to read (BUFSZ, 1024).
        //   flags - parameter #4: 0 means normal blocking receive.
        //   returns  >0 bytes received, 0 = client disconnected, <0 = error.
        int bytes = recv(clientSock, buf, BUFSZ, 0);
        if (bytes > 0) {
            // echo back
            int sent = 0;

            while (sent < bytes) {
                // --- Added explanation: send(clientSock, buf+off, len, flags) ------
                // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
                //   s     - parameter #1: destination socket (clientSock).
                //   buf   - parameter #2: pointer into buf, offset by what's
                //           already been sent this round (buf + sent) -
                //           resumes from where the last partial send() left
                //           off.
                //   len   - parameter #3: remaining bytes to send
                //           (bytes - sent).
                //   flags - parameter #4: 0 means normal blocking send.
                int s = send(clientSock, buf + sent, bytes - sent, 0);
                if (s == SOCKET_ERROR) {
                    ts_print("[Worker] send error: " + std::to_string(WSAGetLastError()));
                    break;

                }
                sent += s;
            }

            // print received message (safe)
            std::string msg(buf, buf + bytes);
            ts_print("[Worker] From " + std::string(ip) + ":" + std::to_string(port) + " => " + msg);

        } else if (bytes == 0) {
            ts_print("[Worker] Client disconnected: " + std::string(ip) + ":" + std::to_string(port));
            break;

        } else {
            ts_print("[Worker] recv error: " + std::to_string(WSAGetLastError()));
            break;
        }
    }
    // --- Added explanation: shutdown(clientSock, SD_BOTH) ----------------------
    // Generic syntax:  shutdown( SOCKET s, int how )
    //   s   - parameter #1: the socket to shut down (clientSock).
    //   how - parameter #2: SD_BOTH disables further sends AND receives.
    shutdown(clientSock, SD_BOTH);
    // --- Added explanation: closesocket(clientSock) -----------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release.
    closesocket(clientSock);
    ts_print("[Worker] Closed: " + std::string(ip) + ":" + std::to_string(port));
}
 
int main(int argc, char* argv[]) {
    const char* port = (argc >= 2) ? argv[1] : "9001";
    ts_print("Simple threaded echo server (Windows) - port " + std::string(port));
    ts_print("Press ENTER to stop the server.");
    WSADATA wsa;
    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       Winsock fills in.
    //   returns              0 on success, nonzero on failure.
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        ts_print("WSAStartup failed");
        return 1;
    }
 
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addrinfo* result = nullptr;
    // --- Added explanation: getaddrinfo(node, service, &hints, &result) --------
    // Generic syntax:  getaddrinfo( const char* pNodeName, const char* pServiceName,
    //                                const addrinfo* pHints, addrinfo** ppResult )
    //   pNodeName    - parameter #1: hostname/IP to resolve; nullptr (with
    //                  AI_PASSIVE in hints) means "any local address".
    //   pServiceName - parameter #2: port as a string (port, e.g. "9001").
    //   pHints       - parameter #3: kind of address wanted (&hints - IPv4,
    //                  TCP, passive/server mode).
    //   ppResult     - parameter #4: address of a pointer that receives the
    //                  resolved addrinfo list (&result).
    //   returns         0 on success, nonzero on failure.
    if (getaddrinfo(nullptr, port, &hints, &result) != 0) {
        ts_print("getaddrinfo failed");
        WSACleanup();
        return 1;
    }

    // --- Added explanation: socket(family, type, protocol) ---------------------
    // Generic syntax:  socket( int af, int type, int protocol )
    //   af       - parameter #1: address family, from the resolved result
    //              (result->ai_family, AF_INET here).
    //   type     - parameter #2: socket type, from the resolved result
    //              (result->ai_socktype, SOCK_STREAM/TCP here).
    //   protocol - parameter #3: protocol, from the resolved result
    //              (result->ai_protocol).
    SOCKET listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSock == INVALID_SOCKET) {
        ts_print("socket() failed: " + std::to_string(WSAGetLastError()));
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
 
    int opt = 1;
    // --- Added explanation: setsockopt(listenSock, level, optname, optval, optlen)
    // Generic syntax:  setsockopt( SOCKET s, int level, int optname,
    //                               const char* optval, int optlen )
    //   s       - parameter #1: the socket to configure (listenSock).
    //   level   - parameter #2: SOL_SOCKET means a generic socket-level
    //             option.
    //   optname - parameter #3: SO_REUSEADDR lets this socket rebind to a
    //             port still lingering from a previous run.
    //   optval  - parameter #4: pointer to the new value
    //             ((const char*)&opt, opt = 1 meaning "enable").
    //   optlen  - parameter #5: size in bytes of that value (sizeof(opt)).
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
 
    // --- Added explanation: bind(listenSock, ai_addr, ai_addrlen) ---------------
    // Generic syntax:  bind( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the socket to bind (listenSock).
    //   name    - parameter #2: address/port to bind to, from the resolved
    //             result (result->ai_addr).
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
    //   ai - parameter #1 (only parameter): the addrinfo list from
    //        getaddrinfo() (result); releases its memory now that bind() has
    //        read what it needed from it.
    freeaddrinfo(result);
 
    // --- Added explanation: listen(listenSock, backlog) -------------------------
    // Generic syntax:  listen( SOCKET s, int backlog )
    //   s       - parameter #1: the bound socket to listen on (listenSock).
    //   backlog - parameter #2: max pending connections; SOMAXCONN asks for
    //             the OS's maximum reasonable value.
    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        ts_print("listen() failed: " + std::to_string(WSAGetLastError()));
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
 
    // Thread that waits for ENTER to stop the server

    // --- Added explanation: thread stopper(lambda) ------------------------------
    // Generic syntax:  std::thread variableName( callableFunction )
    //   callableFunction - parameter #1 (only parameter): a lambda
    //                      `[](){ ... }` with no captures, becoming this new
    //                      thread's entire body. It runs concurrently with
    //                      the accept loop below, immediately blocking on
    //                      getline() until someone presses ENTER.
    std::thread stopper([](){
        std::string line;
        // --- Added explanation: getline(std::cin, line) --------------------------
        // Generic syntax:  getline( istream& is, string& str )
        //   is  - parameter #1: input stream to read from (std::cin).
        //   str - parameter #2: string that receives the typed line (line);
        //         its content is discarded - only the act of pressing ENTER
        //         matters here.
        std::getline(std::cin, line);
        running = false;
    });
 
    std::vector<std::thread> workers;
    workers.reserve(32);
 
    while (running) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        // --- Added explanation: accept(listenSock, &clientAddr, &addrLen) ------
        // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
        //   s       - parameter #1: the listening socket (listenSock).
        //   addr    - parameter #2: output buffer for the connecting
        //             client's address ((sockaddr*)&clientAddr).
        //   addrlen - parameter #3: in/out size of that buffer (&addrLen).
        //   returns    a NEW SOCKET (clientSock) for this connection, or
        //             INVALID_SOCKET. Blocks until a client connects OR
        //             listenSock is closed (see the shutdown flow below).
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);

        if (clientSock == INVALID_SOCKET) {
            int err = WSAGetLastError();
            if (!running) break; // normal shutdown (listen socket closed)
            ts_print("accept() failed: " + std::to_string(err));
            continue;
        }
 
        // spawn worker and detach (for simplicity). Alternatively, push and join on shutdown.
        // --- Added explanation: workers.emplace_back(handle_client, clientSock, clientAddr)
        // Generic syntax:  vectorOfThread.emplace_back( callableFunction, arg1, arg2 )
        //   callableFunction - parameter #1: the function the new thread
        //                      runs (handle_client).
        //   arg1             - parameter #2: forwarded into handle_client's
        //                      `clientSock` parameter.
        //   arg2             - parameter #3: forwarded into handle_client's
        //                      `clientAddr` parameter.
        // Constructs a NEW std::thread in-place inside `workers`, starting
        // it immediately.
        workers.emplace_back(handle_client, clientSock, clientAddr);
        // --- Added explanation: workers.back().detach() -----------------------------
        // Generic syntax:  threadObj.detach()
        //   (no parameters) - severs the just-created thread object
        //   (workers.back(), the one just emplaced) from the thread it's
        //   running; it keeps running independently and cleans itself up
        //   when handle_client returns, with no need to .join() it.
        workers.back().detach();
    }
 
    // cleanup
    // --- Added explanation: closesocket(listenSock) -----------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release
    //       (listenSock). Closing it also unblocks any accept() still
    //       waiting on it, which is why the running-flag check right after
    //       accept() (above) treats INVALID_SOCKET + !running as a normal
    //       shutdown rather than an error.
    closesocket(listenSock);
    // --- Added explanation: stopper.joinable() / stopper.join() ----------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() is true if `stopper` is
    //   still a live, un-joined thread; join() blocks main until `stopper`
    //   has finished (which happens as soon as getline() above returns,
    //   i.e. once ENTER was pressed - already true by this point, since
    //   running was set false by that same thread to break the accept loop).
    if (stopper.joinable()) stopper.join();
    ts_print("Server shutdown. Cleaning up Winsock.");
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process.
    WSACleanup();
    return 0;
}

 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() picks the port from argv[1] (or defaults to "9001"), prints
//     startup messages, then WSAStartup() initializes Winsock.
// #2  getaddrinfo()/socket()/setsockopt(SO_REUSEADDR)/bind()/freeaddrinfo()/
//     listen(SOMAXCONN) set up listenSock exactly as in
//     4_simple_echo_server_win.cpp, ending with listenSock ready to accept
//     connections.
// #3  A detached-by-join-later thread `stopper` is created, immediately
//     blocking on getline(std::cin, line) - it runs concurrently with
//     everything below, waiting for someone to press ENTER at the server's
//     console.
// #4  main() enters the accept loop, guarded by the atomic<bool> `running`
//     (still true): accept() BLOCKS until a client connects.
// #5  On each accepted connection, workers.emplace_back(handle_client,
//     clientSock, clientAddr) constructs a NEW thread in-place inside the
//     `workers` vector, immediately running handle_client() concurrently;
//     workers.back().detach() then lets it run independently (the vector
//     entry is kept only so the thread object exists somewhere - the
//     program does not join these worker threads individually).
// #6  Each handle_client() thread loops on recv() while (running) is true -
//     printing and echoing back via send() whatever bytes arrive - until
//     the client disconnects (recv() returns 0), errors (recv() < 0), or
//     `running` becomes false, then calls shutdown(SD_BOTH) and
//     closesocket() and ends.
// #7  Meanwhile, when someone at the server's console presses ENTER, the
//     `stopper` thread's getline() returns and it sets running = false.
//     This has two effects: (a) any handle_client() thread currently
//     blocked in its while(running) check will exit on its next loop
//     iteration once recv() returns; (b) main's own accept loop condition
//     while(running) will become false too.
// #8  However, main's accept() call is likely still BLOCKED waiting for a
//     new connection at that moment (setting an atomic flag doesn't
//     interrupt an in-progress blocking call) - so main only actually
//     notices `running == false` on the accept loop's NEXT iteration. If no
//     new client ever connects, main would stay blocked in accept()
//     forever; in practice, closing the socket (step #9) or a subsequent
//     connection attempt is what unblocks it (accept() then returns
//     INVALID_SOCKET, and the `if (!running) break;` check treats that as a
//     normal shutdown rather than printing an error).
// #9  Once the accept loop exits, main() calls closesocket(listenSock),
//     waits for the stopper thread with stopper.join(), prints a shutdown
//     message, calls WSACleanup(), and returns 0. Detached worker threads
//     that are still mid-conversation with a client are NOT waited for -
//     they finish on their own once their client disconnects or `running`
//     flips false.
// ============================================================================
