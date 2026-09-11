// Compile (MSVC):
//   cl /EHsc poll_client_win.cpp ws2_32.lib
// MinGW:
//   g++ poll_client_win.cpp -lws2_32 -o poll_client_win.exe
//
// Usage:
//   poll_client_win.exe <server_ip> <port> <num_threads> <messages_per_thread>
//
// Example:
//   poll_client_win.exe 127.0.0.1 9000 4 5
//   -> spawns 4 threads, each sends 5 messages
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;
// --- Added explanation: safe_print(s) ---------------------------------------
// Generic syntax:  safe_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so multiple concurrent
//   client-worker threads never interleave their output mid-line.
void safe_print(const std::string &s) {
    std::lock_guard<std::mutex> lk(cout_mtx);
    std::cout << s << std::flush;
}
 
// --- Added explanation: client_worker(host, port, id, messages) ------------
// Generic syntax:  client_worker( std::string host, std::string port, int id, int messages )
//   host     - parameter #1: hostname/IP of the server to connect to.
//   port     - parameter #2: port number, as a string, to connect to.
//   id       - parameter #3: a simple integer label (1..threads, assigned
//              by main() below) used only for the printed log messages.
//   messages - parameter #4: how many ping-pong messages this client sends
//              before finishing.
void client_worker(const std::string host, const std::string port, int id, int messages) {

    // Resolve
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* res = nullptr;
    // --- Added explanation: getaddrinfo(host, port, &hints, &res) --------------
    // Generic syntax:  getaddrinfo( const char* pNodeName, const char* pServiceName,
    //                                const addrinfo* pHints, addrinfo** ppResult )
    //   pNodeName    - parameter #1: hostname/IP to resolve (host.c_str()).
    //   pServiceName - parameter #2: port as a string (port.c_str()).
    //   pHints       - parameter #3: kind of address wanted (&hints -
    //                  AF_UNSPEC lets the resolver return IPv4 OR IPv6
    //                  candidates).
    //   ppResult     - parameter #4: address of a pointer that receives a
    //                  LINKED LIST of matching addrinfo candidates (&res).
    //   returns         0 on success, nonzero on failure.
    int rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (rc != 0 || !res) {
        safe_print("[C" + std::to_string(id) + "] getaddrinfo failed\n");
        if (res) freeaddrinfo(res);
        return;
    }
 
    SOCKET sock = INVALID_SOCKET;
    // This loop tries each resolved candidate address in turn until one
    // successfully connects.
    for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
        // --- Added explanation: socket(family, type, protocol) -----------------
        // Generic syntax:  socket( int af, int type, int protocol )
        //   af       - parameter #1: address family of this candidate
        //              (p->ai_family).
        //   type     - parameter #2: socket type of this candidate
        //              (p->ai_socktype).
        //   protocol - parameter #3: protocol of this candidate
        //              (p->ai_protocol).
        //   returns     a new SOCKET, or INVALID_SOCKET if this candidate
        //              can't be created (loop continues to the next one).
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == INVALID_SOCKET) continue;
        // --- Added explanation: connect(sock, ai_addr, ai_addrlen) -------------
        // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
        //   s       - parameter #1: the socket just created (sock).
        //   name    - parameter #2: this candidate's address (p->ai_addr).
        //   namelen - parameter #3: size of that address ((int)p->ai_addrlen).
        //   returns    0 on success (loop breaks, keeping this connected
        //             `sock`); nonzero means this candidate failed, so
        //             `sock` is closed and the loop tries the next one.
        if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == 0) break;
        // --- Added explanation: closesocket(sock) - discards a candidate
        // socket that failed to connect; same explanation as the final
        // closesocket(sock) call later in this file.
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
 
    safe_print("[C" + std::to_string(id) + "] Connected\n");
    for (int i = 1; i <= messages; ++i) {
        std::string msg = "Client " + std::to_string(id) + " message " + std::to_string(i) + "\n";
        // --- Added explanation: send(sock, msg.c_str(), len, flags) ------------
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to write to (sock).
        //   buf   - parameter #2: bytes to send (msg.c_str()).
        //   len   - parameter #3: how many bytes to send ((int)msg.size()).
        //   flags - parameter #4: 0 means normal blocking send.
        int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            safe_print("[C" + std::to_string(id) + "] send failed: " + std::to_string(WSAGetLastError()) + "\n");
            break;
        }
 
        // Use WSAPoll to wait for server response (timeout 2s)
        WSAPOLLFD pfd{};
        pfd.fd = sock;
        pfd.events = POLLIN;
        // --- Added explanation: WSAPoll(&pfd, 1, 2000) --------------------------
        // Generic syntax:  WSAPoll( WSAPOLLFD* fdArray, ULONG fds, INT timeout )
        //   fdArray - parameter #1: pointer to the (single-element) array of
        //             sockets to watch (&pfd).
        //   fds     - parameter #2: how many entries are in that array (1).
        //   timeout - parameter #3: max milliseconds to wait for `sock` to
        //             become readable (2000 = 2 seconds) before giving up
        //             and returning 0.
        //   returns    1 if pfd became ready before the timeout (checked via
        //             pfd.revents below), 0 on timeout, or SOCKET_ERROR.
        //   Unlike a plain blocking recv(), this lets the client give up on
        //   a message and move on ("no reply (timeout or error)") instead
        //   of waiting forever for a server that never answers.
        int r = WSAPoll(&pfd, 1, 2000); // 2000 ms
        if (r > 0 && (pfd.revents & POLLIN)) {
            char buf[1024];
            // --- Added explanation: recv(sock, buf, len, flags) -------------------
            // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
            //   s     - parameter #1: connected socket to read from (sock),
            //           already confirmed readable by WSAPoll above.
            //   buf   - parameter #2: destination buffer (buf, 1024 bytes).
            //   len   - parameter #3: max bytes to read (sizeof(buf)-1, room
            //           for a '\0' terminator).
            //   flags - parameter #4: 0 means normal receive.
            //   returns  >0 bytes received, 0 = server closed connection,
            //           <0 = error.
            int n = recv(sock, buf, sizeof(buf)-1, 0);
            if (n > 0) {
                buf[n] = '\0';
                safe_print("[C" + std::to_string(id) + "] got reply: " + std::string(buf));
            } else if (n == 0) {
                safe_print("[C" + std::to_string(id) + "] server closed connection\n");
                break;
            } else {
                safe_print("[C" + std::to_string(id) + "] recv error\n");
                break;
            }
        } else {
            safe_print("[C" + std::to_string(id) + "] no reply (timeout or error)\n");
        }
 
        // small pause
        // --- Added explanation: sleep_for(milliseconds(100)) --------------------
        // Generic syntax:  std::this_thread::sleep_for( std::chrono::duration )
        //   duration - parameter #1 (only parameter): how long to pause THIS
        //              thread (100 ms) before sending the next message.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    safe_print("[C" + std::to_string(id) + "] finished\n");
}
 
int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " <server_ip> <port> <num_threads> <messages_per_thread>\n";
        return 1;
    }
 
    std::string host = argv[1];
    std::string port = argv[2];
    int threads = atoi(argv[3]);
    int msgs = atoi(argv[4]);
    if (threads <= 0) threads = 1;
    if (msgs <= 0) msgs = 1;

    WSADATA wsa;
    // --- Added explanation: WSAStartup(versionRequested, &wsa) -----------------
    // Generic syntax:  WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
    //   wVersionRequested - parameter #1: Winsock version requested, via
    //                       MAKEWORD(2, 2) (version 2.2).
    //   lpWSAData         - parameter #2: address of a WSADATA struct (&wsa)
    //                       Winsock fills in. Called ONCE here in main().
    //   returns              0 on success, nonzero on failure.
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    } 
    std::vector<std::thread> workers;
    for (int i = 1; i <= threads; ++i) {
        // --- Added explanation: workers.emplace_back(client_worker, host, port, i, msgs)
        // Generic syntax:  vectorOfThread.emplace_back( callableFunction, arg1, arg2, arg3, arg4 )
        //   callableFunction - parameter #1: the function the new thread
        //                      runs (client_worker).
        //   arg1 (host)      - parameter #2: forwarded into client_worker's
        //                      `host` parameter.
        //   arg2 (port)      - parameter #3: forwarded into client_worker's
        //                      `port` parameter.
        //   arg3 (i)         - parameter #4: forwarded into client_worker's
        //                      `id` parameter (1..threads across the loop).
        //   arg4 (msgs)      - parameter #5: forwarded into client_worker's
        //                      `messages` parameter.
        // Constructs a NEW std::thread in-place inside `workers`, starting
        // it immediately; kept (not detached) so main() can .join() them.
        workers.emplace_back(client_worker, host, port, i, msgs);
    }
 
    // --- Added explanation: t.joinable() / t.join() -----------------------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() is true if `t` still
    //   represents a live, not-yet-joined thread; join() blocks main until
    //   `t` finishes running client_worker(). Looping this over every entry
    //   in `workers` waits for all `threads` client threads to finish.
    for (auto &t : workers) if (t.joinable()) t.join();
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process, now that every client thread is done.
    WSACleanup();
    return 0;

}
 
// =============================================================================================

// poll_echo_server_win.exe 9000
 
// poll_client_win.exe 127.0.0.1 9000 4 5
 
// 4 threads, 5 messages each.

// ---------------------------------------------------------------------------------------------

 

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() requires 4 command-line arguments (server_ip, port,
//     num_threads, messages_per_thread); if fewer are given it prints usage
//     and exits. Otherwise threads/msgs are parsed (with a floor of 1 each)
//     and WSAStartup() initializes Winsock ONCE for the whole process.
// #2  A std::vector<std::thread> `workers` is built: workers.emplace_back()
//     in a loop constructs `threads` NEW std::thread objects, each
//     immediately running client_worker(host, port, i, msgs) concurrently.
// #3  Inside each concurrently-running client_worker():
//       #3a  getaddrinfo(host, port, ...) with AF_UNSPEC resolves the
//            target into a linked list of candidate addresses.
//       #3b  A for-loop tries socket()+connect() on each candidate until
//            one connects (or the list is exhausted, leaving
//            sock == INVALID_SOCKET and the thread returning early).
//       #3c  A for-loop runs `messages` times: send() transmits a labeled
//            message, then - instead of a plain blocking recv() like
//            7_threaded_client_win.cpp - WSAPoll(&pfd, 1, 2000) waits UP TO
//            2 seconds for `sock` to become readable.
//       #3d  If WSAPoll reports the socket ready, recv() reads and prints
//            the reply; if it times out or errors, "no reply (timeout or
//            error)" is printed and the loop just moves on to the next
//            message (it does NOT break/abort on a timeout, unlike a
//            recv-error or server-disconnect, which DO break the loop).
//       #3e  sleep_for(100ms) paces each iteration.
//       #3f  After the loop, shutdown()+closesocket() close the connection
//            and "finished" is printed.
// #4  Back in main(), `for (auto &t : workers) if (t.joinable()) t.join();`
//     waits for every one of the `threads` client threads to finish.
// #5  WSACleanup() releases Winsock and main() returns 0.
// #6  Net effect: this is a client-side counterpart designed to pair with
//     8_poll_echo_server_win.cpp - it simulates several independent
//     clients, each tolerating slow/missing replies via a timeout instead
//     of blocking forever, useful for testing a poll-based server's
//     behavior under multiple concurrent, occasionally-idle connections.
// #7  The trailing `//===...`, usage-example, and `//---...` lines at the
//     very end of the file are inert comments, not executable code.
// ============================================================================
