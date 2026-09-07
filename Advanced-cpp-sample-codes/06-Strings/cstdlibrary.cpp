// cstlibrary.cpp
// Compile with: g++ -std=c++17 cstdlibrary.cpp -o cstd
// this library provides functions for converting strings to numbers and vice versa, as well as other string manipulations.
// Analogy: Think of these functions as translators that convert between different languages (data types).
// For example, converting a string of digits into an integer is like translating a written number into a spoken number.
// Real-world scenario: When reading user input from a console or a file, the input is often in string format. To perform calculations, you need to convert these strings into numeric types using functions like stoi (string to integer) or stod (string to double). Conversely, when displaying results, you might need to convert numbers back into strings using to_string.
// Note: These functions are part of the C++ Standard Library and are defined in the <string> header.
#include <iostream>
#include <string>
#include <cstdlib>    // strtol, strtod if needed
using namespace std;
 
int main() {

    string s = "12345";
    int x = stoi(s);                  // to int
    cout << "x = " << x << '\n';
    double d = stod("3.14159");
    cout << "d = " << d << '\n';
 
    // number to string
    string sx = to_string(42);
    cout << "sx = " << sx << '\n';
 
    // c_str()
    string str1 = "Hello, C++17!";
    const char* c = str1.c_str();
    cout << "C-string: " << c << '\n';
 
      // safe parsing with error handling
    try {
        int val = stoi("12ab"); // will parse up to 'a' and then stop? actually throws for invalid
        cout << val << '\n';
    } catch (const invalid_argument& e) {
        cout << "Invalid argument in stoi\n";
    } catch (const out_of_range& e) {
        cout << "Out of range\n";
    }
    return 0;

}

 