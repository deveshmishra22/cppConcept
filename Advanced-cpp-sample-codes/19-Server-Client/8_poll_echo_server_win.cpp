// Compile (MSVC):

//   cl /EHsc poll_echo_server_win.cpp ws2_32.lib
// MinGW:
//   g++ -std=c++11 poll_echo_server_win.cpp -lws2_32 -o poll_echo_server_win.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;

// --- Added explanation: safe_print(s) ---------------------------------------
// Generic syntax:  safe_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so the main thread and the
//   stopper thread never interleave their output mid-line.
void safe_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::flush;
}
 
std::atomic<bool> running(true); // what it means: atomic boolean flag to control the server running state across multiple threads. Analogy: like a shared on/off switch that multiple threads can check and update safely without causing conflicts.
int main(int argc, char* argv[]) {

    const char* port = (argc >= 2) ? argv[1] : "9000";
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
 
    // Create listening socket
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
    //   pServiceName - parameter #2: port as a string (port, e.g. "9000").
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
 
    // reuse address
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
 
    safe_print("poll-echo-server listening on port " + std::string(port) + "\n");
    safe_print("Press ENTER to stop the server.\n");
 
    // Poll fds container
    // WSAPOLLFD is a small struct with fields `fd` (the socket handle),
    // `events` (bitmask of what to watch for, e.g. POLLIN = "readable"),
    // and `revents` (bitmask the OS fills in with what actually happened).
    std::vector<WSAPOLLFD> pollfds;
    WSAPOLLFD listenfd{};
    listenfd.fd = listenSock;
    listenfd.events = POLLIN; // wait for incoming connections
    // --- Added explanation: pollfds.push_back(listenfd) -------------------------
    // Generic syntax:  vectorOfWSAPOLLFD.push_back( WSAPOLLFD value )
    //   value - parameter #1 (only parameter): a COPY of `listenfd` is
    //           appended to the end of `pollfds`, becoming its very first
    //           element (index 0) - the "Check listening socket first
    //           (index 0)" code further below relies on this.
    pollfds.push_back(listenfd);
 
    // stopper thread: press Enter to stop
    // --- Added explanation: thread stopper(lambda) ------------------------------
    // Generic syntax:  std::thread variableName( callableFunction )
    //   callableFunction - parameter #1 (only parameter): a lambda
    //                      `[](){ ... }` becoming this new thread's entire
    //                      body, running concurrently with the poll loop
    //                      below, immediately blocking on getline().
    std::thread stopper([](){
        std::string s;
        // --- Added explanation: getline(std::cin, s) ------------------------------
        // Generic syntax:  getline( istream& is, string& str )
        //   is  - parameter #1: input stream to read from (std::cin).
        //   str - parameter #2: string that receives the typed line (s); its
        //         content is discarded - only pressing ENTER matters here.
        std::getline(std::cin, s);
        running = false;
    });
 
    const int POLL_TIMEOUT_MS = 1000; // 1 second
    while (running) {
        int nfds = (int)pollfds.size();
        // --- Added explanation: WSAPoll(pollfds.data(), nfds, timeout) ---------
        // Generic syntax:  WSAPoll( WSAPOLLFD* fdArray, ULONG fds, INT timeout )
        //   fdArray - parameter #1: pointer to the array of sockets to watch
        //             (pollfds.data(), the raw array backing the vector).
        //   fds     - parameter #2: how many entries are in that array
        //             (nfds).
        //   timeout - parameter #3: max milliseconds to wait for ANY of them
        //             to become ready (POLL_TIMEOUT_MS, 1000 = 1 second)
        //             before returning anyway with rc == 0.
        //   returns    number of sockets with a nonzero revents (ready for
        //             I/O or an error/hangup), 0 on timeout, or
        //             SOCKET_ERROR. This single call replaces having a
        //             separate blocking recv()/accept() thread per client -
        //             it lets ONE thread monitor MANY sockets at once.
        int rc = WSAPoll(pollfds.data(), nfds, POLL_TIMEOUT_MS);
        if (rc == SOCKET_ERROR) {
            safe_print("WSAPoll failed: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }

        if (rc == 0) {
            // timeout, loop again
            continue;
        }
 
        // Check listening socket first (index 0)

        if (pollfds.size() > 0 && (pollfds[0].revents & POLLIN)) {
            sockaddr_in clientAddr;
            int addrlen = sizeof(clientAddr);
            // --- Added explanation: accept(listenSock, &clientAddr, &addrlen)
            // Generic syntax:  accept( SOCKET s, sockaddr* addr, int* addrlen )
            //   s       - parameter #1: the listening socket (listenSock).
            //   addr    - parameter #2: output buffer for the connecting
            //             client's address ((sockaddr*)&clientAddr).
            //   addrlen - parameter #3: in/out size of that buffer
            //             (&addrlen).
            //   returns    a NEW SOCKET (clientSock) for this connection, or
            //             INVALID_SOCKET. Since WSAPoll already confirmed
            //             listenSock is readable (POLLIN), this call
            //             returns immediately rather than blocking.
            SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrlen);
            if (clientSock != INVALID_SOCKET) {
                // Add to poll set (monitor for read)
                WSAPOLLFD pfd{};
                pfd.fd = clientSock;
                pfd.events = POLLIN;
                pfd.revents = 0;
                // --- Added explanation: pollfds.push_back(pfd) - same as the
                // pollfds.push_back(listenfd) call explained earlier; appends
                // this new client's WSAPOLLFD so the next WSAPoll() call
                // also watches it for incoming data.
                pollfds.push_back(pfd);
                char ip[INET_ADDRSTRLEN];
                // --- Added explanation: inet_ntop(family, addr, dst, size) -----
                // Generic syntax:  inet_ntop( int af, const void* src, char* dst, socklen_t size )
                //   af   - parameter #1: address family; AF_INET means IPv4.
                //   src  - parameter #2: binary address to convert
                //          (&clientAddr.sin_addr).
                //   dst  - parameter #3: output buffer (ip).
                //   size - parameter #4: size of that buffer (sizeof(ip)).
                inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
                // --- Added explanation: ntohs(clientAddr.sin_port) -----------------
                // Generic syntax:  ntohs( u_short netshort )
                //   netshort - parameter #1 (only parameter): port number in
                //              network byte order (clientAddr.sin_port);
                //              converted to host byte order for printing.
                safe_print("[Server] New client: " + std::string(ip) + ":" + std::to_string(ntohs(clientAddr.sin_port)) + "\n");
            } else {
                safe_print("[Server] accept failed: " + std::to_string(WSAGetLastError()) + "\n");
            }
        }
 
        // Handle client sockets (start from 1)
        for (int i = 1; i < (int)pollfds.size(); ++i) {
            WSAPOLLFD &p = pollfds[i];
            if (p.revents & POLLIN) {
                char buf[1024];
                // --- Added explanation: recv(p.fd, buf, len, flags) ---------------
                // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
                //   s     - parameter #1: this client socket (p.fd), already
                //           confirmed readable by WSAPoll (POLLIN), so this
                //           call returns immediately rather than blocking.
                //   buf   - parameter #2: destination buffer (buf, 1024
                //           bytes).
                //   len   - parameter #3: max bytes to read (sizeof(buf)).
                //   flags - parameter #4: 0 means normal receive.
                //   returns  >0 bytes received, 0 = client disconnected,
                //           <0 = error.
                int n = recv(p.fd, buf, sizeof(buf), 0);
                if (n > 0) {
                    // Echo back
                    int sent = 0;
                    while (sent < n) {
                        // --- Added explanation: send(p.fd, buf+off, len, flags) -------
                        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
                        //   s     - parameter #1: destination socket (p.fd).
                        //   buf   - parameter #2: pointer into buf, offset by
                        //           what's already been sent (buf + sent).
                        //   len   - parameter #3: remaining bytes to send
                        //           (n - sent).
                        //   flags - parameter #4: 0 means normal send.
                        int s = send(p.fd, buf + sent, n - sent, 0);
                        if (s == SOCKET_ERROR) {
                            safe_print("[Server] send error: " + std::to_string(WSAGetLastError()) + "\n");
                            break;
                        }
                        sent += s;
                    }

                    // Print received (safe substring)
                    std::string msg(buf, buf + n);
                    safe_print("[Server] Received from fd " + std::to_string((int)p.fd) + ": " + msg + "\n");

                } else if (n == 0) {
                    // connection closed by client
                    safe_print("[Server] Client closed (fd " + std::to_string((int)p.fd) + ")\n");
                    // --- Added explanation: closesocket(p.fd) ---------------------------
                    // Generic syntax:  closesocket( SOCKET s )
                    //   s - parameter #1 (only parameter): the socket handle
                    //       to release (p.fd, this disconnected client).
                    closesocket(p.fd);

                    // remove this fd from vector
                    // --- Added explanation: pollfds.erase(pollfds.begin() + i) ---------
                    // Generic syntax:  vectorOfWSAPOLLFD.erase( iterator position )
                    //   position - parameter #1 (only parameter): an
                    //              iterator pointing at the element to
                    //              remove (pollfds.begin() + i, i.e. index
                    //              i). Every element after it shifts down
                    //              one slot to fill the gap, which is why
                    //              --i (below) re-checks the SAME index next
                    //              loop iteration - it now holds what used
                    //              to be at i+1.
                    pollfds.erase(pollfds.begin() + i);
                    --i; // adjust index after erase

                } else {
                    int err = WSAGetLastError();
                    safe_print("[Server] recv error: " + std::to_string(err) + "\n");
                    // --- Added explanation: closesocket(p.fd) / pollfds.erase(...) -
                    // same as explained just above for the "client closed"
                    // branch; closes and removes this client on a recv error
                    // too.
                    closesocket(p.fd);
                    pollfds.erase(pollfds.begin() + i);
                    --i;
                }

            } else if (p.revents & (POLLERR | POLLHUP | POLLNVAL)) {

                // error/hangup: close and remove
                safe_print("[Server] Socket event error/hangup on fd " + std::to_string((int)p.fd) + "\n");
                // --- Added explanation: closesocket(p.fd) / pollfds.erase(...) -
                // same pattern again: this branch fires when WSAPoll reports
                // POLLERR/POLLHUP/POLLNVAL (error, hangup, or invalid
                // handle) instead of POLLIN, so the socket is closed and
                // removed here too.
                closesocket(p.fd);
                pollfds.erase(pollfds.begin() + i);
                --i;

            }

            // Reset revents for next poll iteration

            p.revents = 0;

        }

    }
 
    safe_print("Server shutting down...\n");
 
    // Cleanup sockets

    for (auto &p : pollfds) {

        // --- Added explanation: closesocket(p.fd) - same as explained
        // earlier for closesocket(p.fd) inside the poll loop; here it closes
        // whatever sockets (listening + any still-connected clients) remain
        // in `pollfds` once the main while(running) loop has exited.
        if (p.fd != INVALID_SOCKET) closesocket(p.fd);

    }

    pollfds.clear();
    // --- Added explanation: closesocket(listenSock) - same explanation;
    // closes the listening socket a second time defensively (it may already
    // be included in the loop above via pollfds[0], since listenfd was
    // pushed into pollfds earlier).
    closesocket(listenSock);
    // --- Added explanation: stopper.joinable() / stopper.join() ----------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() is true if `stopper` is
    //   still a live, un-joined thread; join() blocks main until `stopper`
    //   finishes (which happens once getline() returns, i.e. once ENTER was
    //   pressed - the very thing that set running = false to get here).
    if (stopper.joinable()) stopper.join();
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process.
    WSACleanup();
    return 0;

}

 // Explanation in detail about the code:
 // This code implements a simple echo server using Winsock on Windows. It listens for incoming TCP
    // connections on a specified port (default 9000) and echoes back any data received from clients.
    // The server uses the WSAPoll function to monitor multiple sockets for incoming data or new connections.
    // Key components of the code:
    // 1. Winsock Initialization: The code initializes the Winsock library using WSAStartup.
    // 2. Listening Socket: It creates a listening socket, binds it to the specified port, and starts listening for incoming connections.
    // 3. Polling Mechanism: The server maintains a vector of WSAPOLL
    // FD structures to monitor the listening socket and connected client sockets.
    // 4. Accepting Connections: When a new connection is detected on the listening socket
    //    (indicated by POLLIN event), the server accepts the connection and adds the new client socket to the poll set.
    // 5. Echoing Data: For each client socket, if data is available to read (POLLIN event), the server reads the data,
    //    echoes it back to the client, and prints the received message to the console.
    // 6. Handling Disconnections and Errors: If a client disconnects or an error occurs on a socket,
    //    the server closes the socket and removes it from the poll set.
    // 7. Graceful Shutdown: A separate thread waits for the user to press ENTER to signal the server to stop.
    //    Upon shutdown, the server cleans up all sockets and Winsock resources.


// Explaining each function used in the code and parameters used with their purpose:
// 1. WSAStartup(MAKEWORD(2,2), &wsa): This function initializes the Winsock library and must be called before any other Winsock functions. The MAKEWORD(2,2) parameter specifies the version of Winsock to use (2.2 in this case), and &wsa is a pointer to a WSADATA structure that will receive information about the Winsock implementation.
// 2. getaddrinfo(nullptr, port, &hints, &result): This function resolves the server address and port for binding. The first parameter is nullptr to indicate that the server should bind to all available interfaces. The second parameter is the port number as a string. The third parameter is a pointer to an addrinfo structure that specifies criteria for selecting the socket address structures returned. The fourth parameter is a pointer to a linked list of addrinfo structures that will hold the results.
// 3. socket(result->ai_family, result->ai_socktype, result->ai_protocol): This function creates a new socket. The parameters specify the address family (AF_INET for IPv4), the socket type (SOCK_STREAM for TCP), and the protocol (usually 0 to select the default protocol for the given socket type).
// 4. setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)): This function sets options on the socket. In this case, it sets the SO_REUSEADDR option to allow the socket to bind to an address that is already in use. This is useful for quickly restarting the server without waiting for the OS to release the port.
// 5. bind(listenSock, result->ai_addr, (int)result->ai_addrlen): This function binds the socket to the specified address and port. The first parameter is the socket to bind, the second parameter is a pointer to the sockaddr structure containing the address to bind to, and the third parameter is the length of that structure.
// 6. listen(listenSock, SOMAXCONN): This function marks the socket as a listening socket that will accept incoming connection requests. The second parameter specifies the maximum length of the queue of pending connections (SOMAXCONN is a constant that indicates the maximum allowed by the system).
// 7. WSAPoll(pollfds.data(), nfds, POLL_TIMEOUT_MS): This function monitors multiple sockets for events. The first parameter is a pointer to an array of WSAPOLLFD structures that specify the sockets to monitor. The second parameter is the number of sockets in the array. The third parameter is the timeout in milliseconds (POLL_TIMEOUT_MS) to wait for an event before returning.
// 8. accept(listenSock, (sockaddr*)&clientAddr, &addrlen): This function accepts an incoming connection on the listening socket. The first parameter is the listening socket, the second parameter is a pointer to a sockaddr structure that will receive the address of the connecting client, and the third parameter is a pointer to an integer that specifies the size of that structure.
// 9. recv(p.fd, buf, sizeof(buf), 0): This function receives data from a connected socket. The first parameter is the socket to read from, the second parameter is a buffer to store the received data, the third parameter is the size of the buffer, and the fourth parameter specifies flags (0 means no special options).
// 10. send(p.fd, buf + sent, n - sent, 0): This function sends data to a connected socket. The first parameter is the socket to send data to, the second parameter is a pointer to the data to send, the third parameter is the number of bytes to send, and the fourth parameter specifies flags (0 means no special options).
// 11. closesocket(p.fd): This function closes a socket. The parameter is the socket to close.
// 12. WSACleanup(): This function terminates the use of the Winsock library. It should be called when the application is done using Winsock functions. 

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() reads the port from argv (or defaults to "9000"), then
//     WSAStartup()/getaddrinfo()/socket()/setsockopt(SO_REUSEADDR)/bind()/
//     freeaddrinfo()/listen(SOMAXCONN) set up listenSock exactly as in the
//     other *_win.cpp servers in this folder.
// #2  A std::vector<WSAPOLLFD> `pollfds` is created and listenSock is
//     pushed into it as element [0] with events = POLLIN ("tell me when
//     this is readable", which for a listening socket means "a new
//     connection is waiting").
// #3  A `stopper` thread is created, immediately blocking on
//     getline(std::cin, s) - it runs concurrently, waiting for ENTER.
// #4  main() enters the while(running) poll loop:
//       #4a  WSAPoll(pollfds.data(), nfds, 1000) blocks (up to 1 second)
//            until at least one socket in `pollfds` has something to report,
//            or the timeout elapses (rc == 0, loop just continues).
//       #4b  If pollfds[0] (listenSock) is readable (POLLIN), accept() pulls
//            the new connection off it, and its new SOCKET is wrapped in a
//            WSAPOLLFD and pushed into `pollfds` - so the NEXT WSAPoll()
//            call will also watch this new client.
//       #4c  For every OTHER entry in `pollfds` (index 1+, the connected
//            clients): if POLLIN is set, recv() reads what's available and
//            send() echoes it straight back (looping to handle partial
//            sends), and the message is logged; if recv() returns 0 or an
//            error, or if POLLERR/POLLHUP/POLLNVAL fired instead, the
//            client's socket is closed and its entry is erase()'d from
//            `pollfds` (with `--i` to not skip the element that slides into
//            its place).
//       #4d  Each entry's `revents` is reset to 0 so the next WSAPoll() call
//            starts fresh for it.
// #5  This loop repeats - ONE thread handling potentially MANY simultaneous
//     client connections without spawning a thread per client (contrast
//     with 6_threaded_server_win.cpp's one-thread-per-client design) - until
//     someone presses ENTER at the console.
// #6  When ENTER is pressed, the `stopper` thread's getline() returns and
//     sets running = false; the poll loop's while(running) check ends the
//     loop on its next pass (WSAPoll's 1-second timeout ensures this
//     happens within about a second even with no socket activity).
// #7  After the loop, every remaining socket in `pollfds` (any clients still
//     connected) is closed, `pollfds` is cleared, listenSock is closed,
//     stopper.join() waits for the stopper thread to finish, WSACleanup()
//     releases Winsock, and main() returns 0.
// #8  Everything from the "Explanation in detail about the code" comment
//     onward (further below) is pre-existing prose documentation already in
//     this file, not something added as part of this pass.
// ============================================================================
