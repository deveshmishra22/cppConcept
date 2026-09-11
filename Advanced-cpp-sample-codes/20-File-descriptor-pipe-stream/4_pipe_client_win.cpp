// Compile (MSVC):

//   cl /EHsc pipe_client_win.cpp

// MinGW:

//   g++ -std=c++11 pipe_client_win.cpp -o pipe_client_win.exe

//

// Run: pipe_client_win.exe "Hello from client"

// Example: pipe_client_win.exe "Hi server!"

// If no argument provided, a default message will be used.
 
#include <windows.h>

#include <iostream>

#include <string>
 
// --- Added explanation: main(argc, argv) entry point -------------------------
// Generic syntax:  int main( int argc, char* argv[] )
//   argc - parameter #1: number of command-line arguments (>= 1).
//   argv - parameter #2: the arguments themselves; argv[1], if present, is
//          used below as the message to send to the server (otherwise a
//          default message is used).
int main(int argc, char* argv[]) {

    const char* pipeName = R"(\\.\pipe\DemoPipe)";

    std::string message = (argc >= 2) ? argv[1] : "Hello from client";
 
    std::cout << "Connecting to pipe: " << pipeName << " ...\n";
 
    // Try to open the pipe (wait up to a bit)

    // If server not ready, Sleep/retry a few times.

    HANDLE hPipe = INVALID_HANDLE_VALUE;

    for (int i = 0; i < 5; ++i) {

        // --- Added explanation: CreateFileA(name, access, share, security,
        //     disposition, flags, template) ---------------------------------
        // Generic syntax:  CreateFileA( LPCSTR name, DWORD access, DWORD share,
        //     LPSECURITY_ATTRIBUTES security, DWORD disposition,
        //     DWORD flagsAndAttrs, HANDLE templateFile )
        //   name (pipeName)   - parameter #1: the same well-known pipe path
        //                       "\\.\pipe\DemoPipe" the server created with
        //                       CreateNamedPipeA - opening a named pipe by
        //                       path is the client-side equivalent of
        //                       connecting a socket.
        //   access            - parameter #2: GENERIC_READ | GENERIC_WRITE,
        //                       so this client can both send its message and
        //                       read the server's reply through the same
        //                       handle.
        //   share             - parameter #3: 0, no sharing with other
        //                       handles/processes.
        //   security          - parameter #4: nullptr, default security.
        //   disposition       - parameter #5: OPEN_EXISTING - fail unless
        //                       the pipe already exists (it must, since only
        //                       the server creates pipe instances).
        //   flagsAndAttrs     - parameter #6: 0, no special flags.
        //   templateFile      - parameter #7: nullptr, unused for pipes.
        //   returns  a HANDLE to the connected pipe on success, or
        //           INVALID_HANDLE_VALUE on failure (e.g. if the server
        //           hasn't created/isn't waiting on an instance yet - see
        //           the retry loop this call lives inside).
        hPipe = CreateFileA(

            pipeName,               // pipe name

            GENERIC_READ | GENERIC_WRITE,

            0,                      // no sharing

            nullptr,

            OPEN_EXISTING,

            0,

            nullptr);

        if (hPipe != INVALID_HANDLE_VALUE) break;

        DWORD err = GetLastError();

        if (err == ERROR_PIPE_BUSY) {

            // Wait up to 2 seconds

            // --- Added explanation: WaitNamedPipeA(pipeName, timeoutMs) ------------
            // Generic syntax:  WaitNamedPipeA( LPCSTR name, DWORD timeoutMs )
            //   name      - parameter #1: the pipe path to wait for
            //               (pipeName).
            //   timeoutMs - parameter #2: how long to wait, in milliseconds,
            //               for an instance to become available (2000, i.e.
            //               2 seconds here). Used only when CreateFileA just
            //               failed with ERROR_PIPE_BUSY (all of the
            //               server's pipe instances are currently occupied);
            //               this blocks until one frees up or the timeout
            //               elapses.
            //   returns     nonzero if a pipe instance became available
            //               before the timeout.
            if (!WaitNamedPipeA(pipeName, 2000)) {

                std::cerr << "WaitNamedPipe timeout\n";

                return 1;

            }

        } else {

            // Sleep and retry

            // --- Added explanation: Sleep(milliseconds) -----------------------------
            // Generic syntax:  Sleep( DWORD milliseconds )
            //   milliseconds - parameter #1 (only parameter): how long to
            //                  pause this thread (200 ms here) before the
            //                  for-loop above retries CreateFileA. Used for
            //                  errors OTHER than ERROR_PIPE_BUSY (e.g. the
            //                  server hasn't started listening yet at all),
            //                  giving it a moment to start up.
            Sleep(200);

        }

    }
 
    if (hPipe == INVALID_HANDLE_VALUE) {

        std::cerr << "Failed to open pipe: " << GetLastError() << "\n";

        return 1;

    }
 
    std::cout << "Connected. Sending message: " << message << "\n";
 
    DWORD bytesWritten = 0;

    // --- Added explanation: WriteFile(hPipe, data, count, &bytesWritten, nullptr)
    // Generic syntax:  WriteFile( HANDLE file, LPCVOID buffer, DWORD count,
    //                             LPDWORD writtenOut, LPOVERLAPPED overlapped )
    //   file       - parameter #1: the connected pipe handle (hPipe).
    //   buffer     - parameter #2: the outgoing bytes (message.c_str(), the
    //                text from argv[1] or the default message).
    //   count      - parameter #3: byte count, from message.size().
    //   writtenOut - parameter #4: out-parameter; bytes actually written
    //                (&bytesWritten).
    //   overlapped - parameter #5: nullptr, synchronous mode. This is the
    //                request the server's ReadFile call is waiting to
    //                receive.
    BOOL w = WriteFile(hPipe, message.c_str(), (DWORD)message.size(), &bytesWritten, nullptr);

    if (!w) {

        std::cerr << "WriteFile failed: " << GetLastError() << "\n";

        CloseHandle(hPipe);

        return 1;

    }
 
    // Read reply

    char buffer[512];

    DWORD bytesRead = 0;

    // --- Added explanation: ReadFile(hPipe, buffer, count, &bytesRead, nullptr)
    // Generic syntax:  ReadFile( HANDLE file, LPVOID buffer, DWORD count,
    //                            LPDWORD bytesReadOut, LPOVERLAPPED overlapped )
    //   file         - parameter #1: the same pipe handle (hPipe) just
    //                  written to.
    //   buffer       - parameter #2: destination for the server's reply
    //                  (buffer, a local 512-byte array).
    //   count        - parameter #3: max bytes to read, sizeof(buffer) - 1
    //                  (room reserved for a manual '\0').
    //   bytesReadOut - parameter #4: out-parameter; bytes actually read
    //                  (&bytesRead).
    //   overlapped   - parameter #5: nullptr, synchronous mode - this call
    //                  BLOCKS until the server's WriteFile (its "Server
    //                  ACK: ..." reply) arrives.
    BOOL r = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);

    if (r && bytesRead > 0) {

        buffer[bytesRead] = '\0';

        std::cout << "[Client] Received: " << buffer << "\n";

    } else {

        std::cerr << "ReadFile failed or no data: " << GetLastError() << "\n";

    }
 
    // --- Added explanation: CloseHandle(hPipe) ----------------------------------
    // Generic syntax:  CloseHandle( HANDLE object )
    //   object - parameter #1 (only parameter): the pipe HANDLE to release
    //            back to the OS (hPipe). This is the client's side of
    //            disconnecting - functionally the counterpart to the
    //            server's DisconnectNamedPipe + CloseHandle pair.
    CloseHandle(hPipe);

    std::cout << "Client finished.\n";

    return 0;

}

// g++ pipe_server_win.cpp -o pipe_server_win.exe

// g++ pipe_client_win.cpp -o pipe_client_win.exe
 
// pipe_client_win.exe "Hello server!"
 
 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() reads argv[1] (if provided) into `message`, e.g. running
//     `pipe_client_win.exe "Hi server!"` sets message = "Hi server!"; with
//     no argument it defaults to "Hello from client".
// #2  The for-loop (up to 5 attempts) calls CreateFileA on
//     \\.\pipe\DemoPipe:
//       #2a If the server has an instance created AND waiting in
//           ConnectNamedPipe, this succeeds immediately and hPipe becomes a
//           valid, connected HANDLE - the loop breaks right away.
//       #2b If the server exists but its one pipe instance is already busy
//           with another client, CreateFileA fails with ERROR_PIPE_BUSY, so
//           WaitNamedPipeA blocks (up to 2 seconds) for an instance to free
//           up, then the loop retries CreateFileA.
//       #2c If the server hasn't even created a pipe instance yet (e.g. it
//           just hasn't started, or this ran before it), CreateFileA fails
//           with some other error, so the code Sleep(200)s and retries.
// #3  If all 5 attempts fail, hPipe stays INVALID_HANDLE_VALUE and the
//     program prints an error and returns 1 - no data is exchanged.
// #4  Once connected: WriteFile sends `message`'s bytes through hPipe into
//     the pipe. On the server side (3_pipe_server_win.cpp), its blocked
//     ReadFile call receives exactly these bytes.
// #5  ReadFile then BLOCKS this client, waiting for the server's reply. The
//     server builds "Server ACK: <message>" and WriteFiles it back over the
//     same pipe handle; once that arrives, this client's ReadFile unblocks,
//     the buffer is null-terminated, and
//     "[Client] Received: Server ACK: <message>" is printed.
// #6  CloseHandle(hPipe) disconnects this client from the pipe (on the
//     server side, this is what makes its FlushFileBuffers/DisconnectNamedPipe
//     sequence complete for that instance).
// #7  "Client finished." is printed and main() returns 0. Net data flow for
//     one run of this program: exactly one request goes client -> pipe ->
//     server, and exactly one reply comes back server -> pipe -> client.
// ============================================================================
 