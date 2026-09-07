// This program demonstrates how to read system information in C++ using standard libraries.
// what is system information? what is /proc
// System information refers to data about the system's hardware and software configuration, including details about the CPU, memory, disk usage, and running processes.
// The /proc directory is a virtual filesystem in Unix-like operating systems that provides a mechanism for the kernel to expose information about the system and processes to user space.

// On Windows, we can use system calls and libraries to gather similar information.
// This example focuses on Windows using the Windows API.
// Note: This code is specific to Windows. For Linux, you would typically read from /proc files.

#include <iostream>
#include <windows.h>
#include <sysinfoapi.h>
#include <fstream>
#include <string>
using namespace std;

void WindowsSystemInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    cout << "System Information:" << endl;
    cout << "-------------------" << endl;
    cout << "Processor Architecture: " << sysInfo.wProcessorArchitecture << endl;
    cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << endl;
    cout << "Page Size: " << sysInfo.dwPageSize << " bytes" << endl;
    cout << "Minimum Application Address: " << sysInfo.lpMinimumApplicationAddress << endl;
    cout << "Maximum Application Address: " << sysInfo.lpMaximumApplicationAddress << endl;

    // Get memory status
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);

    cout << "\nMemory Information:" << endl;
    cout << "-------------------" << endl;
    cout << "Total Physical Memory: " << memStatus.ullTotalPhys / (1024 * 1024) << " MB" << endl;
    cout << "Available Physical Memory: " << memStatus.ullAvailPhys / (1024 * 1024) << " MB" << endl;
    cout << "Total Virtual Memory: " << memStatus.ullTotalVirtual / (1024 * 1024) << " MB" << endl;
    cout << "Available Virtual Memory: " << memStatus.ullAvailVirtual / (1024 * 1024) << " MB" << endl;

}

void read_cpu_info() {
    ifstream cpuFile("/proc/cpuinfo");
    if (!cpuFile) {
        cerr << "Failed to open /proc/cpuinfo\n";
        return;
    }

    string line;
    cout << "=== CPU INFO ===\n";
    int count = 0;
    while (getline(cpuFile, line)) {
        if (line.find("model name") != string::npos) {
            cout << line << endl;
            count++;
        }
    }
    cout << "Total CPUs: " << count << "\n\n";
}
 
void read_mem_info() {
    ifstream memFile("/proc/meminfo");
    if (!memFile) {
        cerr << "Failed to open /proc/meminfo\n";
        return;
    }

    string line;
    cout << "=== MEMORY INFO ===\n";
    for (int i = 0; i < 3 && getline(memFile, line); ++i) {
        cout << line << endl;
    }
}

int main() {
    WindowsSystemInfo();
    read_cpu_info();
    read_mem_info();
    return 0;
}
