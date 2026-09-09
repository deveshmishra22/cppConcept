#include "rrmathlib.h"
#include <iostream>
using namespace std;

int main()
{
    int a = 20, b = 10;

    cout << "Static vs Dynamic Linking in C++" << endl;

    // Using functions from the math library
    cout << "Addition: " << add(a, b) << endl;
    cout << "Subtraction: " << subtract(a, b) << endl;
    cout << "Multiplication: " << multiply(a, b) << endl;
    try {
        cout << "Division: " << divide(a, b) << endl;
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