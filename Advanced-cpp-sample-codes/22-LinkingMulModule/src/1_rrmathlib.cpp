#include "rrmathlib.h"
// --- Added explanation: add(a, b) ------------------------------------------
// Generic syntax:  int add( int a, int b )
//   a - parameter #1: left-hand operand, forwarded straight through from
//       whichever caller invoked add() (e.g. main.cpp's a = 20).
//   b - parameter #2: right-hand operand, forwarded straight through from
//       the caller (e.g. main.cpp's b = 10).
// This is the DEFINITION matching the declaration in header/rrmathlib.h;
// it lives in its own translation unit (this .cpp file), gets compiled into
// its own object file, and is resolved into main.cpp's call at link time.
int add(int a, int b) {
    return a + b;
}
// --- Added explanation: subtract(a, b) -------------------------------------
// Generic syntax:  int subtract( int a, int b )
//   a - parameter #1: the value being subtracted from (e.g. 20).
//   b - parameter #2: the value being subtracted (e.g. 10).
int subtract(int a, int b) {
    return a - b;
}   
// --- Added explanation: multiply(a, b) -------------------------------------
// Generic syntax:  int multiply( int a, int b )
//   a - parameter #1: the first factor (e.g. 20).
//   b - parameter #2: the second factor (e.g. 10).
int multiply(int a, int b) {
    return a * b;
}
// --- Added explanation: divide(a, b) ---------------------------------------
// Generic syntax:  double divide( int a, int b )
//   a - parameter #1: the numerator, cast to double below so the division
//       is done with floating-point precision instead of truncating
//       integer division (e.g. 20).
//   b - parameter #2: the denominator; checked against 0 first because
//       dividing by 0 is undefined for integers and would otherwise crash -
//       so instead this throws a const char* error message that the caller
//       (main.cpp) is expected to catch (e.g. 10 on the first call, 0 on
//       the second call to deliberately exercise this throw path).
double divide(int a, int b) {
    if (b == 0) {
        throw "Division by zero error!";
    }
    return static_cast<double>(a) / b;
}
