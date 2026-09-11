// mathlib.h
// --- Added explanation: add(a, b) ------------------------------------------
// Generic syntax:  int add( int a, int b )
//   a - parameter #1: the value the caller wants as the left-hand operand
//       of the addition (main.cpp passes 20, its local `a`, here).
//   b - parameter #2: the value the caller wants as the right-hand operand
//       of the addition (main.cpp passes 10, its local `b`, here).
// This is only the DECLARATION - it tells any .cpp file that #includes this
// header what add()'s name, parameter types, and return type are, so that
// file can call add() even though its actual body lives in a different
// .cpp file (src/1_rrmathlib.cpp) that gets compiled and linked separately.
int add(int a, int b);
// --- Added explanation: subtract(a, b) -------------------------------------
// Generic syntax:  int subtract( int a, int b )
//   a - parameter #1: the value to subtract FROM (main.cpp passes 20 here).
//   b - parameter #2: the value being subtracted (main.cpp passes 10 here).
int subtract(int a, int b);
// --- Added explanation: multiply(a, b) -------------------------------------
// Generic syntax:  int multiply( int a, int b )
//   a - parameter #1: the first factor to multiply (main.cpp passes 20 here).
//   b - parameter #2: the second factor to multiply (main.cpp passes 10 here).
int multiply(int a, int b);
// --- Added explanation: divide(a, b) ---------------------------------------
// Generic syntax:  double divide( int a, int b )
//   a - parameter #1: the numerator/dividend a caller should pass
//       (main.cpp passes 20 here).
//   b - parameter #2: the denominator/divisor a caller should pass; the
//       definition in src/1_rrmathlib.cpp throws a const char* if this is 0
//       (main.cpp passes 10 for the first call and 0 for the second, to
//       deliberately trigger that error path).
double divide(int a, int b);
