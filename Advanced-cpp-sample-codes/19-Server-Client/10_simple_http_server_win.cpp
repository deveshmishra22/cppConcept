// Compile (MSVC):

//   cl /EHsc simple_http_server_win.cpp ws2_32.lib

// MinGW:

//   g++ simple_http_server_win.cpp -lws2_32 -o simple_http_server_win.exe

//

// Usage:

//   simple_http_server_win.exe [port]

// Example:

//   simple_http_server_win.exe 8080
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#include <ws2tcpip.h>
 
#include <iostream>

#include <string>

#include <vector>

#include <mutex>

#include <atomic>
#include <thread>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;

// --- Added explanation: safe_print(s) ---------------------------------------
// Generic syntax:  safe_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so the main thread and the
//   stopper thread never interleave their output mid-line.
void safe_print(const std::string &s) {

    std::lock_guard<std::mutex> lk(cout_mtx);

    std::cout << s << std::endl;

}
 
std::atomic<bool> running(true);
 
// --- Added explanation: build_response() ------------------------------------
// Generic syntax:  build_response()
//   (no parameters) - builds and returns a complete, ready-to-send HTTP/1.0
//   response string: status line, headers (including a correctly computed
//   Content-Length from the body's actual size), a blank line separating
//   headers from body (the "\r\n" on its own), and the HTML body itself.
//   Called once in main() below and reused for every request, since this
//   server always replies with the exact same page.
std::string build_response() {

    const std::string body = "<html><body><h1>Simple Win HTTP Server</h1><p>Hello from server.</p></body></html>\n";

    std::string resp = "HTTP/1.0 200 OK\r\n";

    resp += "Content-Type: text/html; charset=utf-8\r\n";

    resp += "Content-Length: " + std::to_string(body.size()) + "\r\n";

    resp += "Connection: close\r\n";

    resp += "\r\n";

    resp += body;

    return resp;

}
 
int main(int argc, char* argv[]) {

    const char* port = (argc >= 2) ? argv[1] : "8080";
 
    WSADATA wsa;

    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       Winsock fills in.
    //   returns              0 on success, nonzero on failure.
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {

        std::cerr << "WSAStartup failed\n";

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
    //   pServiceName - parameter #2: port as a string (port, e.g. "8080").
    //   pHints       - parameter #3: kind of address wanted (&hints - IPv4,
    //                  TCP, passive/server mode).
    //   ppResult     - parameter #4: address of a pointer that receives the
    //                  resolved addrinfo list (&result).
    //   returns         0 on success, nonzero on failure.
    if (getaddrinfo(nullptr, port, &hints, &result) != 0) {

        std::cerr << "getaddrinfo failed\n";

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

        std::cerr << "socket() failed: " << WSAGetLastError() << "\n";

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

        std::cerr << "bind() failed: " << WSAGetLastError() << "\n";

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

        std::cerr << "listen() failed: " << WSAGetLastError() << "\n";

        closesocket(listenSock);

        WSACleanup();

        return 1;

    }
 
    safe_print("Simple HTTP server listening on port " + std::string(port));

    safe_print("Press ENTER to stop.");
 
    // Pre-build response

    const std::string response = build_response();
 
    // Thread to stop the server with ENTER

    // --- Added explanation: thread stopper(lambda) ------------------------------
    // Generic syntax:  std::thread variableName( callableFunction )
    //   callableFunction - parameter #1 (only parameter): a lambda
    //                      `[](){ ... }` becoming this new thread's entire
    //                      body, running concurrently with the accept loop
    //                      below, immediately blocking on getline().
    std::thread stopper([](){

        // --- Added explanation: getline(std::cin, s) ------------------------------
        // Generic syntax:  getline( istream& is, string& str )
        //   is  - parameter #1: input stream to read from (std::cin).
        //   str - parameter #2: string that receives the typed line (s); its
        //         content is discarded - only pressing ENTER matters here.
        std::string s; std::getline(std::cin, s);

        running = false;

        // nothing else here; main loop will check running and exit

    });
 
    while (running) {

        sockaddr_in clientAddr{};

        int addrLen = sizeof(clientAddr);

        // Use accept with a short timeout alternative is complex; simple blocking accept is OK here

        // --- Added explanation: accept(listenSock, &clientAddr, &addrLen) ------
        // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
        //   s       - parameter #1: the listening socket (listenSock).
        //   addr    - parameter #2: output buffer for the connecting
        //             client's address ((sockaddr*)&clientAddr).
        //   addrlen - parameter #3: in/out size of that buffer (&addrLen).
        //   returns    a NEW SOCKET (clientSock) for this connection, or
        //             INVALID_SOCKET. Blocks until a client connects OR
        //             listenSock is closed (which is what makes accept()
        //             return INVALID_SOCKET during shutdown, below).
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);

        if (clientSock == INVALID_SOCKET) {

            int err = WSAGetLastError();

            if (!running) break;

            safe_print("accept() failed: " + std::to_string(err));

            continue;

        }
 
        // Read request (simple): read up to CRLF CRLF or until buffer full

        std::string request;

        char buf[1024];

        int total = 0;

        // set recv timeout so slow clients won't hang server forever

        int timeout_ms = 2000;

        // --- Added explanation: setsockopt(clientSock, level, optname, optval, optlen)
        // Generic syntax:  setsockopt( SOCKET s, int level, int optname,
        //                               const char* optval, int optlen )
        //   s       - parameter #1: the socket to configure (clientSock).
        //   level   - parameter #2: SOL_SOCKET means a generic socket-level
        //             option.
        //   optname - parameter #3: SO_RCVTIMEO sets a receive timeout,
        //             unlike the earlier setsockopt(SO_REUSEADDR) call on
        //             listenSock, which controlled address reuse instead.
        //   optval  - parameter #4: pointer to the new value
        //             ((const char*)&timeout_ms, 2000 milliseconds).
        //   optlen  - parameter #5: size in bytes of that value
        //             (sizeof(timeout_ms)).
        //   After this call, any recv() on clientSock that would otherwise
        //   block forever instead gives up after 2 seconds - protecting the
        //   server from one slow/silent client hanging it indefinitely.
        setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(timeout_ms));
 
        while (true) {

            // --- Added explanation: recv(clientSock, buf, len, flags) -----------
            // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
            //   s     - parameter #1: connected socket to read from
            //           (clientSock), now with the 2-second timeout above.
            //   buf   - parameter #2: destination buffer (buf, 1024 bytes).
            //   len   - parameter #3: max bytes to read (sizeof(buf)).
            //   flags - parameter #4: 0 means normal receive.
            //   returns  >0 bytes received, 0 or <0 (including a timeout
            //           expiring) both fall into the `else` branch below,
            //           which simply breaks out of this read loop.
            int n = recv(clientSock, buf, sizeof(buf), 0);

            if (n > 0) {

                request.append(buf, buf + n);

                total += n;

                if (request.find("\r\n\r\n") != std::string::npos) break; // simple end of headers

                if (total > 16*1024) break;

            } else {

                break;

            }

        }
 
        // Optionally print a short summary

        safe_print("[Server] Got request (" + std::to_string(total) + " bytes), replying...");
 
        // Send response (single send; OK for small response)

        int sent = 0;

        const char* data = response.c_str();

        int len = (int)response.size();

        while (sent < len) {

            // --- Added explanation: send(clientSock, data+off, len, flags) -----
            // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
            //   s     - parameter #1: destination socket (clientSock).
            //   buf   - parameter #2: pointer into the pre-built response,
            //           offset by what's already been sent (data + sent).
            //   len   - parameter #3: remaining bytes to send (len - sent).
            //   flags - parameter #4: 0 means normal blocking send.
            int s = send(clientSock, data + sent, len - sent, 0);

            if (s == SOCKET_ERROR) {

                safe_print("[Server] send error: " + std::to_string(WSAGetLastError()));

                break;

            }

            sent += s;

        }
 
        // --- Added explanation: shutdown(clientSock, SD_BOTH) -------------------
        // Generic syntax:  shutdown( SOCKET s, int how )
        //   s   - parameter #1: the socket to shut down (clientSock).
        //   how - parameter #2: SD_BOTH disables further sends AND receives.
        shutdown(clientSock, SD_BOTH);

        // --- Added explanation: closesocket(clientSock) -------------------------
        // Generic syntax:  closesocket( SOCKET s )
        //   s - parameter #1 (only parameter): the socket handle to release
        //       (clientSock). This is an HTTP/1.0-style server: every
        //       request gets its own accept() -> read -> respond ->
        //       close cycle, unlike a keep-alive HTTP/1.1 connection.
        closesocket(clientSock);

    }
 
    // cleanup

    // --- Added explanation: closesocket(listenSock) - same explanation as
    // closesocket(clientSock) above; closes the listening socket once the
    // while(running) loop has exited.
    closesocket(listenSock);

    // --- Added explanation: stopper.joinable() / stopper.join() ----------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() is true if `stopper` is
    //   still a live, un-joined thread; join() blocks main until `stopper`
    //   finishes (which happens once getline() returns, i.e. once ENTER was
    //   pressed).
    if (stopper.joinable()) stopper.join();

    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process.
    WSACleanup();

    safe_print("Server stopped.");

    return 0;

}

 

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() picks the port from argv[1] (or defaults to "8080"), then
//     WSAStartup()/getaddrinfo()/socket()/setsockopt(SO_REUSEADDR)/bind()/
//     freeaddrinfo()/listen(SOMAXCONN) set up listenSock exactly as in the
//     other *_win.cpp servers in this folder.
// #2  build_response() is called ONCE to pre-compute the entire HTTP
//     response (status line + headers + body) as a single string, since
//     this server always answers every request identically.
// #3  A `stopper` thread is created, immediately blocking on getline() to
//     wait for ENTER at the console.
// #4  main() enters the while(running) accept loop:
//       #4a  accept() blocks until a client (e.g. a web browser, or
//            11_http_load_client_win.cpp) connects, returning clientSock.
//       #4b  setsockopt(clientSock, SO_RCVTIMEO, 2000) gives THIS
//            connection's reads a 2-second timeout, so one slow client
//            can't hang the whole server.
//       #4c  An inner while(true) loop calls recv() repeatedly, appending
//            bytes to `request`, until it either sees the "\r\n\r\n" that
//            marks the end of HTTP headers, accumulates more than 16 KB
//            (safety cap), or recv() returns <=0 (client gone, error, or
//            the 2-second timeout expired) - any of which breaks the loop.
//       #4d  Regardless of what was actually in the request (this server
//            does not parse the method, path, or headers - it always
//            replies the same way), a while-loop calls send() repeatedly
//            (handling partial sends) to transmit the entire pre-built
//            `response` string back to the client.
//       #4e  shutdown()+closesocket() close this one connection - this is
//            HTTP/1.0-style: no keep-alive, a fresh TCP connection per
//            request.
// #5  This loop repeats for every new connection until someone presses
//     ENTER; the stopper thread then sets running = false, which the
//     while(running) check picks up on its next pass (once the current
//     accept() unblocks, e.g. from a new connection attempt or
//     listenSock being closed).
// #6  After the loop, closesocket(listenSock), stopper.join(), and
//     WSACleanup() perform final cleanup, "Server stopped." is printed,
//     and main() returns 0.
// #7  In short: point a web browser (or 11_http_load_client_win.cpp) at
//     http://<this machine>:<port>/ and it will always receive the same
//     small HTML page defined in build_response().
// ============================================================================
