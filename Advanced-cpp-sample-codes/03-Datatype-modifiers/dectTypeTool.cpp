#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;


template<typename T3, typename T4>
auto add(T3 a, T4 b) -> decltype(a + b) {
    return a + b;
}

int main() {
    auto start1 = high_resolution_clock::now();
    auto val1 = add(10, 12.5);
    auto end1 = high_resolution_clock::now();
    cout << "Result of add(10, 12.5): " << val1 << endl;
    cout << "Time taken: " 
         << duration_cast<nanoseconds>(end1 - start1).count() 
         << " ns" << endl;

    return 0;
}

// What is the meaning of decltype in C++?
// decltype is a keyword in C++ that is used to query the type of an expression at compile time.
// It allows you to declare variables or return types based on the type of an existing expression without explicitly specifying the type.
// How it works?
// When you use decltype with an expression, the compiler evaluates the expression and deduces its type.
// This is particularly useful in templates and generic programming where the types may not be known in advance
// or when you want to ensure that a variable has the same type as another variable or expression.
// Example:
// int x = 10;
// decltype(x) y = 20; // y will be of type int
// In this example, decltype(x) deduces the type of x (which is int
// and declares y to be of the same type.
// Another example with functions:
// template<typename T1, typename T2>
// auto add(T1 a, T2 b) -> decltype(a + b) {
//     return a + b;
// }
// In this function template, decltype(a + b) deduces the return type based on
// the types of a and b, allowing the function to work with different types seamlessly.
// Use cases of decltype:
// 1. Generic Programming: In templates to deduce types based on expressions.
// 2. Type Safety: To ensure variables have the same type as other expressions.
// 3. Complex Types: When dealing with complex types or expressions where the type is not
// straightforward to determine.
// Is this same as auto keyword?
// decltype and auto are related but serve different purposes.
// auto is used for type inference when declaring variables, allowing the compiler to deduce the type
// based on the initializer expression.
// decltype, on the other hand, is used to query the type of an expression without creating a variable.
// They can be used together effectively, especially in template programming.
// Example combining both:
// template<typename T1, typename T2>
// auto add(T1 a, T2 b) -> decltype(a + b) {
//     return a + b;
// }
// In this example, auto is used to declare the function return type, allowing the function to work with different types seamlessly.
// Conclusion:
// decltype is a powerful feature in C++ that enhances type safety and flexibility in generic programming
// by allowing you to deduce types based on expressions at compile time.
