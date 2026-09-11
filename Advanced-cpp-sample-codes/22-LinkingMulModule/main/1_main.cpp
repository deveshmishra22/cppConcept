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
    //   a - parameter #1: the local variable `a` (20) declared above.
    //   b - parameter #2: the local variable `b` (10) declared above.
    // This call is resolved at link time to the add() DEFINITION compiled
    // from src/1_rrmathlib.cpp - this file only saw add()'s DECLARATION via
    // #include "rrmathlib.h", not its body.
    cout << "Addition: " << add(a, b) << endl;
    // --- Added explanation: subtract(a, b) -----------------------------------
    // Generic syntax:  subtract( int a, int b )
    //   a - parameter #1: `a` (20), the value subtracted from.
    //   b - parameter #2: `b` (10), the value being subtracted.
    cout << "Subtraction: " << subtract(a, b) << endl;
    // --- Added explanation: multiply(a, b) -----------------------------------
    // Generic syntax:  multiply( int a, int b )
    //   a - parameter #1: `a` (20), the first factor.
    //   b - parameter #2: `b` (10), the second factor.
    cout << "Multiplication: " << multiply(a, b) << endl;
    try {
        // --- Added explanation: divide(a, b) ---------------------------------
        // Generic syntax:  divide( int a, int b )
        //   a - parameter #1: `a` (20), the numerator.
        //   b - parameter #2: `b` (10), the (non-zero) denominator, so this
        //       call returns normally with 20.0 / 10.0 = 2.0.
        cout << "Division: " << divide(a, b) << endl;
        // --- Added explanation: divide(a, 0) ---------------------------------
        // Generic syntax:  divide( int a, int b )
        //   a - parameter #1: `a` (20), the numerator.
        //   b - parameter #2: the literal 0, passed deliberately instead of
        //       `b` so divide()'s `if (b == 0)` check is true and it throws
        //       a const char* ("Division by zero error!") instead of
        //       returning a value.
        cout << "Division by zero test: " << divide(a, 0) << endl; // This will throw an exception
    } catch (const char* msg) {
        cerr << "Error: " << msg << endl;
    }

    return 0;
}
// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #0  Compile + link picture (why this demo is split across 3 locations):
//       - header/rrmathlib.h DECLARES add/subtract/multiply/divide (names,
//         parameter types, return types) but has no function bodies.
//       - src/1_rrmathlib.cpp #includes that header and DEFINES the actual
//         bodies of all four functions. The compiler compiles this file, on
//         its own, into an object file (e.g. 1_rrmathlib.o).
//       - main/1_main.cpp (this file) also #includes the SAME header, which
//         is how it is allowed to call add/subtract/multiply/divide even
//         though it never sees their bodies. The compiler compiles THIS
//         file, on its own, into its own separate object file (e.g.
//         1_main.o) - at this point every call below is just a placeholder
//         "call whatever function is named add" instruction.
//       - The LINKER then combines 1_main.o and 1_rrmathlib.o into one
//         executable, matching each unresolved call in 1_main.o to the
//         matching function body compiled into 1_rrmathlib.o. This is the
//         "linking multiple modules" this folder demonstrates.
// #1  main() starts; a = 20 and b = 10 are declared, and the banner line
//     "Static vs Dynamic Linking in C++" is printed.
// #2  add(a, b) runs (in the linked-in rrmathlib object code) and returns
//     30; "Addition: 30" is printed.
// #3  subtract(a, b) returns 10; "Subtraction: 10" is printed.
// #4  multiply(a, b) returns 200; "Multiplication: 200" is printed.
// #5  Inside the try block: divide(a, b) checks b == 0 (10 == 0 is false),
//     so it returns static_cast<double>(20) / 10 = 2.0; "Division: 2"
//     is printed.
// #6  Still inside the SAME try block: divide(a, 0) checks b == 0 (0 == 0
//     is true this time), so instead of returning it executes
//     `throw "Division by zero error!";`. This immediately abandons the
//     rest of the try block (the "Division by zero test: " line never
//     finishes printing) and unwinds to the nearest matching catch.
// #7  `catch (const char* msg)` matches (the thrown value is a const
//     char*), binds msg to "Division by zero error!", and prints
//     "Error: Division by zero error!" to cerr.
// #8  main() reaches `return 0;` and the program exits normally - the
//     thrown exception was fully handled by the catch block, so nothing
//     propagates further.
// ============================================================================
