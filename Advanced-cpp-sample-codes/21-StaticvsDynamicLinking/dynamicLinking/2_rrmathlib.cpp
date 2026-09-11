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
// #1  This file has no main() of its own - it is compiled into a DYNAMIC
//     library (shared object) with `g++ -shared -o rrmathlib.dll
//     rrmathlib.cpp` (see the comments at the bottom of 1_main.cpp in this
//     same folder), producing rrmathlib.dll as a separate file on disk.
// #2  add()/subtract()/multiply()/divide()'s compiled machine code lives
//     ONLY inside rrmathlib.dll - it is never copied into main.exe. That
//     is what "dynamic" linking means here.
// #3  At run time, when 1_main.cpp calls add(a, b) (or subtract/multiply/
//     divide), the OS loader locates rrmathlib.dll, resolves the symbol,
//     and jumps into THIS code running inside the dll - connected at
//     load/run time rather than at compile time.
// #4  divide() still has the same runtime decision as the static version:
//     a second argument of 0 makes it throw a const char* instead of
//     returning a double, so 1_main.cpp's try/catch can handle the error.
// #5  Contrast with ..\2_rrmathlib.cpp (the static version, one folder
//     up): the source code is byte-for-byte the same, but there it gets
//     copied directly into main.exe at link time instead of living in a
//     separate .dll loaded at run time - see the note at the end of
//     ..\1_main.cpp for the full contrast.
// ============================================================================
