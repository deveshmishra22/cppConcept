// mathlib.h
// --- Added explanation: add(a, b) declaration -----------------------------
// Generic syntax:  int add( int a, int b )
//   a - parameter #1: first operand a caller wants added (e.g. 20).
//   b - parameter #2: second operand a caller wants added (e.g. 10).
//   A caller passes two ints it wants summed; add() returns their sum.
int add(int a, int b);
// --- Added explanation: subtract(a, b) declaration ------------------------
// Generic syntax:  int subtract( int a, int b )
//   a - parameter #1: the value a caller wants to subtract FROM.
//   b - parameter #2: the value a caller wants subtracted.
//   A caller passes the minuend and subtrahend; subtract() returns a - b.
int subtract(int a, int b);
// --- Added explanation: multiply(a, b) declaration ------------------------
// Generic syntax:  int multiply( int a, int b )
//   a - parameter #1: first operand a caller wants multiplied.
//   b - parameter #2: second operand a caller wants multiplied.
//   A caller passes two ints it wants the product of.
int multiply(int a, int b);
// --- Added explanation: divide(a, b) declaration --------------------------
// Generic syntax:  double divide( int a, int b )
//   a - parameter #1: the dividend/numerator a caller wants divided.
//   b - parameter #2: the divisor/denominator; a caller must NOT pass 0
//       here without wrapping the call in a try/catch, since divide()
//       throws a const char* on division by zero instead of returning.
double divide(int a, int b);

// ============================================================================
// Added: what this header exposes, and why the library is split this way
// ============================================================================
// This header declares the four functions that make up "rrmathlib" - the
// small math library this folder uses to demonstrate static vs dynamic
// linking. It contains ONLY declarations (function signatures), no
// function bodies/definitions - those live in 2_rrmathlib.cpp instead.
// Splitting declarations (header) from definitions (.cpp) is what lets the
// SAME rrmathlib.h be #included by 1_main.cpp whether rrmathlib's compiled
// code ends up statically copied into the executable (this folder) or
// loaded from a separate dynamic library at runtime (see dynamicLinking\) -
// main.cpp only ever needs to know these functions' signatures, not how or
// where their bodies eventually get linked in.
// ============================================================================
