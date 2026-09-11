// Compile (MSVC):

//   cl /EHsc pipe_server_win.cpp

// MinGW:

//   g++ -std=c++11 pipe_server_win.cpp -o pipe_server_win.exe

//

// Run: pipe_server_win.exe

// Server waits for clients. Press ENTER in server console to stop.
 
#include <windows.h>

#include <iostream>

#include <string>

#include <thread>

#include <atomic>
 
std::atomic<bool> running(true);
 
// --- Added explanation: stopper_thread() function definition ----------------
// Generic syntax:  void stopper_thread()
//   (no parameters) - runs on its own std::thread (see the
//   `std::thread stopper(stopper_thread);` call in main() below). It blocks
//   on std::getline waiting for the operator to press ENTER at the console,
//   then sets the shared `running` flag to false so the server's main
//   while(running) loop (in main(), below) notices and exits after it
//   finishes handling whatever client it is currently servicing.
void stopper_thread() {

    std::string s;

    std::getline(std::cin, s);

    running = false;

}
 
int main() {

    const char* pipeName = R"(\\.\pipe\DemoPipe)";

    std::cout << "Named-pipe server starting. Pipe name: " << pipeName << "\n";

    std::cout << "Press ENTER to stop the server.\n";
 
    // --- Added explanation: std::thread stopper(stopper_thread) ----------------
    // Generic syntax:  std::thread variableName( callableFunction )
    //   callableFunction - parameter #1 (only parameter): the function this
    //                      new thread runs immediately and concurrently with
    //                      main() (stopper_thread, here). This lets the
    //                      server keep accepting/serving pipe clients on the
    //                      main thread while a SEPARATE thread waits for the
    //                      operator to press ENTER.
    std::thread stopper(stopper_thread);
 
    while (running) {

        // Create a named pipe instance (duplex)

        // --- Added explanation: CreateNamedPipeA(name, openMode, pipeMode,
        //     maxInstances, outBufSize, inBufSize, timeout, security) --------
        // Generic syntax:  CreateNamedPipeA( LPCSTR name, DWORD openMode,
        //     DWORD pipeMode, DWORD maxInstances, DWORD outBufSize,
        //     DWORD inBufSize, DWORD defaultTimeOut,
        //     LPSECURITY_ATTRIBUTES security )
        //   Each parameter is labeled by the inline comment beside it below;
        //   in short:
        //     name (pipeName)          - the well-known pipe path
        //                                "\\.\pipe\DemoPipe" that clients
        //                                connect to.
        //     openMode                 - PIPE_ACCESS_DUPLEX: this end can
        //                                both read and write.
        //     pipeMode                 - message-typed, message-read,
        //                                blocking (PIPE_WAIT) I/O.
        //     maxInstances (1)         - only one client can be connected to
        //                                THIS pipe name at a time; the
        //                                surrounding while(running) loop
        //                                creates a fresh instance for each
        //                                client, one after another.
        //     outBufSize/inBufSize     - suggested kernel buffer sizes (512
        //                                bytes each) for data flowing out
        //                                to/in from a client.
        //     defaultTimeOut (0)       - use the system default.
        //     security (nullptr)      - default security descriptor.
        //   returns  a HANDLE to the new pipe instance, or
        //            INVALID_HANDLE_VALUE on failure (checked right below).
        HANDLE hPipe = CreateNamedPipeA(

            pipeName,

            PIPE_ACCESS_DUPLEX,                    // read/write access

            PIPE_TYPE_MESSAGE |                    // message-type pipe

            PIPE_READMODE_MESSAGE |                // message-read mode

            PIPE_WAIT,                             // blocking mode

            1,                                     // max instances

            512,                                   // out buffer size

            512,                                   // in buffer size

            0,                                     // default timeout

            nullptr);                              // default security
 
        if (hPipe == INVALID_HANDLE_VALUE) {

            std::cerr << "CreateNamedPipe failed: " << GetLastError() << "\n";

            break;

        }
 
        std::cout << "Waiting for a client to connect...\n";

        // --- Added explanation: ConnectNamedPipe(hPipe, nullptr) -------------------
        // Generic syntax:  ConnectNamedPipe( HANDLE pipe, LPOVERLAPPED overlapped )
        //   pipe       - parameter #1: the pipe instance just created
        //                (hPipe).
        //   overlapped - parameter #2: nullptr means synchronous (blocking)
        //                mode - this call BLOCKS the server here until a
        //                client calls CreateFileA on this pipe name (see
        //                4_pipe_client_win.cpp), or returns immediately with
        //                ERROR_PIPE_CONNECTED if a client raced in and
        //                connected already.
        //   returns      nonzero on a fresh connection; the surrounding
        //                ternary treats ERROR_PIPE_CONNECTED as success too.
        BOOL connected = ConnectNamedPipe(hPipe, nullptr) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (!connected) {

            std::cerr << "ConnectNamedPipe failed: " << GetLastError() << "\n";

            CloseHandle(hPipe);

            break;

        }
 
        std::cout << "Client connected.\n";
 
        // Read message from client

        char buffer[512];

        DWORD bytesRead = 0;

        // --- Added explanation: ReadFile(hPipe, buffer, count, &bytesRead, nullptr)
        // Generic syntax:  ReadFile( HANDLE file, LPVOID buffer, DWORD count,
        //                            LPDWORD bytesReadOut, LPOVERLAPPED overlapped )
        //   file         - parameter #1: the connected pipe handle (hPipe).
        //   buffer       - parameter #2: destination for incoming bytes
        //                  (buffer, a local 512-byte array).
        //   count        - parameter #3: max bytes to read - sizeof(buffer)
        //                  minus 1, reserving room for a manual '\0'
        //                  terminator below.
        //   bytesReadOut - parameter #4: out-parameter; receives how many
        //                  bytes actually arrived (&bytesRead).
        //   overlapped   - parameter #5: nullptr means synchronous/blocking
        //                  read - this call blocks until the client writes
        //                  something into the pipe (or the pipe breaks).
        BOOL ok = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);

        if (ok && bytesRead > 0) {

            buffer[bytesRead] = '\0';

            std::cout << "[Server] Received: " << buffer << "\n";
 
            // Send reply

            std::string reply = std::string("Server ACK: ") + buffer;

            DWORD bytesWritten = 0;

            // --- Added explanation: WriteFile(hPipe, data, count, &bytesWritten, nullptr)
            // Generic syntax:  WriteFile( HANDLE file, LPCVOID buffer, DWORD count,
            //                             LPDWORD writtenOut, LPOVERLAPPED overlapped )
            //   file       - parameter #1: the same connected pipe handle
            //                (hPipe) the request was just read from - named
            //                pipes opened PIPE_ACCESS_DUPLEX can be written
            //                to and read from using this one handle.
            //   buffer     - parameter #2: the reply bytes (reply.c_str(),
            //                "Server ACK: <whatever the client sent>").
            //   count      - parameter #3: byte count, from reply.size().
            //   writtenOut - parameter #4: out-parameter; bytes actually
            //                written (&bytesWritten).
            //   overlapped - parameter #5: nullptr, synchronous mode.
            BOOL w = WriteFile(hPipe, reply.c_str(), (DWORD)reply.size(), &bytesWritten, nullptr);

            if (!w) {

                std::cerr << "[Server] WriteFile failed: " << GetLastError() << "\n";

            }

        } else {

            std::cerr << "[Server] ReadFile failed or no data: " << GetLastError() << "\n";

        }
 
        // Disconnect & close this pipe instance; loop to accept another client

        // --- Added explanation: FlushFileBuffers(hPipe) -----------------------
        // Generic syntax:  FlushFileBuffers( HANDLE file )
        //   file - parameter #1 (only parameter): the pipe handle (hPipe).
        //   Blocks until any data the server wrote (the reply above) has
        //   actually been transmitted/consumed on the client's end, so the
        //   pipe can be safely disconnected next without truncating it.
        FlushFileBuffers(hPipe);

        // --- Added explanation: DisconnectNamedPipe(hPipe) ---------------------
        // Generic syntax:  DisconnectNamedPipe( HANDLE pipe )
        //   pipe - parameter #1 (only parameter): the pipe instance to
        //          disconnect (hPipe). This forcibly disconnects the current
        //          client (if still attached) so this pipe instance can be
        //          reused/destroyed and a NEW instance created for the next
        //          client on the next loop iteration.
        DisconnectNamedPipe(hPipe);

        // --- Added explanation: CloseHandle(hPipe) ------------------------------
        // Generic syntax:  CloseHandle( HANDLE object )
        //   object - parameter #1 (only parameter): the HANDLE to release
        //            back to the OS (hPipe). After this, the while(running)
        //            loop goes back to CreateNamedPipeA to create a brand
        //            NEW pipe instance for the next client, since a named
        //            pipe HANDLE cannot be reused once closed.
        CloseHandle(hPipe);

        std::cout << "Client handled, pipe closed.\n\n";

    }
 
    // --- Added explanation: stopper.joinable() / stopper.join() ----------------
    // Generic syntax:  threadObj.joinable()  and  threadObj.join()
    //   (no parameters for either) - joinable() reports whether `stopper`
    //   represents a still-live, not-yet-joined thread (true here, since it
    //   was created above and never joined until now); join() then blocks
    //   main() until that thread (stopper_thread) actually returns - which
    //   it already has by the time we reach here, since running became
    //   false is what let the while(running) loop above exit in the first
    //   place.
    if (stopper.joinable()) stopper.join();

    std::cout << "Server stopping.\n";

    return 0;

}

 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() prints its startup banner, then launches `stopper` - a second
//     thread running stopper_thread(), which immediately blocks on
//     std::getline(std::cin, ...) waiting for the operator to press ENTER.
//     Meanwhile main()'s own thread continues straight into the
//     while(running) loop below - the two threads now run concurrently.
// #2  Each iteration of while(running):
//       #2a CreateNamedPipeA creates a brand-new named-pipe instance called
//           \\.\pipe\DemoPipe (duplex, message-mode, blocking). If this
//           fails, the loop breaks and the server shuts down.
//       #2b ConnectNamedPipe BLOCKS this thread until a client process
//           (4_pipe_client_win.cpp) calls CreateFileA on that same pipe
//           name and connects.
//       #2c Once connected, ReadFile BLOCKS until the client writes its
//           message (e.g. "Hello from client") into the pipe, which arrives
//           in `buffer`.
//       #2d The server null-terminates the buffer, prints what it received,
//           builds a reply string "Server ACK: <message>", and WriteFile
//           sends that reply back to the SAME client over the SAME pipe
//           handle (duplex - one handle serves both directions).
//       #2e FlushFileBuffers waits for the client to consume the reply,
//           DisconnectNamedPipe kicks the client off this pipe instance, and
//           CloseHandle releases the instance. Because a named-pipe instance
//           cannot be reconnected once disconnected, the loop goes back to
//           step #2a and creates an entirely NEW instance for the NEXT
//           client - so exactly one client is served per loop iteration.
// #3  This repeats indefinitely - one client connects, exchanges one
//     request/reply, and disconnects, then the server waits for the next
//     one - until `running` becomes false.
// #4  `running` becomes false only when the OPERATOR (a human at the server
//     console) presses ENTER, which unblocks stopper_thread()'s
//     std::getline call and sets running = false. The main thread only
//     notices this at the TOP of the while(running) check - so if the
//     server is mid-way through serving a client (blocked in ConnectNamedPipe
//     or ReadFile) when ENTER is pressed, it still finishes that one
//     client's request/reply before the loop condition is re-checked and
//     found false.
// #5  Once the loop exits, main() calls stopper.join() (via the
//     joinable()-guarded call) to wait for stopper_thread() to fully finish,
//     prints "Server stopping.", and returns 0.
// ============================================================================