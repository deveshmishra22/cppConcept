/*
Preprocessor Macros (#define)
The #define directive is a legacy C-style preprocessor feature that performs simple text replacement across your source code before the compiler actually runs.
Because it operates purely on raw text substitution, it completely lacks type safety, ignores C++ scoping rules and namespaces, and often creates subtle,
difficult-to-trace bugs. In modern C++, #define should generally be avoided for constants and functions, remaining useful primarily for conditional compilation
and header guards.

Compile-Time Capable (constexpr)
Introduced in C++11, constexpr tells the compiler that a variable or function is capable of being evaluated at compile time whenever its inputs are compile-time constants.
Its primary advantage is versatility: if a constexpr function receives constant expressions as arguments, it executes at compile time to boost performance, 
but if it receives runtime variables, it seamlessly falls back to normal runtime execution with full type safety and scope protection.

Guaranteed Compile-Time (consteval)
Introduced in C++20, consteval defines an "immediate function" that guarantees strict execution at compile time. Unlike constexpr, a consteval function completely 
forbids runtime evaluation; if you attempt to pass a runtime variable into it, the compiler will generate a hard error instead of falling back to runtime execution.
This ensures that a function is never accidentally called during program execution.
*/

#include <iostream>
using namespace std;

constexpr int square(int x) {
    return x * x;
}

int main() {
    constexpr int result = square(5);
    cout << "The square of 5 is: " << result << endl;
    
    cout<< "Calling square func temp: " << endl;
    cout<< "The square of 10 is: " << square(10) << endl;
    return 0;
}


// #include <iostream>
// #include <chrono>
// using namespace std;
// using namespace std::chrono;

// constexpr int square(int x) {
//     return x * x;
// }

// int runtime_square(int x) {
//     return x * x;
// }

// int main() {
//     // Compile-time computation
//     constexpr int result = square(5);
//     cout << "Compile-time square of 5: " << result << endl;

//     // Runtime computation (constexpr used at runtime)
//     auto start1 = high_resolution_clock::now();
//     int val1 = square(1000000);
//     auto end1 = high_resolution_clock::now();
//     cout << "Runtime constexpr square(1000000): " << val1 << endl;
//     cout << "Time taken: " 
//          << duration_cast<nanoseconds>(end1 - start1).count() 
//          << " ns" << endl;

//     // Regular function
//     auto start2 = high_resolution_clock::now();
//     int val2 = runtime_square(1000000);
//     auto end2 = high_resolution_clock::now();
//     cout << "Normal square(1000000): " << val2 << endl;
//     cout << "Time taken: " 
//          << duration_cast<nanoseconds>(end2 - start2).count() 
//          << " ns" << endl;

//     return 0;
// }
