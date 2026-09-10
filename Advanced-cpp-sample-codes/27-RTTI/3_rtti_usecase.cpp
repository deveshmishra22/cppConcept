// A realistic use case for RTTI: a heterogeneous event queue.
//
// Most of the time, virtual functions alone are enough - you never need to
// ask "what type is this really?" because the base class already declares
// the behaviour every subtype must provide.
//
// RTTI earns its place when you receive a base-class handle from a boundary
// you don't fully control (a plugin, a deserializer, a legacy queue) and only
// SOME event kinds need special handling that doesn't belong on every event.
// Forcing every subtype to implement a virtual method it doesn't need just to
// satisfy one caller is worse than a small, well-documented dynamic_cast chain.

#include <iostream>
#include <memory>
#include <vector>
#include <string>
using namespace std;

struct DiagnosticEvent {
    string source;
    DiagnosticEvent(string s) : source(move(s)) {}
    virtual ~DiagnosticEvent() {}
    virtual string describe() const { return source + ": generic event"; }
};

struct TempEvent : DiagnosticEvent {
    double celsius;
    TempEvent(string s, double c) : DiagnosticEvent(move(s)), celsius(c) {}
    string describe() const override {
        return source + ": temperature reading " + to_string(celsius) + "C";
    }
};

struct DiskEvent : DiagnosticEvent {
    int percentFull;
    DiskEvent(string s, int pct) : DiagnosticEvent(move(s)), percentFull(pct) {}
    string describe() const override {
        return source + ": disk " + to_string(percentFull) + "% full";
    }
};

struct NetworkEvent : DiagnosticEvent {
    int latencyMs;
    NetworkEvent(string s, int ms) : DiagnosticEvent(move(s)), latencyMs(ms) {}
    string describe() const override {
        return source + ": network latency " + to_string(latencyMs) + "ms";
    }
};

// Every event already knows how to describe() itself - that's ordinary
// polymorphism and needs no RTTI. But the ALERT THRESHOLD RULES differ per
// concrete type, and adding "virtual bool isCritical()" to the base class
// would force NetworkEvent and DiskEvent to know about each other's
// thresholds. Instead, a small identification step at the one call site that
// needs it keeps each event class focused on its own data.
bool isCritical(const DiagnosticEvent& e) {
    if (auto* t = dynamic_cast<const TempEvent*>(&e))
        return t->celsius > 90.0;
    if (auto* d = dynamic_cast<const DiskEvent*>(&e))
        return d->percentFull > 90;
    if (auto* n = dynamic_cast<const NetworkEvent*>(&e))
        return n->latencyMs > 500;
    return false; // unknown event kinds are never treated as critical
}

int main() {
    vector<unique_ptr<DiagnosticEvent>> queue;
    queue.push_back(make_unique<TempEvent>("Rack-A", 94.5));
    queue.push_back(make_unique<DiskEvent>("Rack-B", 42));
    queue.push_back(make_unique<NetworkEvent>("Rack-C", 610));
    queue.push_back(make_unique<DiskEvent>("Rack-D", 97));

    cout << "=== Processing heterogeneous diagnostic queue ===\n";
    for (const auto& eventPtr : queue) {
        cout << eventPtr->describe();                       // ordinary virtual call
        if (isCritical(*eventPtr))                           // RTTI-based routing
            cout << "  [CRITICAL - paging on-call]";
        cout << "\n";
    }

    cout << "\n=== typeid-based tally (an alternative to the dynamic_cast chain) ===\n";
    int tempCount = 0, diskCount = 0, netCount = 0;
    for (const auto& eventPtr : queue) {
        const DiagnosticEvent& e = *eventPtr;
        if (typeid(e) == typeid(TempEvent)) ++tempCount;
        else if (typeid(e) == typeid(DiskEvent)) ++diskCount;
        else if (typeid(e) == typeid(NetworkEvent)) ++netCount;
    }
    cout << "Temp events: " << tempCount
         << ", Disk events: " << diskCount
         << ", Network events: " << netCount << "\n";
}

// Compile:  g++ -std=c++17 3_rtti_usecase.cpp -o rtti_usecase
// Run:      rtti_usecase.exe   (Windows)   or   ./rtti_usecase   (Linux/macOS)
