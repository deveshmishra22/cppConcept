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
    // --- Added explanation: GetSystemInfo(&sysInfo) --------------------------
    // Generic syntax:  GetSystemInfo( LPSYSTEM_INFO lpSystemInfo )
    //   lpSystemInfo - parameter #1 (only parameter): address of a
    //                  SYSTEM_INFO struct (&sysInfo, here) that this
    //                  function fills in with the result; the caller reads
    //                  the fields (wProcessorArchitecture,
    //                  dwNumberOfProcessors, dwPageSize,
    //                  lpMinimumApplicationAddress,
    //                  lpMaximumApplicationAddress, etc.) afterward.
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
    // --- Added explanation: GlobalMemoryStatusEx(&memStatus) -----------------
    // Generic syntax:  GlobalMemoryStatusEx( LPMEMORYSTATUSEX lpBuffer )
    //   lpBuffer - parameter #1 (only parameter): address of a
    //              MEMORYSTATUSEX struct (&memStatus, here) that this
    //              function fills in with the result; the caller reads the
    //              fields (ullTotalPhys, ullAvailPhys, ullTotalVirtual,
    //              ullAvailVirtual, etc.) afterward. Note the line right
    //              above this call: memStatus.dwLength must be set to
    //              sizeof(memStatus) BEFORE calling, so the function knows
    //              which struct version/size it is filling in.
    GlobalMemoryStatusEx(&memStatus);

    cout << "\nMemory Information:" << endl;
    cout << "-------------------" << endl;
    cout << "Total Physical Memory: " << memStatus.ullTotalPhys / (1024 * 1024) << " MB" << endl;
    cout << "Available Physical Memory: " << memStatus.ullAvailPhys / (1024 * 1024) << " MB" << endl;
    cout << "Total Virtual Memory: " << memStatus.ullTotalVirtual / (1024 * 1024) << " MB" << endl;
    cout << "Available Virtual Memory: " << memStatus.ullAvailVirtual / (1024 * 1024) << " MB" << endl;

}

void read_cpu_info() {
    // --- Added explanation: ifstream cpuFile("/proc/cpuinfo") ---------------
    // Generic syntax:  std::ifstream variableName( const char* filename )
    //   filename - parameter #1 (only parameter): path of the file to open
    //              for reading ("/proc/cpuinfo", here). On Windows this
    //              path does not exist, so the open fails and cpuFile ends
    //              up in a failed state (see the `if (!cpuFile)` check
    //              right below).
    ifstream cpuFile("/proc/cpuinfo");
    if (!cpuFile) {
        cerr << "Failed to open /proc/cpuinfo\n";
        return;
    }

    string line;
    cout << "=== CPU INFO ===\n";
    int count = 0;
    // --- Added explanation: getline(cpuFile, line) ---------------------------
    // Generic syntax:  std::getline( istream& is, string& str )
    //   is  - parameter #1: the input stream to read the next line from
    //         (cpuFile, here).
    //   str - parameter #2: the string that receives the line's text
    //         (line, here), overwriting whatever it held before. The call
    //         also returns the stream itself, and that return value is
    //         what the while-loop tests: it evaluates to false once
    //         cpuFile has no more lines (or was never opened), ending the
    //         loop.
    while (getline(cpuFile, line)) {
        // --- Added explanation: line.find("model name") ----------------------
        // Generic syntax:  str.find( const string& substr )
        //   substr - parameter #1 (only parameter): the text to search for
        //            inside `line` ("model name", here). Returns the index
        //            where the match starts, or string::npos if it is not
        //            found at all - so this condition is true only for
        //            lines that mention a CPU's model name.
        if (line.find("model name") != string::npos) {
            cout << line << endl;
            count++;
        }
    }
    cout << "Total CPUs: " << count << "\n\n";
}
 
void read_mem_info() {
    // --- Added explanation: ifstream memFile("/proc/meminfo") ---------------
    // Generic syntax:  std::ifstream variableName( const char* filename )
    //   filename - parameter #1 (only parameter): path of the file to open
    //              for reading ("/proc/meminfo", here). As with cpuFile
    //              above, this path does not exist on Windows, so the open
    //              fails.
    ifstream memFile("/proc/meminfo");
    if (!memFile) {
        cerr << "Failed to open /proc/meminfo\n";
        return;
    }

    string line;
    cout << "=== MEMORY INFO ===\n";
    // --- Added explanation: getline(memFile, line) ---------------------------
    // Generic syntax:  std::getline( istream& is, string& str )
    //   is  - parameter #1: the input stream to read from (memFile, here).
    //   str - parameter #2: the string that receives the line's text
    //         (line, here). Used here as part of the for-loop condition
    //         together with `i < 3`, so the loop stops after 3 lines OR as
    //         soon as memFile runs out of lines (or was never opened) -
    //         whichever happens first.
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

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() starts and calls WindowsSystemInfo() first.
// #2  Inside WindowsSystemInfo(): a local SYSTEM_INFO struct `sysInfo` is
//     declared (uninitialized), then GetSystemInfo(&sysInfo) fills it in
//     with the current machine's processor architecture, number of
//     logical processors, page size, and the min/max application address
//     range. Five cout lines then print those fields straight out of
//     `sysInfo` under a "System Information:" header.
// #3  Still inside WindowsSystemInfo(): a local MEMORYSTATUSEX struct
//     `memStatus` is declared, its dwLength field is set to
//     sizeof(memStatus) (required so the API knows the struct's size/
//     version), and GlobalMemoryStatusEx(&memStatus) fills the rest of it
//     in with total/available physical and virtual memory (in bytes).
//     Four cout lines then print those fields, converted from bytes to MB
//     by dividing by (1024 * 1024), under a "Memory Information:" header.
//     WindowsSystemInfo() returns after this.
// #4  Back in main(), read_cpu_info() runs next. It tries to open
//     "/proc/cpuinfo" with an ifstream. On Windows (which this file is
//     written for, per the comments at the top) that path does not exist,
//     so the stream fails to open, `if (!cpuFile)` is true, "Failed to
//     open /proc/cpuinfo" is printed to cerr, and the function returns
//     immediately - no CPU lines are ever read or printed on Windows. (On
//     a Linux host where /proc/cpuinfo exists, this function would instead
//     loop with getline, print every line containing "model name", and
//     finish by printing the total count of such lines.)
// #5  Back in main(), read_mem_info() runs next, mirroring read_cpu_info()
//     but against "/proc/meminfo". On Windows this open also fails, so
//     "Failed to open /proc/meminfo" is printed to cerr and the function
//     returns immediately without printing any memory lines. (On Linux,
//     it would instead print the first 3 lines of /proc/meminfo.)
// #6  main() returns 0. Net effect on this Windows/MinGW build: the
//     "System Information:" and "Memory Information:" sections (from
//     WindowsSystemInfo(), steps #2-#3) print real data read from the
//     Windows API, while the CPU-info and memory-info sections that rely
//     on /proc files (steps #4-#5) print only their "Failed to open ..."
//     error messages, since /proc does not exist on Windows.
// ============================================================================
