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
 
int main(int argc, char* argv[]) {

    const char* pipeName = R"(\\.\pipe\DemoPipe)";

    std::string message = (argc >= 2) ? argv[1] : "Hello from client";
 
    std::cout << "Connecting to pipe: " << pipeName << " ...\n";
 
    // Try to open the pipe (wait up to a bit)

    // If server not ready, Sleep/retry a few times.

    HANDLE hPipe = INVALID_HANDLE_VALUE;

    for (int i = 0; i < 5; ++i) {

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

            if (!WaitNamedPipeA(pipeName, 2000)) {

                std::cerr << "WaitNamedPipe timeout\n";

                return 1;

            }

        } else {

            // Sleep and retry

            Sleep(200);

        }

    }
 
    if (hPipe == INVALID_HANDLE_VALUE) {

        std::cerr << "Failed to open pipe: " << GetLastError() << "\n";

        return 1;

    }
 
    std::cout << "Connected. Sending message: " << message << "\n";
 
    DWORD bytesWritten = 0;

    BOOL w = WriteFile(hPipe, message.c_str(), (DWORD)message.size(), &bytesWritten, nullptr);

    if (!w) {

        std::cerr << "WriteFile failed: " << GetLastError() << "\n";

        CloseHandle(hPipe);

        return 1;

    }
 
    // Read reply

    char buffer[512];

    DWORD bytesRead = 0;

    BOOL r = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);

    if (r && bytesRead > 0) {

        buffer[bytesRead] = '\0';

        std::cout << "[Client] Received: " << buffer << "\n";

    } else {

        std::cerr << "ReadFile failed or no data: " << GetLastError() << "\n";

    }
 
    CloseHandle(hPipe);

    std::cout << "Client finished.\n";

    return 0;

}

// g++ pipe_server_win.cpp -o pipe_server_win.exe

// g++ pipe_client_win.cpp -o pipe_client_win.exe
 
// pipe_client_win.exe "Hello server!"
 
 