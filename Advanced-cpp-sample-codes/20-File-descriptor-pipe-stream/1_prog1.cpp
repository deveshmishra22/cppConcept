#include <iostream>
#include <cstring>   // for strlen()
#include <fcntl.h>   // for _open(), _O_CREAT, _O_WRONLY, etc.
#include <io.h>      // for _write(), _close()
#include <sys/stat.h> // for _S_IREAD, _S_IWRITE
 
int main() {

    // Open a file (Windows version uses underscore-prefixed functions)
    // --- Added explanation: _open(path, oflag, pmode) --------------------------
    // Generic syntax:  _open( const char* path, int oflag, int pmode )
    //   path  - parameter #1: filesystem path of the file to open/create
    //           ("example.txt", here - created in the current working
    //           directory).
    //   oflag - parameter #2: bitwise-OR'd flags controlling how the file is
    //           opened (_O_CREAT | _O_WRONLY | _O_TRUNC, here - create the
    //           file if it doesn't already exist, open it write-only, and
    //           truncate it to zero length if it does already exist).
    //   pmode - parameter #3: permission bits applied ONLY when _O_CREAT
    //           actually creates a brand-new file (_S_IREAD | _S_IWRITE,
    //           here - readable and writable).
    //   returns  a new file descriptor (a small non-negative int) on
    //           success, or -1 on failure (checked immediately below).
    int fd = _open("example.txt", _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (fd == -1) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }
 
    std::cout << "Opened 'example.txt' with file descriptor: " << fd << "\n";
 
    // Write to the file
    const char* message = "Hello from file descriptor!\n";
    // --- Added explanation: _write(fd, buffer, count) ---------------------------
    // Generic syntax:  _write( int fd, const void* buffer, unsigned int count )
    //   fd     - parameter #1: the file descriptor to write to (fd, the
    //            descriptor returned by _open() above, i.e. the open
    //            "example.txt" file).
    //   buffer - parameter #2: pointer to the bytes to write (message, the
    //            C-string "Hello from file descriptor!\n").
    //   count  - parameter #3: how many bytes to write; computed here via
    //            strlen(message) (the string's length, not counting the
    //            trailing '\0'), then cast to unsigned as _write() expects.
    _write(fd, message, (unsigned)strlen(message));
 
    // Write to STDOUT (file descriptor = 1)
    const char* stdoutMsg = "This goes to STDOUT\n";
    // --- Added explanation: _write(1, buffer, count) -----------------------------
    // Generic syntax:  _write( int fd, const void* buffer, unsigned int count )
    //   fd (1) - parameter #1: writes directly to file descriptor 1, which is
    //            always STDOUT for this process - bypassing std::cout
    //            entirely, this text still lands on the console (or wherever
    //            STDOUT has been redirected).
    //   buffer - parameter #2: pointer to the bytes to write (stdoutMsg,
    //            "This goes to STDOUT\n").
    //   count  - parameter #3: byte count from strlen(stdoutMsg), cast to
    //            unsigned (same treatment as the _write() call above).
    _write(1, stdoutMsg, (unsigned)strlen(stdoutMsg));
 
    // Close the file
    // --- Added explanation: _close(fd) -------------------------------------------
    // Generic syntax:  _close( int fd )
    //   fd - parameter #1 (only parameter): the file descriptor to close
    //        (fd, the "example.txt" descriptor opened above). After this
    //        call returns, fd is no longer valid and the OS is free to reuse
    //        that integer for a future _open()/_pipe() call.
    _close(fd);
    std::cout << "File closed.\n";
    return 0;

}

// What is a file descriptor in C++?
// A file descriptor is a low-level integer handle used to access files or other input/output resources
// It is used by the operating system to identify an open file or I/O resource, allowing programs to read from or write to it.
// In C++, file descriptors are typically used in conjunction with system calls for file operations,
// such as open(), read(), write(), and close().
// How to use file descriptors in C++?
// To use file descriptors in C++, you can use the POSIX system calls provided by the operating system.
// Here are the basic steps to work with file descriptors:
// 1. Open a file using the open() system call, which returns a file descriptor.
// 2. Use the read() and write() system calls to read from or write to the file using the file descriptor.
// 3. Close the file using the close() system call when you are done with it.
// Note: File descriptors are a low-level concept, and in modern C++ programming, it is often recommended to use higher-level abstractions like file streams (fstream) for file operations.

// when to choose file descriptors and when to choose file streams?
// File descriptors are typically used in low-level system programming or when you need fine-grained control
// over file I/O operations, such as in operating system development or network programming.
// File streams (fstream) are higher-level abstractions that provide a more convenient and safer way to work with files in C++.
// They are generally preferred for most application-level programming tasks due to their ease of use and integration with C++ features like exceptions and RAII (Resource Acquisition Is Initialization).
// In summary, use file descriptors for low-level system programming and file streams for general application development.

// Explaination of above code line by line:
// 1. We include necessary headers for file operations and I/O.
// 2. We open a file named "example.txt" using _open() with flags to create, write-only, and truncate the file.
// 3. We check if the file was opened successfully by verifying the returned file descriptor.
// 4. We write a message to the file using _write() and the obtained file descriptor.
// 5. We write a message directly to standard output (STDOUT) using file descriptor 1.
// 6. We close the file using _close() to release the file descriptor.
// 7. Finally, we print messages to indicate the status of file operations.

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() calls _open("example.txt", _O_CREAT | _O_WRONLY | _O_TRUNC,
//     _S_IREAD | _S_IWRITE). This creates example.txt in the current working
//     directory if it doesn't exist (or truncates it to empty if it does),
//     opens it write-only, and returns a small integer file descriptor into
//     `fd` (typically 3, since 0/1/2 are already taken by stdin/stdout/stderr).
// #2  If fd == -1 (open failed - e.g. permissions problem), an error is
//     printed to stderr and the program returns 1 immediately, skipping every
//     step below.
// #3  Assuming success, std::cout prints the value of fd so you can see the
//     descriptor number the OS handed back.
// #4  _write(fd, message, strlen(message)) copies the bytes of
//     "Hello from file descriptor!\n" through the fd into example.txt's
//     underlying OS file buffer. Nothing is printed to the console here - the
//     data goes only into the file, not to STDOUT.
// #5  _write(1, stdoutMsg, strlen(stdoutMsg)) writes "This goes to STDOUT\n"
//     directly to file descriptor 1. Descriptor 1 is always STDOUT for this
//     process, so this text appears on the console - but note it went
//     through the low-level _write() call, not through std::cout, so its
//     buffering/flush timing relative to the std::cout lines above/below is
//     handled independently by the C runtime rather than by iostream.
// #6  _close(fd) closes example.txt: any OS-level buffered data for that
//     descriptor is finalized/flushed to disk, and fd itself becomes invalid
//     (the OS may hand that same integer out again on a future _open()).
// #7  std::cout prints "File closed." and main() returns 0.
// #8  End state: example.txt now exists on disk containing exactly
//     "Hello from file descriptor!\n", while "This goes to STDOUT\n" and
//     "File closed." (plus the earlier "Opened ... with file descriptor: N")
//     were written to the console/terminal, not the file.
// ============================================================================
