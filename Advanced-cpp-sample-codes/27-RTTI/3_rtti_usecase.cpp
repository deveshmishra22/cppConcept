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
    // --- Added explanation: dynamic_cast<const TempEvent*>(&e) etc. -----------
    // Generic syntax:  dynamic_cast<TargetType*>( sourcePointer )
    //   TargetType    - the concrete event type being tested for, in turn:
    //                   const TempEvent*, then const DiskEvent*, then
    //                   const NetworkEvent*. `const` is included because
    //                   parameter `e` is a const DiagnosticEvent&, so only
    //                   a const-qualified pointer type is reachable from
    //                   it.
    //   sourcePointer - &e: the address of the (const) DiagnosticEvent that
    //                   was passed in; its real object is whichever
    //                   concrete event isCritical() was called with
    //                   (TempEvent, DiskEvent, or NetworkEvent, depending
    //                   on the caller).
    // Each dynamic_cast either succeeds (returns a non-null pointer to the
    // matching concrete type, letting the if-body read that type's own
    // field - celsius, percentFull, or latencyMs) or fails (returns
    // nullptr, so the if is skipped and the next dynamic_cast in the chain
    // is tried).
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
    // --- Added explanation: make_unique<TempEvent>("Rack-A", 94.5) etc. -------
    // Generic syntax:  std::make_unique<ClassName>( ctorArg1, ctorArg2, ... )
    //   ClassName  - template parameter: which concrete DiagnosticEvent
    //                subclass to construct; TempEvent, DiskEvent (twice),
    //                and NetworkEvent below.
    //   ctorArg1   - each subclass's `s` (source) parameter: "Rack-A",
    //                "Rack-B", "Rack-C", "Rack-D" respectively.
    //   ctorArg2   - each subclass's second constructor parameter: celsius
    //                (94.5), percentFull (42), latencyMs (610), then
    //                percentFull again (97).
    // make_unique allocates the object on the heap and returns a
    // std::unique_ptr<ClassName>, which push_back stores in `queue` - but
    // widened to unique_ptr<DiagnosticEvent> (the base), since that is the
    // vector's element type. The concrete type is preserved at run time via
    // the vtable, which is exactly what describe(), dynamic_cast, and
    // typeid below rely on.
    queue.push_back(make_unique<TempEvent>("Rack-A", 94.5));
    queue.push_back(make_unique<DiskEvent>("Rack-B", 42));
    queue.push_back(make_unique<NetworkEvent>("Rack-C", 610));
    queue.push_back(make_unique<DiskEvent>("Rack-D", 97));

    cout << "=== Processing heterogeneous diagnostic queue ===\n";
    for (const auto& eventPtr : queue) {
        cout << eventPtr->describe();                       // ordinary virtual call
        // --- Added explanation: isCritical(*eventPtr) --------------------------
        // Generic syntax:  isCritical( const DiagnosticEvent& e )
        //   e - parameter #1 (only parameter): *eventPtr, i.e. the actual
        //       DiagnosticEvent object a unique_ptr in `queue` owns on this
        //       loop iteration (a TempEvent, DiskEvent, or NetworkEvent).
        // This call runs the dynamic_cast chain defined above on whichever
        // concrete object it was given, and returns true/false depending
        // on that specific event's own threshold field.
        if (isCritical(*eventPtr))                           // RTTI-based routing
            cout << "  [CRITICAL - paging on-call]";
        cout << "\n";
    }

    cout << "\n=== typeid-based tally (an alternative to the dynamic_cast chain) ===\n";
    int tempCount = 0, diskCount = 0, netCount = 0;
    for (const auto& eventPtr : queue) {
        const DiagnosticEvent& e = *eventPtr;
        // --- Added explanation: typeid(e) == typeid(TempEvent) etc. -----------
        // Generic syntax:  typeid( operandA ) == typeid( operandB )
        //   operandA - e, a reference to the actual polymorphic object for
        //              this loop iteration (bound just above from
        //              *eventPtr).
        //   operandB - a TYPE NAME used directly as the operand (TempEvent,
        //              then DiskEvent, then NetworkEvent), asking for that
        //              named type's own type_info rather than any
        //              variable's.
        // Exactly one of these three comparisons is true per iteration
        // (matching e's real, most-derived type), incrementing the
        // matching counter - an alternative way to identify "what kind of
        // event is this?" without dynamic_cast's if/else-if chain, useful
        // when you don't need a usable pointer to the concrete type, only
        // its identity.
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

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() creates an empty vector<unique_ptr<DiagnosticEvent>> queue.
// #2  Four make_unique calls construct, on the heap: TempEvent("Rack-A",
//     94.5), DiskEvent("Rack-B", 42), NetworkEvent("Rack-C", 610), and
//     DiskEvent("Rack-D", 97). Each resulting unique_ptr is push_back'd
//     into queue, so queue ends up with 4 entries, all handled through the
//     base type DiagnosticEvent but each actually pointing at its own
//     concrete subtype.
// #3  First loop over queue: eventPtr->describe() runs virtually (ordinary
//     polymorphism, no RTTI needed) for each entry, printing:
//       "Rack-A: temperature reading 94.500000C"
//       "Rack-B: disk 42% full"
//       "Rack-C: network latency 610ms"
//       "Rack-D: disk 97% full"
// #4  For each entry, isCritical(*eventPtr) is also called:
//       - Rack-A (TempEvent, celsius=94.5): dynamic_cast<const TempEvent*>
//         succeeds; 94.5 > 90.0 is true -> critical.
//       - Rack-B (DiskEvent, percentFull=42): the TempEvent cast fails
//         (nullptr), the DiskEvent cast succeeds; 42 > 90 is false -> not
//         critical.
//       - Rack-C (NetworkEvent, latencyMs=610): TempEvent and DiskEvent
//         casts both fail, NetworkEvent cast succeeds; 610 > 500 is true
//         -> critical.
//       - Rack-D (DiskEvent, percentFull=97): DiskEvent cast succeeds;
//         97 > 90 is true -> critical.
//     So "  [CRITICAL - paging on-call]" is appended after Rack-A, Rack-C,
//     and Rack-D's lines, but not after Rack-B's.
// #5  Second loop: for each entry, typeid(e) is compared against
//     typeid(TempEvent), typeid(DiskEvent), typeid(NetworkEvent) in turn.
//     Tally ends up: tempCount = 1 (Rack-A), diskCount = 2 (Rack-B and
//     Rack-D), netCount = 1 (Rack-C). "Temp events: 1, Disk events: 2,
//     Network events: 1" is printed.
// #6  main() falls off the end, implicitly returning 0. `queue` (a local
//     vector of unique_ptr) is destroyed, which destroys each owned
//     DiagnosticEvent through its virtual destructor, freeing all four
//     heap objects.
// ============================================================================
