// CHALLENGE MODE — Nimbus Fleet Command: Diagnostics Relay Agent
//
// Brings together every topic tab from Module 8:
//   Networking      : sendReportOverTcp()
//   Move semantics  : buildReport() takes ownership of the readings vector
//   Pipes & FDs     : archiveReportViaPipe()
//   System info     : getSystemSnapshot()
//   Additional (RTTI): NetworkEvent::describe() + isCritical()
//   Linking         : see the CONSTRAINT at the bottom of this file
//
// This file has 5 features. Each one currently throws a clear
// "[TODO] ..." message instead of doing real work, so the program compiles
// and runs today. Fill in one feature at a time, rebuilding after each.
// Companion file: relay_server.cpp (same folder) — start it before testing
// Feature 4.
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <sysinfoapi.h>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
using namespace std;

// ---------- GIVEN: DiagnosticEvent hierarchy ----------
struct DiagnosticEvent {
    string source;
    DiagnosticEvent(string s) : source(move(s)) {}
    virtual ~DiagnosticEvent() {}
    virtual string describe() const { return source + ": generic event"; }
};
struct TempEvent : DiagnosticEvent {
    double celsius;
    TempEvent(string s, double c) : DiagnosticEvent(move(s)), celsius(c) {}
    string describe() const override { return source + ": temperature " + to_string(celsius) + "C"; }
};
struct DiskEvent : DiagnosticEvent {
    int percentFull;
    DiskEvent(string s, int pct) : DiagnosticEvent(move(s)), percentFull(pct) {}
    string describe() const override { return source + ": disk " + to_string(percentFull) + "% full"; }
};

// ---------- GIVEN: NetworkEvent skeleton — fields and constructor only ----------
struct NetworkEvent : DiagnosticEvent {
    int latencyMs;
    NetworkEvent(string s, int ms) : DiagnosticEvent(move(s)), latencyMs(ms) {}

    // Feature 1a (YOUR TASK): return "<source>: network latency <latencyMs>ms"
    // e.g. source="Rack-C", latencyMs=610 -> "Rack-C: network latency 610ms"
    string describe() const override {
        throw logic_error("Feature 1 not implemented: NetworkEvent::describe()");
    }
};

// Feature 1b (YOUR TASK): identify each event's REAL type at run time and
// apply that type's own critical threshold:
//   TempEvent    -> critical if celsius    > 90.0
//   DiskEvent    -> critical if percentFull > 90
//   NetworkEvent -> critical if latencyMs   > 500
// Use dynamic_cast<const T*>(&e) for each check; anything else is not critical.
bool isCritical(const DiagnosticEvent& e) {
    throw logic_error("Feature 1 not implemented: isCritical()");
}

// Feature 2 (YOUR TASK): move semantics.
// This function takes `readings` BY VALUE on purpose — the caller's vector
// is moved into this parameter, not copied, because a vector<unique_ptr<T>>
// cannot be copied at all (unique_ptr has no copy constructor). Build an
// ostringstream starting with the line "=== Fleet Diagnostics Report ===\n",
// then for every event append e->describe(), plus " [CRITICAL]" when
// isCritical(*e) is true, then a newline. Return report.str().
string buildReport(vector<unique_ptr<DiagnosticEvent>> readings) {
    throw logic_error("Feature 2 not implemented: buildReport()");
}

// Feature 3 (YOUR TASK): system info.
// Fill a SYSTEM_INFO via GetSystemInfo() and a MEMORYSTATUSEX (remember to
// set memStatus.dwLength = sizeof(memStatus) BEFORE calling
// GlobalMemoryStatusEx(), or the call fails) and return a string formatted
// exactly as: "Processors: <N> | Total RAM: <MB> MB"
string getSystemSnapshot() {
    throw logic_error("Feature 3 not implemented: getSystemSnapshot()");
}

// Feature 4 (YOUR TASK): networking.
// Create a TCP socket, connect() to host:port, and if the connection
// succeeds, send() the full report. Return true on a successful connect,
// false otherwise — a failed connect must NOT crash or throw, because the
// relay server may not be running yet (that's an expected, testable case).
// Don't forget WSAStartup before and WSACleanup after.
bool sendReportOverTcp(const string& report, const char* host, int port) {
    throw logic_error("Feature 4 not implemented: sendReportOverTcp()");
}

// ---------- GIVEN: the archiver child process (the "-archive" side) ----------
int child_main() {
    string line;
    if (getline(cin, line)) {
        ofstream log("archive_log.txt", ios::app);
        log << line << "\n";
        cout << "[Archiver] Wrote " << line.size() << " bytes to archive_log.txt\n";
    }
    return 0;
}

// Feature 5 (YOUR TASK): pipes & file descriptors.
// Re-launch THIS SAME executable with the argument "archive" (see main()'s
// argv check and GetModuleFileNameA), connected via an anonymous pipe whose
// read end becomes the child's stdin (STARTUPINFOA.hStdInput + dwFlags =
// STARTF_USESTDHANDLES). Write `report` to the pipe's write end, then close
// it to signal EOF, then WaitForSingleObject on the child. Return true if
// CreatePipe/CreateProcess both succeeded, false otherwise.
// Reminder: the child reads ONE line with std::getline — strip or replace
// any '\n' characters inside `report` before writing it, or the child will
// only see the first line.
bool archiveReportViaPipe(const string& report) {
    throw logic_error("Feature 5 not implemented: archiveReportViaPipe()");
}

// ---------- GIVEN: wiring it all together ----------
int main(int argc, char* argv[]) {
    if (argc >= 2 && string(argv[1]) == "archive")
        return child_main();

    vector<unique_ptr<DiagnosticEvent>> readings;
    readings.push_back(make_unique<TempEvent>("Rack-A", 94.5));
    readings.push_back(make_unique<DiskEvent>("Rack-B", 42));
    readings.push_back(make_unique<NetworkEvent>("Rack-C", 610));

    string report;
    try {
        report = buildReport(move(readings));
    } catch (const logic_error& e) {
        cout << "[TODO] " << e.what() << "\n";
        report = "=== Fleet Diagnostics Report (placeholder) ===\n";
    }

    try {
        report += getSystemSnapshot() + "\n";
    } catch (const logic_error& e) {
        cout << "[TODO] " << e.what() << "\n";
    }

    cout << report;

    try {
        if (sendReportOverTcp(report, "127.0.0.1", 9090))
            cout << "Report sent to relay.\n";
        else
            cout << "Could not reach relay (start relay_server.exe first).\n";
    } catch (const logic_error& e) {
        cout << "[TODO] " << e.what() << "\n";
    }

    try {
        if (archiveReportViaPipe(report))
            cout << "Report archived via pipe.\n";
        else
            cout << "Archiver pipe failed.\n";
    } catch (const logic_error& e) {
        cout << "[TODO] " << e.what() << "\n";
    }
}

// ---------- CONSTRAINT (Linking — no TODO stub, this is a structural task) ----------
// Once all 5 features above pass, move the DiagnosticEvent/TempEvent/
// DiskEvent/NetworkEvent/isCritical block into its own rttilib.h (declarations)
// and rttilib.cpp (definitions), matching the Linking tab's rrmathlib.h/.cpp
// pattern. Compile rttilib.cpp into a static library with `ar`, then link your
// main.cpp against that library instead of compiling the hierarchy inline.
// Your submission must include the exact g++/ar commands you used.
