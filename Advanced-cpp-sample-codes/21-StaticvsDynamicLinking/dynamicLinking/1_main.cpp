#include "rrmathlib.h"
#include <iostream>
using namespace std;

int main()
{
    int a = 20, b = 10;

    cout << "Static vs Dynamic Linking in C++" << endl;

    // Using functions from the math library
    // --- Added explanation: add(a, b) ---------------------------------------
    // Generic syntax:  add( int a, int b )
    //   a - parameter #1: first operand to add (20, here - the local `a`).
    //   b - parameter #2: second operand to add (10, here - the local `b`).
    // This call is resolved into rrmathlib.dll (see notes below).
    cout << "Addition: " << add(a, b) << endl;
    // --- Added explanation: subtract(a, b) -----------------------------------
    // Generic syntax:  subtract( int a, int b )
    //   a - parameter #1: the value to subtract FROM (20, here).
    //   b - parameter #2: the value being subtracted (10, here).
    cout << "Subtraction: " << subtract(a, b) << endl;
    // --- Added explanation: multiply(a, b) -----------------------------------
    // Generic syntax:  multiply( int a, int b )
    //   a - parameter #1: first operand to multiply (20, here).
    //   b - parameter #2: second operand to multiply (10, here).
    cout << "Multiplication: " << multiply(a, b) << endl;
    try {
        // --- Added explanation: divide(a, b) ---------------------------------
        // Generic syntax:  divide( int a, int b )
        //   a - parameter #1: the dividend/numerator (20, here).
        //   b - parameter #2: the divisor/denominator (10, here); divide()
        //       throws a const char* if this is 0.
        cout << "Division: " << divide(a, b) << endl;
        // --- Added explanation: divide(a, 0) ---------------------------------
        // Generic syntax:  divide( int a, int b )
        //   a - parameter #1: the dividend/numerator (20, here).
        //   b - parameter #2: the divisor/denominator - literal 0 here, which
        //       deliberately triggers divide()'s `throw "Division by zero
        //       error!";` so the catch block below can be demonstrated.
        cout << "Division by zero test: " << divide(a, 0) << endl; // This will throw an exception
    } catch (const char* msg) {
        cerr << "Error: " << msg << endl;
    }

    return 0;
}

// Steps to keep in mind:
// 1. Create header file (rrmathlib.h) with function declarations.
// 2. Create source file (rrmathlib.cpp) with function definitions.
// 3. Compile rrmathlib.cpp to create a dynamic library (e.g., rrmathlib.dll or librrmathlib.so).
// How to compile a dynamic library?
// For Windows (using g++):
// g++ -shared -o rrmathlib.dll rrmathlib.cpp
// Once dll is created, we can link it during compilation of main.cpp
// g++ main.cpp -L. -lrrmathlib -o main.exe
// 4. In main.cpp, include the header file and link against the library during compilation
//   (e.g., g++ main.cpp -L. -lrrmathlib -o main).
// 5. Run the executable to see the results.
// What is the difference between static and dynamic linking?
// - Static Linking: In static linking, all the library code used by the program is copied
//   into the final executable at compile time. This results in a larger executable size
//   but allows the program to run independently of external libraries.
// - Dynamic Linking: In dynamic linking, the library code is not copied into the executable.
//   Instead, the program contains references to the shared libraries, which are loaded at runtime.

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  Before this program can run, 2_rrmathlib.cpp in this same folder is
//     compiled into a DYNAMIC library using the command shown above:
//     `g++ -shared -o rrmathlib.dll rrmathlib.cpp`. This produces
//     rrmathlib.dll, a separate file containing compiled machine code for
//     add/subtract/multiply/divide.
// #2  This main.cpp is then compiled and linked AGAINST that dll (per the
//     comment above: `g++ main.cpp -L. -lrrmathlib -o main.exe`). Unlike
//     the static version in the parent folder, add/subtract/multiply/
//     divide's actual machine code is NOT copied into main.exe here - only
//     a reference/import saying "these symbols live in rrmathlib.dll" is
//     recorded.
// #3  main() starts. `a = 20`, `b = 10` are declared, and the banner line
//     "Static vs Dynamic Linking in C++" is printed.
// #4  add(a, b) is called: at this point the OS loader resolves the
//     reference recorded in step #2 by locating rrmathlib.dll (it must be
//     on disk somewhere the OS can find it, e.g. the same folder as
//     main.exe or on PATH) and jumping into ITS copy of add()'s code -
//     which then returns 30, printed as "Addition: 30".
// #5  subtract(a, b) and multiply(a, b) run the same way, jumping into
//     rrmathlib.dll's code each time, returning 10 and 200 respectively.
// #6  Inside the try block: divide(a, b) (also resolved into
//     rrmathlib.dll) returns 2.0, printed as "Division: 2".
// #7  divide(a, 0) is called next; the divide() code running inside
//     rrmathlib.dll throws a const char* "Division by zero error!" because
//     its `b` parameter is 0, before it can return anything.
// #8  The thrown const char* propagates back out of the dll call and is
//     caught by `catch (const char* msg)` in this file, printing
//     "Error: Division by zero error!" to cerr.
// #9  main() returns 0.
//
// Dynamic linking, in plain language: add()/subtract()/multiply()/
// divide()'s actual code stays OUTSIDE this executable, living in
// rrmathlib.dll. main.exe only stores a note that those symbols must be
// resolved from that dll, and the OS loader wires up the connection when
// the program is loaded (or on first call, depending on load mode) -
// rrmathlib.dll MUST be present and locatable at run time or the program
// fails to run with a missing-dll error. This is the opposite tradeoff
// from the static version one folder up (..\1_main.cpp): that executable
// is self-contained but larger; this one is smaller but depends on
// rrmathlib.dll always being shipped alongside it.
// ============================================================================