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
 
void stopper_thread() {

    std::string s;

    std::getline(std::cin, s);

    running = false;

}
 
int main() {

    const char* pipeName = R"(\\.\pipe\DemoPipe)";

    std::cout << "Named-pipe server starting. Pipe name: " << pipeName << "\n";

    std::cout << "Press ENTER to stop the server.\n";
 
    std::thread stopper(stopper_thread);
 
    while (running) {

        // Create a named pipe instance (duplex)

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

        BOOL ok = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);

        if (ok && bytesRead > 0) {

            buffer[bytesRead] = '\0';

            std::cout << "[Server] Received: " << buffer << "\n";
 
            // Send reply

            std::string reply = std::string("Server ACK: ") + buffer;

            DWORD bytesWritten = 0;

            BOOL w = WriteFile(hPipe, reply.c_str(), (DWORD)reply.size(), &bytesWritten, nullptr);

            if (!w) {

                std::cerr << "[Server] WriteFile failed: " << GetLastError() << "\n";

            }

        } else {

            std::cerr << "[Server] ReadFile failed or no data: " << GetLastError() << "\n";

        }
 
        // Disconnect & close this pipe instance; loop to accept another client

        FlushFileBuffers(hPipe);

        DisconnectNamedPipe(hPipe);

        CloseHandle(hPipe);

        std::cout << "Client handled, pipe closed.\n\n";

    }
 
    if (stopper.joinable()) stopper.join();

    std::cout << "Server stopping.\n";

    return 0;

}

 