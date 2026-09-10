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

    Vehicle* v1 = new Car();
    Vehicle* v2 = new Truck();
    // v1 and v2 both have the STATIC type "Vehicle*" - that's what the
    // compiler sees. typeid(*v1) asks the OBJECT itself what it is.
    cout << "STEP 3: typeid(*v1).name()    = " << typeid(*v1).name() << " (it's really a Car)\n";
    cout << "STEP 3: typeid(*v2).name()    = " << typeid(*v2).name() << " (it's really a Truck)\n\n";

    // ---------------------------------------------------------------------
    // STEP 4: dynamic_cast on a pointer - the safe downcast.
    // Ask "is v1 really a Car?" If yes, get a usable Car*. If no, get
    // nullptr instead of a crash or garbage pointer.
    // ---------------------------------------------------------------------
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
        Car& carRef = dynamic_cast<Car&>(vref);      // succeeds - vref IS a Car
        cout << "STEP 5: dynamic_cast<Car&>(vref) succeeded\n";
        (void)carRef;

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
    Vehicle* fleet[] = { new Car(), new Truck(), new Car() };
    cout << "STEP 6: identifying a mixed fleet of Vehicle*\n";
    for (Vehicle* veh : fleet) {
        if (dynamic_cast<Car*>(veh))
            cout << "  -> found a Car\n";
        else if (dynamic_cast<Truck*>(veh))
            cout << "  -> found a Truck\n";
    }

    delete v1;
    delete v2;
    for (Vehicle* veh : fleet) delete veh;
}
