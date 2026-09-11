// //What is pipe in C++?
// //A pipe in C++ is a unidirectional communication channel that allows data to be transferred from one process to another.   
// //Pipes are commonly used in inter-process communication (IPC) to enable processes to exchange data in a synchronized manner.
// //In C++, pipes can be created using system calls provided by the operating system, such as
// //pipe() on Unix-like systems or CreatePipe() on Windows.

// What does it mean Inter-process communication (IPC)?
// Inter-process communication (IPC) refers to the mechanisms and techniques that allow different processes to communicate and share data with each other.
// IPC is essential in modern operating systems, where multiple processes often need to collaborate or exchange information to perform tasks efficiently.
// IPC can be achieved through various methods, including pipes, message queues, shared memory, sockets, and remote procedure calls (RPC).
// for example, pipes provide a simple way for one process to send data to another process in a unidirectional manner, while sockets enable communication between processes over a network.
// like process A (sender: 127.0.0.1:9001) sends data to process B (receiver: 127.0.0.1:9002) using a pipe, and process B reads that data from the pipe for further processing.


// //How to use pipe in C++?
// //To use pipes in C++, you typically follow these steps:
// //1. Create a pipe using the appropriate system call. This will give you two file descriptors: one for reading and one for writing.
// //2. Fork a new process using the fork() system call (on Unix-like systems). The child process will inherit the file descriptors of the parent process.
// //3. In the parent process, close the read end of the pipe and write data to the write end.
// //4. In the child process, close the write end of the pipe and read data from the read end.
// //5. Close the pipe file descriptors when done to free up resources.    

// // How it is different from file streams or Sockets or communication between threads?
// //Pipes are specifically designed for inter-process communication (IPC) and provide a simple way for processes to exchange data.
// //File streams (fstream) are higher-level abstractions for file I/O operations and are not primarily intended for IPC.
// //Sockets are more versatile and can be used for communication between processes on different machines over a network.
// //Communication between threads typically uses shared memory or synchronization primitives like mutexes and condition variables, rather than pipes.

// #include <iostream>
// #include <fcntl.h>   // for _open(), _O_CREAT, _O_WRONLY, etc.
// #include <io.h>      // for _write(), _close()
// #include <sys/stat.h> // for _S_IREAD, _S_IWRITE
// #include <windows.h> // for Windows pipe functions

// using namespace std;

// // Question statement: Parent creates an anonymous pipe with CreatePipe.
// // Parent marks the write handle non-inheritable so the child only gets the read end.
// // Parent prepares STARTUPINFO with hStdInput = hRead and calls CreateProcess with bInheritHandles = TRUE. The child process will inherit the read handle and have it as its standard input.
// // Parent writes a message with WriteFile to the write end, then closes it (this signals EOF to the child).
// // Child (same program launched with child arg) reads from std::cin (its STDIN) and prints the message.

// void childProcess() {
//     // Child process reads from standard input (which is the read end of the pipe)
//     char buffer[128];
//     cin.getline(buffer, sizeof(buffer));
//     cout << "Child process received: " << buffer << endl;
// }

// void parentProcess(HANDLE hWritePipe) {
//     const char* message = "Hello from parent process via pipe!";
//     DWORD bytesWritten;
//     WriteFile(hWritePipe, message, strlen(message), &bytesWritten, NULL);
//     cout << "Parent process sent: " << message << endl;
//     CloseHandle(hWritePipe); // Close the write end to signal EOF to child
// }

// int main()
// {

//     // Check if this is the child process
//     if (__argc > 1 && string(__argv[1]) == "child") 
//     {
//         childProcess();
//         return 0;
//     }
//     // Parent process
//     HANDLE hReadPipe, hWritePipe;
//     SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
//     // Create a pipe
//     if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
//         cerr << "Failed to create pipe." << endl;
//         return 1;
//     }
//     // Ensure the write handle to the pipe is not inherited
//     SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT, 0);
//     // Set up STARTUPINFO for the child process
//     STARTUPINFO si = { sizeof(STARTUPINFO) };
//     si.dwFlags = STARTF_USESTDHANDLES;
//     si.hStdInput = hReadPipe; // Child's standard input is the read end of the pipe
//     si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
//     si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
//     PROCESS_INFORMATION pi;
//     // Create the child process
//     if (!CreateProcess(NULL, const_cast<LPSTR>("pipe.exe child"), NULL,
//         NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
//         cerr << "Failed to create child process." << endl;
//         return 1;
//     }
//     CloseHandle(hReadPipe); // Parent doesn't need the read end
//     parentProcess(hWritePipe);
//     // Wait for child process to finish
//     WaitForSingleObject(pi.hProcess, INFINITE);
//     CloseHandle(pi.hProcess);
//     CloseHandle(pi.hThread);
//     // The following code is an alternative simple pipe example (commented out)

//     // HANDLE hReadPipe, hWritePipe;
//     // SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

//     // // Create a pipe
//     // if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
//     //     cerr << "Failed to create pipe." << endl;
//     //     return 1;
//     // }

//     // cout << "Pipe created successfully." << endl;

//     // // Write to the pipe
//     // const char* message = "Hello from pipe!\n";
//     // DWORD bytesWritten;
//     // WriteFile(hWritePipe, message, strlen(message), &bytesWritten, NULL);
//     // cout << "Written to pipe: " << message;

//     // // Read from the pipe
//     // char buffer[128];
//     // DWORD bytesRead;
//     // ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
//     // buffer[bytesRead] = '\0'; // Null-terminate the string
//     // cout << "Read from pipe: " << buffer;

//     // // Close the pipe handles
//     // CloseHandle(hReadPipe);
//     // CloseHandle(hWritePipe);
//     // cout << "Pipe closed." << endl;

//     return 0;
// }


// file: pipe_example_windows.cpp

// Compile (MSVC):

//    cl /EHsc pipe_example_windows.cpp

// Or MinGW:

//    g++ -std=c++11 pipe_example_windows.cpp -o pipe_example_windows.exe
 
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
 
// --- Added explanation: Fail(msg) function definition -----------------------
// Generic syntax:  void Fail( const char* msg )
//   msg - parameter #1 (only parameter): a short description of what just
//         failed (e.g. "CreatePipe failed"). This helper prints that message
//         together with the last Windows API error code (GetLastError()) and
//         then terminates the whole process via ExitProcess(1) - it never
//         returns, so every call site below effectively acts like a fatal
//         "abort with message" statement.
void Fail(const char* msg) {
    std::cerr << msg << " (err=" << GetLastError() << ")\n";
    ExitProcess(1);
}
 
// --- Added explanation: child_main() function definition --------------------
// Generic syntax:  int child_main()
//   (no parameters) - this process re-invokes itself with a "child" argument
//   (see the CreateProcessA call below) so the SAME executable can act as
//   either the parent or the child depending on argv[1]. When acting as the
//   child, main() forwards here immediately.
int child_main() {
    // Child: read from STDIN (redirected by the parent)
    std::string line;

    // Use std::getline so it works with text stream
    if (std::getline(std::cin, line)) {
        std::cout << "[Child] Received message: " << line << std::endl;
    } else {
        std::cerr << "[Child] No data read from stdin.\n";
    }

    return 0;
}
 
// --- Added explanation: main(argc, argv) entry point -------------------------
// Generic syntax:  int main( int argc, char* argv[] )
//   argc - parameter #1: number of command-line arguments, supplied by the
//          OS/CRT (>= 1; argv[0] is always the program's own path).
//   argv - parameter #2: the arguments themselves. This program checks
//          argv[1] for the literal string "child" to decide whether THIS
//          run of the executable should behave as the parent (creates the
//          pipe and spawns a child) or as the child (reads from the
//          inherited pipe via stdin) - see CreateProcessA below, which
//          launches this same .exe again with "child" appended.
int main(int argc, char* argv[]) {

    // If launched with "child" argument, act as child
    if (argc >= 2 && std::string(argv[1]) == "child") {
        return child_main();
    }
 
    // Parent process: create pipe and spawn child process with read end as child's STDIN
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE; // pipe handles are inheritable
    HANDLE hRead = nullptr;
    HANDLE hWrite = nullptr;
 
    // --- Added explanation: CreatePipe(&hRead, &hWrite, &sa, 0) -----------------
    // Generic syntax:  CreatePipe( PHANDLE readHandle, PHANDLE writeHandle,
    //                              LPSECURITY_ATTRIBUTES attrs, DWORD bufSize )
    //   readHandle  - parameter #1: out-parameter; receives the new HANDLE
    //                 for the pipe's READ end (&hRead, so hRead is filled in).
    //   writeHandle - parameter #2: out-parameter; receives the new HANDLE
    //                 for the pipe's WRITE end (&hWrite).
    //   attrs       - parameter #3: security attributes for the two handles
    //                 (&sa, here with bInheritHandle = TRUE so BOTH handles
    //                 start out inheritable by a child process).
    //   bufSize     - parameter #4: requested pipe buffer size in bytes; 0
    //                 means "use the system's default size".
    //   returns       nonzero on success; on failure it returns 0/FALSE, in
    //                 which case Fail() (below) is called and the process
    //                 exits.
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        // --- Added explanation: Fail("CreatePipe failed") -----------------------
        // Generic syntax:  Fail( const char* msg )
        //   msg - parameter #1: "CreatePipe failed" - printed with the
        //         Windows error code, then the process exits (see Fail's
        //         definition above).
        Fail("CreatePipe failed");
    }
 
    // Ensure the write handle is NOT inherited by the child.
    // We want the child to inherit only the read end (so parent's write won't be duplicated).

    // --- Added explanation: SetHandleInformation(hWrite, flag, mask) -----------
    // Generic syntax:  SetHandleInformation( HANDLE obj, DWORD mask, DWORD flags )
    //   obj   - parameter #1: the handle to modify (hWrite, the pipe's write
    //           end, which CreatePipe made inheritable above).
    //   mask  - parameter #2: which bit(s) of the handle's inheritance info
    //           to change (HANDLE_FLAG_INHERIT - the "is this inheritable by
    //           child processes" bit).
    //   flags - parameter #3: the new value for that bit (0 = turn
    //           inheritance OFF). Net effect: only hRead stays inheritable,
    //           so the soon-to-be-spawned child process will inherit the
    //           READ end but NOT this WRITE end.
    if (!SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0)) {
        // --- Added explanation: Fail("SetHandleInformation failed") -------------
        // Generic syntax:  same as the Fail() call explained above; msg here
        // is "SetHandleInformation failed".
        Fail("SetHandleInformation failed");
    }
 
    // Prepare command line for child: same executable + "child"
    // Get current module filename

    char exePath[MAX_PATH];
    // --- Added explanation: GetModuleFileNameA(nullptr, exePath, MAX_PATH) -----
    // Generic syntax:  GetModuleFileNameA( HMODULE module, LPSTR buffer, DWORD size )
    //   module - parameter #1: which loaded module's path to fetch; nullptr
    //            means "the current executable itself" rather than some DLL.
    //   buffer - parameter #2: destination char array to receive the full
    //            path (exePath).
    //   size   - parameter #3: capacity of that buffer, so the API never
    //            writes past it (MAX_PATH).
    //   This is how the parent discovers its own .exe path, so it can launch
    //   ANOTHER copy of itself (as the child) via CreateProcessA below.
    if (!GetModuleFileNameA(nullptr, exePath, MAX_PATH)) {
        // --- Added explanation: Fail("GetModuleFileNameA failed") ---------------
        // Generic syntax:  same as the Fail() call explained above; msg here
        // is "GetModuleFileNameA failed".
        Fail("GetModuleFileNameA failed");
    }

    std::string cmd = std::string("\"") + exePath + "\" child";
 
    // Setup STARTUPINFO to redirect child's STDIN to pipe read end
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hRead;      // child's STDIN will come from the pipe read end
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); // inherit parent's stdout/stderr (optional)
    si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
 
    // Create child process (inherit handles must be TRUE)
    // --- Added explanation: CreateProcessA(...) ---------------------------------
    // Generic syntax:  CreateProcessA( LPCSTR appName, LPSTR cmdLine,
    //     LPSECURITY_ATTRIBUTES procAttrs, LPSECURITY_ATTRIBUTES threadAttrs,
    //     BOOL inheritHandles, DWORD creationFlags, LPVOID env, LPCSTR curDir,
    //     LPSTARTUPINFOA startupInfo, LPPROCESS_INFORMATION procInfo )
    //   Each parameter is already labeled by the inline comment beside it
    //   below; the ones that matter most for this pipe demo are:
    //     cmdLine (&cmd[0])      - the same .exe path plus " child", so the
    //                              new process re-runs this program in
    //                              "child" mode.
    //     inheritHandles (TRUE)  - REQUIRED for the child to actually
    //                              receive hRead; without TRUE, none of this
    //                              process's inheritable handles would cross
    //                              over, even though hRead was marked
    //                              inheritable back when CreatePipe made it.
    //     startupInfo (&si)      - si.hStdInput was set to hRead above, so
    //                              the child's STDIN (std::cin) becomes the
    //                              pipe's read end.
    //     procInfo (&pi)         - out-parameter; filled in with handles to
    //                              the new process and its main thread
    //                              (pi.hProcess, pi.hThread), used later to
    //                              wait for it and clean up.
    //   returns  nonzero/TRUE on success.
    BOOL ok = CreateProcessA(
        nullptr,                       // lpApplicationName
        &cmd[0],                       // lpCommandLine (writable)
        nullptr,                       // lpProcessAttributes
        nullptr,                       // lpThreadAttributes
        TRUE,                          // bInheritHandles -> child inherits hRead
        0,                             // dwCreationFlags
        nullptr,                       // lpEnvironment
        nullptr,                       // lpCurrentDirectory
        &si,                           // lpStartupInfo
        &pi                            // lpProcessInformation

    );
 
    if (!ok) {

        // clean up handles

        // --- Added explanation: CloseHandle(hRead) ------------------------------
        // Generic syntax:  CloseHandle( HANDLE object )
        //   object - parameter #1 (only parameter): the open HANDLE to
        //            release back to the OS (hRead, the pipe's read end,
        //            here). This is the Windows-HANDLE equivalent of POSIX
        //            close(fd) - after this call, hRead must not be used
        //            again. Every other CloseHandle(...) call further below
        //            in this file follows this exact same one-parameter
        //            pattern, just closing a different handle.
        CloseHandle(hRead);

        // --- Added explanation: CloseHandle(hWrite) -----------------------------
        // Same as CloseHandle(hRead) explained just above, but releasing the
        // pipe's WRITE end instead - both handles must be cleaned up since
        // CreateProcessA failed and no child will ever use them.
        CloseHandle(hWrite);

        // --- Added explanation: Fail("CreateProcess failed") --------------------
        // Generic syntax:  same as the Fail() call explained above; msg here
        // is "CreateProcess failed". Reports the failure and exits.
        Fail("CreateProcess failed");

    }
 
    // Parent no longer needs the read end

    // --- Added explanation: CloseHandle(hRead) ------------------------------
    // Same as explained above; the child inherited its OWN copy of hRead
    // when CreateProcessA ran, so the parent closing its copy here does NOT
    // affect the child's ability to read from the pipe.
    CloseHandle(hRead);
 
    // Write message into the pipe (child will read it from its stdin)

    const char* msg = "Hello from parent via pipe!\n";

    DWORD written = 0;

    // --- Added explanation: WriteFile(hWrite, msg, count, &written, nullptr) ---
    // Generic syntax:  WriteFile( HANDLE file, LPCVOID buffer, DWORD count,
    //                             LPDWORD writtenOut, LPOVERLAPPED overlapped )
    //   file       - parameter #1: the HANDLE to write to (hWrite, the
    //                pipe's write end).
    //   buffer     - parameter #2: pointer to the bytes to send (msg,
    //                "Hello from parent via pipe!\n").
    //   count      - parameter #3: how many bytes to write, from
    //                strlen(msg), cast to DWORD.
    //   writtenOut - parameter #4: out-parameter; receives how many bytes
    //                were actually written (&written).
    //   overlapped - parameter #5: nullptr here means synchronous
    //                (blocking) I/O rather than async overlapped I/O.
    //   This is the Windows-HANDLE equivalent of POSIX write(fd, ...); the
    //   bytes land in the pipe's kernel buffer, ready for the child process
    //   to read via its inherited stdin (see child_main() above).
    BOOL wok = WriteFile(hWrite, msg, (DWORD)strlen(msg), &written, nullptr);

    if (!wok) {

        std::cerr << "WriteFile failed (err=" << GetLastError() << ")\n";

    } else {

        std::cout << "[Parent] Wrote " << written << " bytes to pipe.\n";

    }
 
    // Close the write end to signal EOF to child

    // --- Added explanation: CloseHandle(hWrite) -----------------------------
    // Same as explained above, closing the write end this time. This is the
    // crucial step that lets the child's std::getline(std::cin, ...) finish:
    // getline blocks until it sees a newline OR end-of-stream, and closing
    // the last write handle to the pipe is what produces that end-of-stream
    // signal on the child's read side.
    CloseHandle(hWrite);
 
    // Wait for child to exit

    // --- Added explanation: WaitForSingleObject(pi.hProcess, INFINITE) ---------
    // Generic syntax:  WaitForSingleObject( HANDLE object, DWORD timeoutMs )
    //   object    - parameter #1: the handle to wait on (pi.hProcess, the
    //               child process created by CreateProcessA above). A
    //               process handle becomes "signaled" when that process
    //               exits.
    //   timeoutMs - parameter #2: how long to wait, in milliseconds;
    //               INFINITE means block forever until the child exits (no
    //               timeout).
    //   This blocks the parent here until the child process has fully
    //   finished running (i.e. until child_main() has returned).
    WaitForSingleObject(pi.hProcess, INFINITE);
 
    // Get child's exit code (optional)

    DWORD exitCode = 0;

    // --- Added explanation: GetExitCodeProcess(pi.hProcess, &exitCode) ---------
    // Generic syntax:  GetExitCodeProcess( HANDLE process, LPDWORD exitCodeOut )
    //   process     - parameter #1: the (now-exited) child process handle
    //                 (pi.hProcess).
    //   exitCodeOut - parameter #2: out-parameter; receives the child's exit
    //                 code (&exitCode) - here, whatever child_main() returned
    //                 (0 on the normal path).
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {

        std::cout << "[Parent] Child exited with code: " << exitCode << std::endl;

    }
 
    // Cleanup

    // --- Added explanation: CloseHandle(pi.hProcess) ------------------------
    // Same pattern as every CloseHandle(...) call explained above, this time
    // releasing the child PROCESS handle produced by CreateProcessA (the
    // child itself has already exited by this point; this just frees the
    // parent's HANDLE bookkeeping for it).
    CloseHandle(pi.hProcess);

    // --- Added explanation: CloseHandle(pi.hThread) -------------------------
    // Same pattern again, releasing the handle to the child's main THREAD
    // (also produced by CreateProcessA). Every resource CreateProcessA
    // handed back (hProcess, hThread) must be closed once no longer needed,
    // just like the pipe handles were.
    CloseHandle(pi.hThread);

    return 0;

}

 
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  The compiled executable is run with NO arguments, so main()'s check
//     `argc >= 2 && argv[1] == "child"` is false, and execution falls
//     through into the PARENT path.
// #2  CreatePipe(&hRead, &hWrite, &sa, 0) creates one unnamed, unidirectional
//     pipe inside the OS kernel and returns two HANDLEs: hRead (read end)
//     and hWrite (write end). Because sa.bInheritHandle == TRUE, BOTH
//     handles start out inheritable by a child process.
// #3  SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0) turns inheritance
//     back OFF for hWrite only, so only hRead will cross over to the child -
//     the parent keeps sole ownership of the write side.
// #4  GetModuleFileNameA fetches this program's own .exe path into exePath,
//     and cmd is built as "<exePath>" child - the exact command line used to
//     re-launch this same program in child mode.
// #5  STARTUPINFOA si is configured with si.hStdInput = hRead, so whatever
//     process CreateProcessA launches will have hRead attached as its
//     standard input (fd 0 / std::cin) instead of the console.
// #6  CreateProcessA(..., TRUE, ...) launches a NEW instance of this same
//     executable, passing "child" as argv[1] and inheriting hRead as that
//     new process's stdin. At this point there are TWO processes running:
//     the original (now the "parent") and the new one (the "child").
// #7  Back in the parent: CloseHandle(hRead) drops the parent's own copy of
//     the read handle (the child has its own inherited copy, so the pipe
//     stays open on the read side).
// #8  WriteFile(hWrite, msg, strlen(msg), &written, nullptr) copies
//     "Hello from parent via pipe!\n" into the pipe's kernel buffer. This
//     data is now available for the child to read via its stdin - the
//     parent does NOT wait for the child to actually read it before
//     continuing.
// #9  Meanwhile, in the CHILD process: main() sees argv[1] == "child" and
//     immediately calls child_main(), which calls
//     std::getline(std::cin, line). Because the child's stdin IS the pipe's
//     read end, this call blocks until either a newline arrives through the
//     pipe or the pipe's write end is closed (EOF). Once the parent's
//     WriteFile() above lands, the message (ending in '\n') satisfies
//     getline, so the child prints
//     "[Child] Received message: Hello from parent via pipe!" and returns 0
//     from child_main(), ending the child process.
// #10 Back in the parent: CloseHandle(hWrite) closes the write end. If the
//     child's getline() call were still blocked waiting for more input,
//     THIS is the call that would unblock it by signaling end-of-stream -
//     in this particular run the child likely already finished after
//     receiving the newline-terminated message, but closing the write end
//     is still required so the pipe's resources are fully released.
// #11 WaitForSingleObject(pi.hProcess, INFINITE) blocks the parent until the
//     child process has fully exited.
// #12 GetExitCodeProcess retrieves the child's exit code (0, from
//     child_main()'s `return 0;`), and the parent prints
//     "[Parent] Child exited with code: 0".
// #13 CloseHandle(pi.hProcess) and CloseHandle(pi.hThread) release the
//     parent's remaining handles to the (now-finished) child process and
//     its main thread.
// #14 main() returns 0, ending the parent process. Net data flow: one
//     message traveled one-way, parent -> pipe -> child, exactly once.
// ============================================================================