// Compile (MSVC):

//   cl /EHsc http_load_client_win.cpp ws2_32.lib

// MinGW:

//   g++ http_load_client_win.cpp -lws2_32 -o http_load_client_win.exe

//

// Usage:

//   http_load_client_win.exe <host> <port> <path> <num_threads> <requests_per_thread>

//

// Example:

//   http_load_client_win.exe 127.0.0.1 8080 / 10 5

//   -> 10 threads, 5 requests per thread to http://127.0.0.1:8080/
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#include <ws2tcpip.h>
 
#include <iostream>

#include <thread>

#include <vector>

#include <mutex>

#include <chrono>

#include <sstream>

#include <atomic>
 
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex cout_mtx;

std::atomic<int> succ{0}, failc{0};
 
// --- Added explanation: safe_print(s) ---------------------------------------
// Generic syntax:  safe_print( const std::string& s )
//   s - parameter #1 (only parameter): the message to print to std::cout.
//   Wraps the print in a lock_guard on cout_mtx so multiple concurrent
//   worker threads never interleave their output mid-line.
void safe_print(const std::string &s) {

    std::lock_guard<std::mutex> lk(cout_mtx);

    std::cout << s << std::flush;

}
 
// --- Added explanation: make_request(host, path) ----------------------------
// Generic syntax:  make_request( const std::string& host, const std::string& path )
//   host - parameter #1: the "Host:" header value to send (e.g. "127.0.0.1").
//   path - parameter #2: the request path/URI to GET (e.g. "/").
//   returns  a complete HTTP/1.0 GET request as one string, ready to send()
//   as-is - request line, Host/Connection/User-Agent headers, and the
//   blank line ("\r\n") that terminates HTTP headers.
std::string make_request(const std::string &host, const std::string &path) {

    std::ostringstream oss;

    oss << "GET " << path << " HTTP/1.0\r\n"
<< "Host: " << host << "\r\n"
<< "Connection: close\r\n"
<< "User-Agent: http-load-client/1.0\r\n"
<< "\r\n";

    return oss.str();

}
 
// --- Added explanation: worker(host, port, path, id, reqsPerThread) --------
// Generic syntax:  worker( std::string host, std::string port, std::string path, int id, int reqsPerThread )
//   host          - parameter #1: server hostname/IP to hit.
//   port          - parameter #2: server port, as a string.
//   path          - parameter #3: URL path to request (e.g. "/").
//   id            - parameter #4: a simple integer label (1..threads,
//                   assigned by main() below) used only in log messages.
//   reqsPerThread - parameter #5: how many separate HTTP requests this
//                   thread sends, one full connect/request/response cycle
//                   at a time, before finishing.
void worker(const std::string host, const std::string port, const std::string path, int id, int reqsPerThread) {

    for (int r = 1; r <= reqsPerThread; ++r) {

        addrinfo hints{}; hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;

        addrinfo *res = nullptr;

        // --- Added explanation: getaddrinfo(host, port, &hints, &res) ------------
        // Generic syntax:  getaddrinfo( const char* pNodeName, const char* pServiceName,
        //                                const addrinfo* pHints, addrinfo** ppResult )
        //   pNodeName    - parameter #1: hostname/IP to resolve (host.c_str()).
        //   pServiceName - parameter #2: port as a string (port.c_str()).
        //   pHints       - parameter #3: kind of address wanted (&hints -
        //                  AF_UNSPEC lets the resolver return IPv4 OR IPv6).
        //   ppResult     - parameter #4: address of a pointer that receives a
        //                  LINKED LIST of matching addrinfo candidates
        //                  (&res). Called fresh on EVERY request (each loop
        //                  iteration), unlike the single-connection client
        //                  patterns elsewhere in this folder.
        //   returns         0 on success, nonzero on failure.
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0 || !res) {

            safe_print("[T" + std::to_string(id) + "] getaddrinfo failed\n");

            failc++;

            if (res) freeaddrinfo(res);

            continue;

        }
 
        SOCKET sock = INVALID_SOCKET;

        // This loop tries each resolved candidate address in turn until one
        // successfully connects.
        for (addrinfo* p = res; p != nullptr; p = p->ai_next) {

            // --- Added explanation: socket(family, type, protocol) ---------------
            // Generic syntax:  socket( int af, int type, int protocol )
            //   af       - parameter #1: address family of this candidate
            //              (p->ai_family).
            //   type     - parameter #2: socket type of this candidate
            //              (p->ai_socktype).
            //   protocol - parameter #3: protocol of this candidate
            //              (p->ai_protocol).
            //   returns     a new SOCKET, or INVALID_SOCKET if this
            //              candidate can't be created (loop continues).
            sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

            if (sock == INVALID_SOCKET) continue;

            // --- Added explanation: connect(sock, ai_addr, ai_addrlen) -----------
            // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
            //   s       - parameter #1: the socket just created (sock).
            //   name    - parameter #2: this candidate's address
            //             (p->ai_addr).
            //   namelen - parameter #3: size of that address
            //             ((int)p->ai_addrlen).
            //   returns    0 on success (loop breaks, keeping this connected
            //             `sock`); nonzero means this candidate failed.
            if (connect(sock, p->ai_addr, (int)p->ai_addrlen) == 0) break;

            // --- Added explanation: closesocket(sock) - discards a candidate
            // socket that failed to connect; same explanation as the final
            // closesocket(sock) call later in this function.
            closesocket(sock); sock = INVALID_SOCKET;

        }

        // --- Added explanation: freeaddrinfo(res) --------------------------------
        // Generic syntax:  freeaddrinfo( addrinfo* ai )
        //   ai - parameter #1 (only parameter): the addrinfo list from
        //        getaddrinfo() (res); releases its memory now that the loop
        //        above has finished trying every candidate.
        freeaddrinfo(res);
 
        if (sock == INVALID_SOCKET) {

            safe_print("[T" + std::to_string(id) + "] connect failed\n");

            failc++;

            continue;

        }
 
        std::string req = make_request(host, path);

        auto t0 = std::chrono::high_resolution_clock::now();

        // --- Added explanation: send(sock, req.c_str(), len, flags) --------------
        // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
        //   s     - parameter #1: connected socket to write to (sock).
        //   buf   - parameter #2: bytes to send (req.c_str(), the full HTTP
        //           request built by make_request()).
        //   len   - parameter #3: how many bytes to send ((int)req.size()).
        //   flags - parameter #4: 0 means normal blocking send.
        int sent = send(sock, req.c_str(), (int)req.size(), 0);

        if (sent == SOCKET_ERROR) {

            safe_print("[T" + std::to_string(id) + "] send error: " + std::to_string(WSAGetLastError()) + "\n");

            closesocket(sock);

            failc++;

            continue;

        }
 
        // read until connection closed

        std::string resp;

        char buf[1024];

        while (true) {

            // --- Added explanation: recv(sock, buf, len, flags) ---------------------
            // Generic syntax:  recv( SOCKET s, char* buf, int len, int flags )
            //   s     - parameter #1: connected socket to read from (sock).
            //   buf   - parameter #2: destination buffer (buf, 1024 bytes).
            //   len   - parameter #3: max bytes to read (sizeof(buf)).
            //   flags - parameter #4: 0 means normal blocking receive.
            //   returns  >0 bytes received (appended to `resp` and the loop
            //           continues), 0 = server closed the connection
            //           (normal end, since this server sent "Connection:
            //           close" and closes after replying), <0 = error - both
            //           0 and <0 break this loop.
            int n = recv(sock, buf, sizeof(buf), 0);

            if (n > 0) {

                resp.append(buf, buf + n);

            } else if (n == 0) {

                break; // server closed

            } else {

                safe_print("[T" + std::to_string(id) + "] recv error: " + std::to_string(WSAGetLastError()) + "\n");

                break;

            }

        }
 
        auto t1 = std::chrono::high_resolution_clock::now();

        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
 
        // simple validation: look for HTTP status line

        if (resp.find("HTTP/1.0") != std::string::npos || resp.find("HTTP/1.1") != std::string::npos) {

            succ++;

            std::ostringstream out;

            out << "[T" << id << " R" << r << "] OK " << (int)ms << " ms, bytes=" << resp.size() << "\n";

            safe_print(out.str());

        } else {

            failc++;

            safe_print("[T" + std::to_string(id) + " R" + std::to_string(r) + "] invalid response\n");

        }
 
        // --- Added explanation: closesocket(sock) --------------------------------
        // Generic syntax:  closesocket( SOCKET s )
        //   s - parameter #1 (only parameter): the socket handle to release
        //       (sock). A fresh socket is created for EVERY request (see
        //       the top of this loop), so it's closed here at the end of
        //       each request too - no connection reuse/keep-alive.
        closesocket(sock);

        // small pause to avoid perfect sync

        // --- Added explanation: sleep_for(milliseconds(30 + (id*7)%100)) --------
        // Generic syntax:  std::this_thread::sleep_for( std::chrono::duration )
        //   duration - parameter #1 (only parameter): how long to pause THIS
        //              thread before its next request; the amount varies
        //              per-thread (30 to 129 ms, based on `id`) specifically
        //              so that multiple worker threads don't all fire their
        //              requests in perfect lockstep.
        std::this_thread::sleep_for(std::chrono::milliseconds(30 + (id*7)%100));

    }

}
 
int main(int argc, char* argv[]) {

    if (argc < 6) {

        std::cout << "Usage: " << argv[0] << " <host> <port> <path> <num_threads> <requests_per_thread>\n"
<< "Example: " << argv[0] << " 127.0.0.1 8080 / 10 5\n";

        return 1;

    }
 
    std::string host = argv[1];

    std::string port = argv[2];

    std::string path = argv[3];

    int threads = atoi(argv[4]);

    int reqs = atoi(argv[5]);

    if (threads <= 0) threads = 1;

    if (reqs <= 0) reqs = 1;
 
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
 
    std::cout << "HTTP load client -> host=" << host << " port=" << port << " path=" << path
<< " threads=" << threads << " reqs/thread=" << reqs << "\n";
 
    std::vector<std::thread> pool;

    // --- Added explanation: pool.emplace_back(worker, host, port, path, i, reqs)
    // Generic syntax:  vectorOfThread.emplace_back( callableFunction, arg1, arg2, arg3, arg4, arg5 )
    //   callableFunction - parameter #1: the function the new thread runs
    //                      (worker).
    //   arg1 (host)      - parameter #2: forwarded into worker's `host`.
    //   arg2 (port)      - parameter #3: forwarded into worker's `port`.
    //   arg3 (path)      - parameter #4: forwarded into worker's `path`.
    //   arg4 (i)         - parameter #5: forwarded into worker's `id`
    //                      (1..threads across the loop).
    //   arg5 (reqs)      - parameter #6: forwarded into worker's
    //                      `reqsPerThread`.
    // Constructs a NEW std::thread in-place inside `pool`, starting it
    // immediately; kept (not detached) so main() can .join() them below.
    for (int i = 1; i <= threads; ++i) pool.emplace_back(worker, host, port, path, i, reqs);
 
    // --- Added explanation: t.joinable() / t.join() -----------------------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() is true if `t` still
    //   represents a live, not-yet-joined thread; join() blocks main until
    //   `t` finishes running worker(). Looping this over every entry in
    //   `pool` waits for all `threads` worker threads to finish.
    for (auto &t : pool) if (t.joinable()) t.join();
 
    // --- Added explanation: succ.load() / failc.load() --------------------------
    // Generic syntax:  atomicVariable.load()
    //   (no parameters) - reads the current value of an std::atomic<int>
    //   (succ or failc) in a thread-safe way. Safe to call here because all
    //   worker threads have already been joined above, so no thread is
    //   still incrementing these counters concurrently.
    std::cout << "\nDone. success=" << succ.load() << " failures=" << failc.load() << "\n";
 
    // --- Added explanation: WSACleanup() ----------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - undoes WSAStartup(); releases Winsock's internal
    //   resources for this process, now that every worker thread is done.
    WSACleanup();

    return 0;

}
 
// ==========================================================================================

// simple_http_server_win.exe 8080
 
// http_load_client_win.exe 127.0.0.1 8080 / 10 5
 
 

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() requires 5 command-line arguments (host, port, path,
//     num_threads, requests_per_thread); if fewer are given it prints usage
//     and exits. Otherwise threads/reqs are floored at 1 and WSAStartup()
//     initializes Winsock ONCE for the whole process.
// #2  A std::vector<std::thread> `pool` is built: pool.emplace_back() in a
//     loop constructs `threads` NEW std::thread objects, each immediately
//     running worker(host, port, path, i, reqs) concurrently.
// #3  Inside each concurrently-running worker(), a for-loop runs
//     `reqsPerThread` times, and EACH iteration is a fully independent
//     HTTP request cycle (fresh DNS resolution, fresh socket, fresh TCP
//     connection - no reuse across requests):
//       #3a  getaddrinfo() resolves host:port into candidate addresses;
//            a for-loop tries socket()+connect() on each until one
//            connects (or all fail, in which case failc++ and this
//            iteration is skipped via `continue`).
//       #3b  make_request(host, path) builds a complete HTTP/1.0 GET
//            request string; a high-resolution timer (t0) starts.
//       #3c  send() transmits the request; if it fails, the socket is
//            closed, failc++ and the iteration is skipped.
//       #3d  A while(true) loop calls recv() repeatedly, appending every
//            byte received into `resp`, until the server closes the
//            connection (n == 0 - expected, since the request declared
//            "Connection: close") or an error occurs.
//       #3e  The timer stops (t1) and elapsed milliseconds are computed.
//            `resp` is checked for an "HTTP/1.0" or "HTTP/1.1" status
//            line: if found, succ++ and a timing/size line is printed; if
//            not, failc++ and "invalid response" is printed.
//       #3f  closesocket(sock) closes this request's connection, then
//            sleep_for() pauses a per-thread-varying amount (30-129 ms,
//            derived from `id`) before the next iteration, so concurrent
//            threads don't all hit the server in perfect lockstep.
// #4  Back in main(), `for (auto &t : pool) if (t.joinable()) t.join();`
//     waits for every one of the `threads` worker threads to finish (all
//     `threads * reqsPerThread` requests completed, in total).
// #5  main() prints the final success/failure counts (read via
//     succ.load()/failc.load(), safe now that all threads have joined),
//     calls WSACleanup(), and returns 0.
// #6  Net effect: this is a simple concurrent HTTP load-testing tool meant
//     to hammer a server like 10_simple_http_server_win.cpp with many
//     overlapping short-lived connections and report how many requests
//     round-tripped successfully versus failed, and how long each took.
// ============================================================================
