// threaded_client_win.cpp
// Compile (MSVC): cl /EHsc threaded_client_win.cpp ws2_32.lib
// MinGW: g++ threaded_client_win.cpp -lws2_32 -o threaded_client_win.exe
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
 
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <chrono>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;
// --- Added explanation: safe_print(s) ---------------------------------------
// Generic syntax:  safe_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so multiple concurrent
//   client threads never interleave their output mid-line.
void safe_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::flush;
}
 
// --- Added explanation: client_task(server, port, id, messages) ------------
// Generic syntax:  client_task( std::string server, std::string port, int id, int messages )
//   server   - parameter #1: hostname/IP of the server to connect to
//              (defaults to "127.0.0.2" in main() below, if not given on
//              the command line).
//   port     - parameter #2: port number, as a string, to connect to.
//   id       - parameter #3: a simple integer label (1..threads, assigned
//              by main()) used only for the printed log messages.
//   messages - parameter #4: how many ping-pong messages this client sends
//              before finishing.
void client_task(const std::string server, const std::string port, int id, int messages) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* res = nullptr;
    // --- Added explanation: getaddrinfo(server, port, &hints, &res) ------------
    // Generic syntax:  getaddrinfo( const char* pNodeName, const char* pServiceName,
    //                                const addrinfo* pHints, addrinfo** ppResult )
    //   pNodeName    - parameter #1: hostname/IP to resolve (server.c_str()).
    //   pServiceName - parameter #2: port as a string (port.c_str()).
    //   pHints       - parameter #3: kind of address wanted (&hints -
    //                  AF_UNSPEC lets the resolver return IPv4 OR IPv6
    //                  candidates, unlike earlier files that fixed AF_INET).
    //   ppResult     - parameter #4: address of a pointer that receives a
    //                  LINKED LIST of matching addrinfo candidates (&res) -
    //                  the for-loop below tries each one in turn.
    //   returns         0 on success, nonzero on failure (checked below).
    if (getaddrinfo(server.c_str(), port.c_str(), &hints, &res) != 0) {
        safe_print("[C" + std::to_string(id) + "] getaddrinfo failed\n");
        return;
    }
 
    SOCKET sock = INVALID_SOCKET;
    // This loop tries each resolved candidate address in turn (there may be
    // several, e.g. one IPv4 and one IPv6) until one successfully connects.
    for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
        // --- Added explanation: socket(family, type, protocol) -----------------
        // Generic syntax:  socket( int af, int type, int protocol )
        //   af       - parameter #1: address family of this candidate
        //              (p->ai_family).
        //   type     - parameter #2: socket type of this candidate
        //              (p->ai_socktype).
        //   protocol - parameter #3: protocol of this candidate
        //              (p->ai_protocol).
        //   returns     a new SOCKET, or INVALID_SOCKET if this particular
        //              candidate can't even be created (loop continues to
        //              the next candidate below).
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) continue;
        // --- Added explanation: connect(sock, ai_addr, ai_addrlen) -------------
        // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
        //   s       - parameter #1: the socket just created (sock).
        //   name    - parameter #2: this candidate's address (p->ai_addr).
        //   namelen - parameter #3: size of that address ((int)p->ai_addrlen).
        //   returns    0 on success (loop breaks immediately below, keeping
        //             this connected `sock`); nonzero means this candidate
        //             failed, so `sock` is closed and the loop tries the
        //             next one.
        if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == 0) break;
        // --- Added explanation: closesocket(sock) - same as explained later
        // in this file for the final closesocket(sock) call; here it just
        // discards a candidate socket that failed to connect.
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    // --- Added explanation: freeaddrinfo(res) -----------------------------------
    // Generic syntax:  freeaddrinfo( addrinfo* ai )
    //   ai - parameter #1 (only parameter): the addrinfo list from
    //        getaddrinfo() (res); releases its memory now that the loop
    //        above has finished trying every candidate.
    freeaddrinfo(res);
 
    if (sock == INVALID_SOCKET) {
        safe_print("[C" + std::to_string(id) + "] connect failed\n");
        return;
    }
    safe_print("[C" + std::to_string(id) + "] Connected to " + server + ":" + port + "\n");
 
    for (int i = 1; i <= messages; ++i) {
        std::string msg = "Client " + std::to_string(id) + " msg " + std::to_string(i) + "\n";
        // --- Added explanation: send(sock, msg.c_str(), len, flags) ------------
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to write to (sock).
        //   buf   - parameter #2: bytes to send (msg.c_str()).
        //   len   - parameter #3: how many bytes to send ((int)msg.size()).
        //   flags - parameter #4: 0 means normal blocking send.
        int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            safe_print("[C" + std::to_string(id) + "] send error: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }
 
        char buf[1024];
        // --- Added explanation: recv(sock, buf, len, flags) ---------------------
        // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to read from (sock).
        //   buf   - parameter #2: destination buffer (buf, 1024 bytes).
        //   len   - parameter #3: max bytes to read (sizeof(buf)-1, room
        //           for a '\0' terminator).
        //   flags - parameter #4: 0 means normal blocking receive.
        //   returns  >0 bytes received, 0 = server closed connection,
        //           <0 = error.
        int recvd = recv(sock, buf, sizeof(buf)-1, 0);
        if (recvd > 0) {
            buf[recvd] = '\0';
            safe_print("[C" + std::to_string(id) + "] Received: " + std::string(buf));
        } else if (recvd == 0) {
            safe_print("[C" + std::to_string(id) + "] Server closed connection\n");
            break;
        } else {
            safe_print("[C" + std::to_string(id) + "] recv error: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }
 
        // --- Added explanation: sleep_for(milliseconds(200)) --------------------
        // Generic syntax:  std::this_thread::sleep_for( std::chrono::duration )
        //   duration - parameter #1 (only parameter): how long to pause THIS
        //              thread (200 ms) before sending the next message.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
 
    // --- Added explanation: shutdown(sock, SD_BOTH) -----------------------------
    // Generic syntax:  shutdown( SOCKET s, int how )
    //   s   - parameter #1: the socket to shut down (sock).
    //   how - parameter #2: SD_BOTH disables further sends AND receives.
    shutdown(sock, SD_BOTH);
    // --- Added explanation: closesocket(sock) -----------------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release.
    closesocket(sock);
    safe_print("[C" + std::to_string(id) + "] Finished\n");
}
 
int main(int argc, char* argv[]) {
    const char* server = (argc >= 2) ? argv[1] : "127.0.0.2";
    const char* port   = (argc >= 3) ? argv[2] : "90001";
    int threads = (argc >= 4) ? std::stoi(argv[3]) : 3;
    int messages = (argc >= 5) ? std::stoi(argv[4]) : 5;
 
    WSADATA wsa;
    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       Winsock fills in. Called ONCE here in main(),
    //                       unlike 5_simple_3clients_win.cpp where each
    //                       thread called it independently.
    //   returns              0 on success, nonzero on failure.
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
 
    std::cout << "Launching " << threads << " client threads to " << server << ":" << port
<< " (" << messages << " messages each)\n";
 
    std::vector<std::thread> pool;
    for (int i = 1; i <= threads; ++i) {
        // --- Added explanation: pool.emplace_back(client_task, server, port, i, messages)
        // Generic syntax:  vectorOfThread.emplace_back( callableFunction, arg1, arg2, arg3, arg4 )
        //   callableFunction - parameter #1: the function the new thread
        //                      runs (client_task).
        //   arg1 (server)    - parameter #2: forwarded into client_task's
        //                      `server` parameter.
        //   arg2 (port)      - parameter #3: forwarded into client_task's
        //                      `port` parameter.
        //   arg3 (i)         - parameter #4: forwarded into client_task's
        //                      `id` parameter (1..threads across the loop).
        //   arg4 (messages)  - parameter #5: forwarded into client_task's
        //                      `messages` parameter.
        // Constructs a NEW std::thread in-place inside `pool`, starting it
        // immediately; kept (not detached) so main() can .join() them below.
        pool.emplace_back(client_task, server, port, i, messages);
    }
 
    // --- Added explanation: t.joinable() / t.join() -----------------------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() is true if `t` still
    //   represents a live, not-yet-joined thread; join() blocks main until
    //   `t` finishes running client_task(). Looping this over every entry in
    //   `pool` waits for all `threads` client threads to finish.
    for (auto &t : pool) if (t.joinable()) t.join();
 
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process, now that every client thread is done.
    WSACleanup();
    std::cout << "All client threads finished.\n";
    return 0;
}
 
//==================================================================================================
//threaded_client_win.exe 127.0.0.1 9000 5 10

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() reads server/port/threads/messages from argv (or defaults to
//     127.0.0.2:90001, 3 threads, 5 messages each), then WSAStartup()
//     initializes Winsock ONCE for the whole process.
// #2  A std::vector<std::thread> `pool` is built: pool.emplace_back(...) in
//     a loop constructs `threads` NEW std::thread objects in-place, each
//     immediately running client_task(server, port, i, messages)
//     concurrently (i = 1..threads).
// #3  Inside each concurrently-running client_task():
//       #3a  getaddrinfo(server, port, ...) with AF_UNSPEC resolves the
//            target into a linked list of candidate addresses (res).
//       #3b  A for-loop walks that list (`for (addrinfo* p = res; ...)`),
//            trying socket() + connect() on each candidate until one
//            connect() succeeds (breaking the loop with a live `sock`) or
//            the list is exhausted (leaving sock == INVALID_SOCKET).
//       #3c  freeaddrinfo(res) releases the resolved list either way.
//       #3d  If no candidate connected, the thread prints "connect failed"
//            and returns early (this instance's messages-loop never runs).
//       #3e  Otherwise, a for-loop runs `messages` times: send() transmits
//            a labeled message, recv() blocks for the server's reply and
//            prints it, then sleep_for(200ms) paces the next iteration.
//       #3f  After the loop (or an early break on error/disconnect),
//            shutdown()+closesocket() close the connection and "Finished"
//            is printed.
// #4  Back in main(), `for (auto &t : pool) if (t.joinable()) t.join();`
//     waits for every one of the `threads` client threads to finish (all
//     of which were started before this loop, so they run concurrently
//     even though they're joined one at a time, in vector order).
// #5  WSACleanup() releases Winsock, "All client threads finished." is
//     printed, and main() returns 0.
// #6  Net effect: this is the client-side counterpart to something like
//     6_threaded_server_win.cpp - it simulates several independent clients
//     (each doing its own address resolution with connect-to-first-working-
//     candidate fallback) hitting the same server concurrently.
// #7  The trailing `//===...` and `//threaded_client_win.exe ...` lines at
//     the very end of the file are just an inert usage-example comment, not
//     executable code.
// ============================================================================
