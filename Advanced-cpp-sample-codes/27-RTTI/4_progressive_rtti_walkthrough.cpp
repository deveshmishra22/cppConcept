// Progressive RTTI walkthrough - one idea per step, simplest possible code.
// Read this file top to bottom. Compile once, then re-read each STEP's
// comment right before its output prints. Nothing here depends on a step
// that hasn't printed yet.
//
// Compile:  g++ -std=c++17 4_progressive_rtti_walkthrough.cpp -o rtti_walkthrough
// Run:      rtti_walkthrough.exe   (Windows)   or   ./rtti_walkthrough   (Linux/macOS)

#include <iostream>
#include <typeinfo>
using namespace std;

int main() {
    // ---------------------------------------------------------------------
    // STEP 1: typeid on an ordinary variable.
    // typeid(expr) returns a std::type_info object describing expr's type.
    // For a plain int, the compiler already knows the answer while compiling
    // your code - no "run-time" work is needed.
    // ---------------------------------------------------------------------
    int plainNumber = 42;
    // --- Added explanation: typeid(plainNumber) --------------------------------
    // Generic syntax:  typeid( expression )
    //   expression - plainNumber, an int. Because int can never be
    //                polymorphic, the compiler already knows the answer
    //                while compiling this line - no vtable lookup happens
    //                when this actually runs.
    cout << "STEP 1: typeid(plainNumber).name() = "
         << typeid(plainNumber).name() << "\n\n";

    // ---------------------------------------------------------------------
    // STEP 2: Two unrelated classes, NEITHER has a virtual function.
    // typeid still works, but it can only ever tell you the type you wrote
    // in the source - it cannot look "inside" an object at run time yet.
    // ---------------------------------------------------------------------
    struct Wheel {};
    struct Engine {};
    Wheel w;
    Engine e;
    // --- Added explanation: typeid(w) / typeid(e) / typeid(w)==typeid(e) -------
    // Generic syntax:  typeid( expression )   and   typeid(a) == typeid(b)
    //   expression - w (a Wheel) in the first call, e (an Engine) in the
    //                second; both are resolved at compile time since
    //                neither Wheel nor Engine has a virtual function.
    //   a, b       - the two operands compared by operator== a few lines
    //                below; here w and e, which are unrelated types, so
    //                the comparison is false ("no") - as it would be even
    //                without any RTTI support, since this is decided
    //                purely from the source, not from inspecting any
    //                object at run time.
    cout << "STEP 2: typeid(w).name()      = " << typeid(w).name() << "\n";
    cout << "STEP 2: typeid(e).name()      = " << typeid(e).name() << "\n";
    cout << "STEP 2: are they the same type? "
         << (typeid(w) == typeid(e) ? "yes" : "no") << "\n\n";

    // ---------------------------------------------------------------------
    // STEP 3: Introduce ONE virtual function. This single word - "virtual" -
    // is what turns the class "polymorphic" and switches typeid from a
    // compile-time answer to a genuine run-time lookup.
    // ---------------------------------------------------------------------
    struct Vehicle {
        virtual ~Vehicle() {}   // <-- this line makes RTTI possible
    };
    struct Car : Vehicle {};
    struct Truck : Vehicle {};

    // --- Added explanation: new Car() / new Truck() ---------------------------
    // Generic syntax:  new ClassName( ctorArgs... )
    //   ClassName - Car, then Truck; neither declares its own constructor,
    //               so each uses the compiler-generated default one (which
    //               also runs Vehicle's default constructor as the base).
    //   ctorArgs  - none; both calls take zero arguments.
    // Each `new` returns a pointer stored in a Vehicle* variable (v1, v2) -
    // the STATIC type visible to the compiler is Vehicle* in both cases,
    // even though the real objects underneath are a Car and a Truck.
    Vehicle* v1 = new Car();
    Vehicle* v2 = new Truck();
    // v1 and v2 both have the STATIC type "Vehicle*" - that's what the
    // compiler sees. typeid(*v1) asks the OBJECT itself what it is.
    // --- Added explanation: typeid(*v1) / typeid(*v2) --------------------------
    // Generic syntax:  typeid( *pointerExpr )
    //   *pointerExpr - the dereferenced pointer, so the operand is the
    //                  Vehicle OBJECT itself (v1's or v2's pointee), not
    //                  the pointer variable. Because Vehicle has a virtual
    //                  destructor, this typeid performs a genuine run-time
    //                  vtable lookup - it reports Car for *v1 and Truck
    //                  for *v2, their real most-derived types.
    cout << "STEP 3: typeid(*v1).name()    = " << typeid(*v1).name() << " (it's really a Car)\n";
    cout << "STEP 3: typeid(*v2).name()    = " << typeid(*v2).name() << " (it's really a Truck)\n\n";

    // ---------------------------------------------------------------------
    // STEP 4: dynamic_cast on a pointer - the safe downcast.
    // Ask "is v1 really a Car?" If yes, get a usable Car*. If no, get
    // nullptr instead of a crash or garbage pointer.
    // ---------------------------------------------------------------------
    // --- Added explanation: dynamic_cast<Car*>(v1) / dynamic_cast<Truck*>(v1) --
    // Generic syntax:  dynamic_cast<TargetType*>( sourcePointer )
    //   TargetType    - Car* in the first call, Truck* in the second: the
    //                   type you want to end up pointing at.
    //   sourcePointer - v1 in both calls: a Vehicle* whose real pointee is
    //                   a Car (constructed in STEP 3 above).
    // The first call succeeds (v1 really points at a Car), returning a
    // usable Car* stored in maybeCar. The second call fails (v1's object
    // is not a Truck), returning nullptr, stored in maybeTruck - proving
    // the cast checks the real object, not just that both are some kind
    // of Vehicle.
    Car* maybeCar = dynamic_cast<Car*>(v1);
    Truck* maybeTruck = dynamic_cast<Truck*>(v1); // v1 is NOT a Truck
    cout << "STEP 4: dynamic_cast<Car*>(v1)   -> " << (maybeCar ? "succeeded" : "nullptr") << "\n";
    cout << "STEP 4: dynamic_cast<Truck*>(v1) -> " << (maybeTruck ? "succeeded" : "nullptr") << "\n\n";

    // ---------------------------------------------------------------------
    // STEP 5: dynamic_cast on a reference - the "loud failure" version.
    // A reference cannot be null, so an impossible cast throws std::bad_cast
    // instead of returning a sentinel value.
    // ---------------------------------------------------------------------
    Vehicle& vref = *v1; // vref refers to the same Car object as v1
    try {
        // --- Added explanation: dynamic_cast<Car&>(vref) -----------------------
        // Generic syntax:  dynamic_cast<TargetType&>( sourceReference )
        //   TargetType      - Car& here.
        //   sourceReference - vref, a Vehicle& that actually refers to the
        //                     same Car object v1 points to.
        // Since vref's real object IS a Car, this succeeds and returns a
        // Car& bound to that same object (no nullptr possible for
        // references, unlike the pointer form in STEP 4).
        Car& carRef = dynamic_cast<Car&>(vref);      // succeeds - vref IS a Car
        cout << "STEP 5: dynamic_cast<Car&>(vref) succeeded\n";
        (void)carRef;

        // --- Added explanation: dynamic_cast<Truck&>(vref) ----------------------
        // Generic syntax:  dynamic_cast<TargetType&>( sourceReference )
        //   TargetType      - Truck& here.
        //   sourceReference - vref again, but its real object is a Car,
        //                     not a Truck.
        // Because a reference can't be null, an impossible reference
        // dynamic_cast throws std::bad_cast instead - so this line throws
        // immediately, truckRef is never initialized, and the next two
        // lines never execute; control jumps to the catch block below.
        Truck& truckRef = dynamic_cast<Truck&>(vref); // impossible - throws
        cout << "STEP 5: unreachable\n";
        (void)truckRef;
    } catch (const bad_cast&) {
        cout << "STEP 5: dynamic_cast<Truck&>(vref) threw std::bad_cast, as expected\n\n";
    }

    // ---------------------------------------------------------------------
    // STEP 6: Put it together - identify a mixed collection at run time.
    // This is the pattern real code uses: a container of base-class
    // pointers, and one place that needs to know each object's real type.
    // ---------------------------------------------------------------------
    // --- Added explanation: new Car() / new Truck() / new Car() (fleet) -------
    // Generic syntax:  new ClassName( ctorArgs... )
    //   ClassName - Car, Truck, Car again; each with zero constructor
    //               args, same as STEP 3's calls.
    // The three resulting pointers are collected into one Vehicle* array,
    // mixing real Car and Truck objects behind a single element type - the
    // scenario the loop below resolves with dynamic_cast.
    Vehicle* fleet[] = { new Car(), new Truck(), new Car() };
    cout << "STEP 6: identifying a mixed fleet of Vehicle*\n";
    for (Vehicle* veh : fleet) {
        // --- Added explanation: dynamic_cast<Car*>(veh) / dynamic_cast<Truck*>(veh)
        // Generic syntax:  dynamic_cast<TargetType*>( sourcePointer )
        //   TargetType    - Car* tried first, Truck* tried second.
        //   sourcePointer - veh, the current fleet element - a Vehicle*
        //                   whose real pointee changes each loop iteration
        //                   (Car, then Truck, then Car again).
        // Exactly one of the two casts succeeds per iteration (matching
        // veh's real type), so exactly one branch's cout runs each time -
        // this is the same if/else-if identification pattern used in
        // 2_dynamic_cast.cpp and 3_rtti_usecase.cpp, applied to a fresh
        // example.
        if (dynamic_cast<Car*>(veh))
            cout << "  -> found a Car\n";
        else if (dynamic_cast<Truck*>(veh))
            cout << "  -> found a Truck\n";
    }

    delete v1;
    delete v2;
    for (Vehicle* veh : fleet) delete veh;
}

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() begins. STEP 1: plainNumber = 42 (a non-polymorphic int).
//     typeid(plainNumber) is resolved entirely at compile time, printing
//     the compiler-mangled name for int (typically "i" on g++).
// #2  STEP 2: local structs Wheel and Engine are defined (both
//     non-polymorphic, unrelated types). w and e are default-constructed.
//     typeid(w) and typeid(e) are both compile-time answers; the
//     comparison typeid(w) == typeid(e) is false, so "no" is printed.
// #3  STEP 3: local structs Vehicle (with a virtual ~Vehicle()), Car :
//     Vehicle, and Truck : Vehicle are defined. new Car() and new Truck()
//     construct two heap objects, whose addresses are stored in v1 and v2
//     (both declared Vehicle*). typeid(*v1) performs a genuine run-time
//     vtable lookup and reports Car; typeid(*v2) reports Truck - matching
//     the "(it's really a ...)" comments.
// #4  STEP 4: dynamic_cast<Car*>(v1) succeeds (v1's real object is a Car),
//     so maybeCar is non-null - "succeeded" is printed. dynamic_cast
//     <Truck*>(v1) fails (v1 is not a Truck), so maybeTruck is nullptr -
//     "nullptr" is printed.
// #5  STEP 5: vref binds to *v1 (the same Car object). Inside the try
//     block: dynamic_cast<Car&>(vref) succeeds, carRef is bound, and
//     "STEP 5: dynamic_cast<Car&>(vref) succeeded" is printed; (void)carRef
//     silences an unused-variable warning. dynamic_cast<Truck&>(vref) is
//     attempted next - vref's real object is a Car, not a Truck, so this
//     THROWS std::bad_cast immediately. truckRef is never initialized,
//     "STEP 5: unreachable" is never printed, and control jumps straight
//     to the catch(const bad_cast&) block, which prints "STEP 5:
//     dynamic_cast<Truck&>(vref) threw std::bad_cast, as expected".
// #6  STEP 6: fleet[] holds three new pointers - Car, Truck, Car (three
//     real heap objects) - all stored as Vehicle*. The header is printed,
//     then the loop runs: iteration 1 (Car) -> dynamic_cast<Car*>
//     succeeds -> "-> found a Car"; iteration 2 (Truck) -> the Car* cast
//     fails, the Truck* cast succeeds -> "-> found a Truck"; iteration 3
//     (Car) -> same as iteration 1 -> "-> found a Car".
// #7  Cleanup: delete v1; delete v2; free the STEP 3/4/5 Car and Truck
//     objects (their destructors run virtually via Vehicle's virtual
//     destructor). The final loop, for (Vehicle* veh : fleet) delete veh;,
//     frees the three fleet objects the same way.
// #8  main() falls off the end without an explicit return statement;
//     since main's return type is int, this implicitly returns 0, ending
//     the program successfully.
// ============================================================================
