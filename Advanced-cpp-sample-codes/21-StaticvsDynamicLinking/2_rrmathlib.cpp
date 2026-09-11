#include "rrmathlib.h"
// --- Added explanation: add(a, b) -----------------------------------------
// Generic syntax:  int add( int a, int b )
//   a - parameter #1: first operand to add.
//   b - parameter #2: second operand to add.
//   Returns a + b.
int add(int a, int b) {
    return a + b;
}
// --- Added explanation: subtract(a, b) ------------------------------------
// Generic syntax:  int subtract( int a, int b )
//   a - parameter #1: the value to subtract FROM (the minuend).
//   b - parameter #2: the value being subtracted (the subtrahend).
//   Returns a - b.
int subtract(int a, int b) {
    return a - b;
}
// --- Added explanation: multiply(a, b) ------------------------------------
// Generic syntax:  int multiply( int a, int b )
//   a - parameter #1: first operand to multiply.
//   b - parameter #2: second operand to multiply.
//   Returns a * b.
int multiply(int a, int b) {
    return a * b;
}
// --- Added explanation: divide(a, b) --------------------------------------
// Generic syntax:  double divide( int a, int b )
//   a - parameter #1: the dividend/numerator.
//   b - parameter #2: the divisor/denominator. If this is 0, the function
//       throws a const char* literal instead of returning, since integer
//       division by zero has no valid numeric result.
//   Returns static_cast<double>(a) / b when b != 0.
double divide(int a, int b) {
    if (b == 0) {
        throw "Division by zero error!";
    }
    return static_cast<double>(a) / b;
}

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  This file has no main() of its own - it is a LIBRARY source file. It
//     is compiled to an object file and either archived into a STATIC
//     library (e.g. librrmathlib.a / rrmathlib.a), or compiled together
//     with 1_main.cpp in one command (`g++ 1_main.cpp 2_rrmathlib.cpp -o
//     main`), per the notes at the bottom of 1_main.cpp in this folder.
// #2  Whichever way it's built, add()/subtract()/multiply()/divide()'s
//     compiled machine code ends up COPIED into the final `main`
//     executable at link time - that is what "static" linking means here.
// #3  At run time, when 1_main.cpp calls add(a, b) (or subtract/multiply/
//     divide), control jumps to this code directly inside the SAME
//     process image - there is no separate library file to locate or
//     load; the code is already physically part of main.exe.
// #4  divide() is the one function with a runtime decision: if its second
//     argument is 0, it throws a const char* instead of returning a
//     double, letting the caller's try/catch (in 1_main.cpp) handle the
//     error instead of crashing or returning a meaningless value.
// #5  Contrast with dynamicLinking\2_rrmathlib.cpp: the source code there
//     is identical, but it gets compiled into a separate rrmathlib.dll
//     that is loaded by the OS at/before run time rather than copied into
//     the executable - see the note at the end of dynamicLinking\1_main.cpp
//     for the full contrast.
// ============================================================================
