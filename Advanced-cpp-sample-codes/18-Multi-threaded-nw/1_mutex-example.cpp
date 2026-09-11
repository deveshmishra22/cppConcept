// // Compile (MSVC): cl /EHsc mutex_socket_example.cpp ws2_32.lib
// // or (MinGW): g++ -std=c++11 mutex_socket_example.cpp -lws2_32 -o mutex_socket_example.exe
// Both threads (t1 and t2) share the same socket.
// Each thread tries to send messages.
// The std::lock_guard<std::mutex> ensures:
// Only one thread enters the send() section at a time.
// Others wait until the mutex is released.
// This prevents mixed-up data on the socket.
 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")
 
std::mutex send_mutex; // 🔒 protects socket send()
// --- Added explanation: send_messages(sock, thread_id) --------------------
// Generic syntax:  void send_messages( SOCKET sock, int thread_id )
//   sock      - parameter #1: the already-connected SOCKET handle both
//               threads share (the single `sock` created in main() below);
//               this function does NOT open its own socket, it just sends
//               on whichever one it is handed.
//   thread_id - parameter #2: an identifying number used only in the
//               printed/sent message text (1 for t1, 2 for t2 below), so
//               the echoed output can be traced back to which thread sent it.
void send_messages(SOCKET sock, int thread_id) {
    for (int i = 1; i <= 5; ++i) {
        std::string msg = "Thread " + std::to_string(thread_id) +
                          " says hello " + std::to_string(i) + "\n";
        {
            // --- Added explanation: lock_guard<mutex> lock(send_mutex) ----------
            // Generic syntax:  std::lock_guard<MutexType> variableName( mutexObj )
            //   MutexType - template parameter: the type of mutex being wrapped
            //               (mutex, here).
            //   mutexObj  - constructor parameter #1: the specific mutex instance
            //               (send_mutex) to lock RIGHT NOW, at construction. Its
            //               destructor calls send_mutex.unlock() automatically
            //               when `lock` goes out of scope (end of this `{ }`
            //               block, right after the send() call below), so
            //               whichever of t1/t2 gets here first finishes its
            //               send()+cout before the other thread is allowed in.
            std::lock_guard<std::mutex> lock(send_mutex); // only one thread sends at a time
            // --- Added explanation: send(sock, msg.c_str(), msg.size(), 0) ------
            // Generic syntax:  send( SOCKET s, const char* buf, int len, int flags )
            //   s     - parameter #1: the connected socket to write bytes to
            //           (sock, shared by both threads here).
            //   buf   - parameter #2: pointer to the raw bytes to send
            //           (msg.c_str(), the C-string form of this thread's
            //           "Thread N says hello i" message).
            //   len   - parameter #3: how many bytes from buf to send
            //           ((int)msg.size(), the message's length).
            //   flags - parameter #4: socket send flags; 0 means no special
            //           behavior (the normal case).
            send(sock, msg.c_str(), (int)msg.size(), 0);
            std::cout << "[Thread " << thread_id << "] Sent: " << msg;
        }
        // --- Added explanation: sleep_for(milliseconds(400)) -----------------
        // Generic syntax:  std::this_thread::sleep_for( duration )
        //   duration - parameter #1: how long THIS thread pauses before its
        //              next loop iteration (400 milliseconds here). This is
        //              only a MINIMUM delay - the OS may wake the thread
        //              later, but never earlier.
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}
 
int main() {

    // Initialize Winsock
    WSADATA wsa;
    // --- Added explanation: WSAStartup(MAKEWORD(2,2), &wsa) ------------------
    // Generic syntax:  WSAStartup( WORD versionRequested, LPWSADATA lpWSAData )
    //   versionRequested - parameter #1: the Winsock version to request,
    //                      packed by MAKEWORD(2,2) into "version 2.2" (the
    //                      standard modern value).
    //   lpWSAData        - parameter #2: address of a WSADATA struct (&wsa)
    //                      that Winsock fills in with details about the
    //                      implementation it loaded. Must be called once
    //                      before ANY other Winsock function (socket(),
    //                      connect(), send(), etc. below all require this).
    WSAStartup(MAKEWORD(2,2), &wsa);
 
    // Connect to local echo server
    const char* server = "127.0.0.1";
    const char* port = "9000";
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo* result = nullptr;
    // --- Added explanation: getaddrinfo(server, port, &hints, &result) -------
    // Generic syntax:  getaddrinfo( const char* node, const char* service,
    //                                const addrinfo* hints, addrinfo** res )
    //   node    - parameter #1: hostname or IP string to resolve ("127.0.0.1").
    //   service - parameter #2: port number or service name, as a string
    //             ("9000").
    //   hints   - parameter #3: an addrinfo describing what KIND of address
    //             is wanted (&hints, filled above with AF_INET/SOCK_STREAM)
    //             so getaddrinfo doesn't return address types this program
    //             can't use.
    //   res     - parameter #4: out-parameter (&result); on success,
    //             getaddrinfo allocates a linked list of matching addrinfo
    //             results and points `result` at the first one.
    getaddrinfo(server, port, &hints, &result);
    // --- Added explanation: socket(family, socktype, protocol) ---------------
    // Generic syntax:  SOCKET socket( int af, int type, int protocol )
    //   af       - parameter #1: address family (result->ai_family, filled
    //              in by getaddrinfo above - AF_INET for IPv4).
    //   type     - parameter #2: socket type (result->ai_socktype -
    //              SOCK_STREAM, i.e. a reliable TCP stream).
    //   protocol - parameter #3: specific protocol (result->ai_protocol,
    //              typically 0/IPPROTO_TCP for a stream socket). Returns a
    //              new, unconnected SOCKET handle.
    SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    // --- Added explanation: connect(sock, ai_addr, ai_addrlen) ---------------
    // Generic syntax:  connect( SOCKET s, const sockaddr* name, int namelen )
    //   s       - parameter #1: the unconnected socket to connect (sock,
    //             just created above).
    //   name    - parameter #2: address of the server to connect to
    //             (result->ai_addr, the resolved 127.0.0.1:9000 address).
    //   namelen - parameter #3: size in bytes of that address structure
    //             ((int)result->ai_addrlen). Blocks until the TCP handshake
    //             with the echo server completes (or fails).
    connect(sock, result->ai_addr, (int)result->ai_addrlen);
    // --- Added explanation: freeaddrinfo(result) ------------------------------
    // Generic syntax:  freeaddrinfo( addrinfo* ai )
    //   ai - parameter #1 (only parameter): the linked list returned by
    //        getaddrinfo above (result). Must be freed once its contents
    //        (the resolved address) are no longer needed - sock has already
    //        been created and connected by this point, so it's safe to free.
    freeaddrinfo(result); 
    std::cout << "Connected to " << server << ":" << port << "\n";

    // Start two threads sharing one socket
    // --- Added explanation: thread t1(send_messages, sock, 1) ----------------
    // Generic syntax:  std::thread variableName( callableFunction, arg1, arg2 )
    //   callableFunction - parameter #1: the function the new thread runs
    //                      (send_messages, here).
    //   arg1 (sock)      - parameter #2: forwarded into send_messages'
    //                      first parameter `sock` - the SAME socket handle
    //                      t2 below will also be given, so both threads
    //                      really do share one socket.
    //   arg2 (1)         - parameter #3: forwarded into send_messages'
    //                      second parameter `thread_id`.
    // This starts a NEW thread immediately running send_messages(sock, 1)
    // concurrently with main() and (once created) t2 below.
    std::thread t1(send_messages, sock, 1);
    // --- Added explanation: same generic syntax as t1 above; this time
    // arg2 = 2, so this starts a second thread running send_messages(sock, 2)
    // - concurrently with t1, using the SAME sock.
    std::thread t2(send_messages, sock, 2);
 
    // --- Added explanation: t1.join() -----------------------------------------
    // Generic syntax:  threadObj.join()
    //   (no parameters) - blocks the calling (main) thread until `t1` has
    //   fully finished running send_messages(sock, 1) (all 5 iterations).
    t1.join();
    // --- Added explanation: t2.join() -----------------------------------------
    // Generic syntax: same as t1.join() above (no parameters); blocks main
    // until t2 has finished all 5 of its send iterations too. Only once BOTH
    // joins return does main reach the "Both threads finished..." line below.
    t2.join();
 
    std::cout << "Both threads finished sending.\n";
    // --- Added explanation: shutdown(sock, SD_BOTH) ---------------------------
    // Generic syntax:  shutdown( SOCKET s, int how )
    //   s   - parameter #1: the socket to shut down (sock, now that both
    //         threads are done sending).
    //   how - parameter #2: which direction(s) to disable; SD_BOTH disables
    //         further sends AND receives on this socket (a clean TCP
    //         shutdown handshake) before it is closed below.
    shutdown(sock, SD_BOTH);
    // --- Added explanation: closesocket(sock) ----------------------------------
    // Generic syntax:  closesocket( SOCKET s )
    //   s - parameter #1 (only parameter): the socket handle to release back
    //       to the OS (sock). After this call, sock must not be used again.
    closesocket(sock);
    // --- Added explanation: WSACleanup() ---------------------------------------
    // Generic syntax:  WSACleanup()
    //   (no parameters) - releases the Winsock resources WSAStartup acquired
    //   at the top of main(). Should be the last Winsock-related call.
    WSACleanup();
    return 0;

}
 
 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() calls WSAStartup to initialize Winsock, then resolves
//     "127.0.0.1":"9000" with getaddrinfo, creates a SOCKET with socket(),
//     and connect()s it to that address - blocking until a server is
//     listening on 127.0.0.1:9000 and accepts the connection (there is NO
//     server in this file; you must run a separate echo server, or 3_server
//     from this same folder configured for port 9000/matching port, for
//     connect() to succeed instead of failing/blocking).
// #2  freeaddrinfo(result) releases the resolved-address list; `sock` is now
//     a single connected TCP socket. "Connected to 127.0.0.1:9000" is printed.
// #3  std::thread t1(send_messages, sock, 1) is constructed: a NEW OS thread
//     starts immediately, running send_messages(sock, 1) concurrently with
//     main. std::thread t2(send_messages, sock, 2) does the same with
//     thread_id = 2. From this point t1, t2, and main are all running at once.
// #4  Inside EACH thread's send_messages loop (5 iterations each):
//       #4a  build msg = "Thread <id> says hello <i>\n".
//       #4b  lock_guard locks send_mutex - if the OTHER thread is currently
//            inside its own lock_guard scope, this thread blocks here until
//            it unlocks (lock_guard's destructor releases it automatically
//            at the end of the `{ }` block, right after send()+cout run).
//       #4c  send(sock, msg.c_str(), msg.size(), 0) writes msg's bytes onto
//            the shared socket, then cout prints "[Thread <id>] Sent: <msg>".
//            Because of the mutex, t1's and t2's 5-line writes and 5 prints
//            can never interleave mid-message - each call to send()+cout
//            fully completes as one atomic unit before the other thread's
//            iteration can begin.
//       #4d  the lock is released (end of scope) and the thread sleeps 400ms
//            before its next iteration, giving the other thread a chance to
//            acquire send_mutex and run its own iteration in between.
// #5  Since t1 and t2 both sleep ~400ms per iteration and contend for the
//     same mutex, their 5 sends each typically INTERLEAVE at the message
//     level (e.g. Thread 1's message 1, then Thread 2's message 1, then
//     Thread 1's message 2, ...) - though the OS scheduler ultimately decides
//     the exact order; only the intra-message mixing is prevented, not the
//     ordering between whole messages.
// #6  main() calls t1.join(), which blocks until t1 has sent all 5 messages
//     and returned; then t2.join() blocks until t2 finishes its 5 as well.
//     By the time both joins return, exactly 10 messages have been sent on
//     `sock` (5 from each thread), all cleanly separated by the mutex.
// #7  "Both threads finished sending." is printed. shutdown(sock, SD_BOTH)
//     tells the peer no more data is coming in either direction, then
//     closesocket(sock) releases the socket handle and WSACleanup() releases
//     Winsock's resources.
// #8  main() returns 0, ending the program. Note: since nothing in this file
//     ever calls recv() on `sock`, this program never reads back whatever
//     the server may have echoed - it only sends and then closes.
// ============================================================================
