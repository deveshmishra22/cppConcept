#include <iostream>
#include <fcntl.h>   // for _open(), _O_CREAT, _O_WRONLY, etc.
#include <io.h>      // for _write(), _close()
#include <sys/stat.h> // for _S_IREAD, _S_IWRITE
 
int main() {

    // Open a file (Windows version uses underscore-prefixed functions)
    int fd = _open("example.txt", _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (fd == -1) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }
 
    std::cout << "Opened 'example.txt' with file descriptor: " << fd << "\n";
 
    // Write to the file
    const char* message = "Hello from file descriptor!\n";
    _write(fd, message, 30);
 
    // Write to STDOUT (file descriptor = 1)
    _write(1, "This goes to STDOUT\n", 21);
 
    // Close the file
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
