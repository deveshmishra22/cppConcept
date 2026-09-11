#include "cpp-codes\\Day4\\LinkingMulModule\\header\\rrmathlib.h"
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
