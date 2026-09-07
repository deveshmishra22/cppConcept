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
 
void Fail(const char* msg) {
    std::cerr << msg << " (err=" << GetLastError() << ")\n";
    ExitProcess(1);
}
 
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
 
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        Fail("CreatePipe failed");
    }
 
    // Ensure the write handle is NOT inherited by the child.
    // We want the child to inherit only the read end (so parent's write won't be duplicated).

    if (!SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0)) {
        Fail("SetHandleInformation failed");
    }
 
    // Prepare command line for child: same executable + "child"
    // Get current module filename

    char exePath[MAX_PATH];
    if (!GetModuleFileNameA(nullptr, exePath, MAX_PATH)) {
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

        CloseHandle(hRead);

        CloseHandle(hWrite);

        Fail("CreateProcess failed");

    }
 
    // Parent no longer needs the read end

    CloseHandle(hRead);
 
    // Write message into the pipe (child will read it from its stdin)

    const char* msg = "Hello from parent via pipe!\n";

    DWORD written = 0;

    BOOL wok = WriteFile(hWrite, msg, (DWORD)strlen(msg), &written, nullptr);

    if (!wok) {

        std::cerr << "WriteFile failed (err=" << GetLastError() << ")\n";

    } else {

        std::cout << "[Parent] Wrote " << written << " bytes to pipe.\n";

    }
 
    // Close the write end to signal EOF to child

    CloseHandle(hWrite);
 
    // Wait for child to exit

    WaitForSingleObject(pi.hProcess, INFINITE);
 
    // Get child's exit code (optional)

    DWORD exitCode = 0;

    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {

        std::cout << "[Parent] Child exited with code: " << exitCode << std::endl;

    }
 
    // Cleanup

    CloseHandle(pi.hProcess);

    CloseHandle(pi.hThread);

    return 0;

}

 