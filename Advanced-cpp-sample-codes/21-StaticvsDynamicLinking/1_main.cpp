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
// 3. Compile rrmathlib.cpp to create a static library (e.g., librrmathlib.a) or dynamic library (e.g., rrmathlib.dll or librrmathlib.so).
// 4. In main.cpp, include the header file and link against the library during compilation
//   (e.g., g++ main.cpp -L. -lrrmathlib -o main). or ( g++ main.cpp rrmathlib.cpp -o main for static linking).
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
// #1  Before this program can even run, rrmathlib.cpp is compiled and its
//     object code is either archived into a STATIC library (per the "Steps
//     to keep in mind" note above), or compiled directly alongside
//     main.cpp in one command (`g++ main.cpp rrmathlib.cpp -o main`).
//     Either way, the compiled machine code for add/subtract/multiply/
//     divide is COPIED into the final executable at compile/link time -
//     after this step, the resulting `main` binary is self-contained and
//     no longer needs rrmathlib.cpp, rrmathlib.h, or any .a file to run.
// #2  main() starts. `a = 20`, `b = 10` are declared, and the banner line
//     "Static vs Dynamic Linking in C++" is printed.
// #3  add(a, b) runs INSIDE this same executable's own machine code
//     (because it was statically linked in at step #1) and returns 30,
//     printed as "Addition: 30".
// #4  subtract(a, b) similarly returns 10, printed as "Subtraction: 10".
// #5  multiply(a, b) returns 200, printed as "Multiplication: 200".
// #6  Inside the try block: divide(a, b) returns 2.0 (20.0 / 10), printed
//     as "Division: 2".
// #7  divide(a, 0) is called next. Because its `b` parameter is 0, the
//     `if (b == 0)` check inside divide() throws a const char* literal
//     "Division by zero error!" BEFORE it can return anything - the
//     "Division by zero test: " cout statement never finishes printing a
//     number.
// #8  The thrown const char* is caught by `catch (const char* msg)`, and
//     "Error: Division by zero error!" is printed to cerr.
// #9  main() returns 0.
//
// Static linking, in plain language: everything add()/subtract()/
// multiply()/divide() DO is baked directly into this program's .exe file
// at build time. There is no separate rrmathlib.dll (or .so) that has to
// be found, loaded, and matched up at run time - the executable is bigger
// on disk but has no runtime dependency on the math library ever being
// present again. Contrast this with dynamicLinking\1_main.cpp, where the
// exact same add/subtract/multiply/divide calls instead jump into code
// that lives in a SEPARATE rrmathlib.dll file, loaded by the OS when the
// program starts (or on first use, depending on load mode) - the .exe
// itself stays small, but rrmathlib.dll must ship alongside it and be
// locatable at run time, or the program fails to run.
// ============================================================================