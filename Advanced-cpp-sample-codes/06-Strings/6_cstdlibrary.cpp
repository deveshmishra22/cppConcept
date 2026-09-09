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

// Explanation:
// In this code, we demonstrate the use of C++ Standard Library functions for string manipulation and
// conversion between strings and numeric types. We use `stoi` to convert a string to an integer, `stod` to convert 
// a string to a double, and `to_string` to convert a number back to a string. We also show how to obtain a C-style string
// using the `c_str()` method of the `std::string` class. Additionally, we demonstrate error handling when 
// converting strings to numbers, catching exceptions for invalid arguments and out-of-range values.
// Real-world scenario: These functions are commonly used when processing user input, reading data from files, or 
// performing calculations that require numeric values. Proper error handling ensures that the program can gracefully handle 
// unexpected input without crashing.
 
// Analogy: Think of these functions as translators that convert between different languages (data types).
// For example, converting a string of digits into an integer is like translating a written number into a spoken number.

// can we do the same with c_str() in c++17?
// Yes, the `c_str()` method is available in C++17 and can be used to obtain a C-style string 
// (null-terminated character array) from a `std::string` object. The method returns a pointer to the 
// internal character array of the string, which can be used in functions that require C-style strings. 
// However, it's important to note that the pointer returned by `c_str()` is only valid as long as the `std::string` 
// object is not modified or destroyed.


// can we do typecast into any type in c++17?
// In C++17, you can perform type casting to convert between different data types, but there are some rules and limitations. 
// C++ provides several casting operators, including `static_cast`, `dynamic_cast`, `const_cast`, and `reinterpret_cast`,
// each serving different purposes. However, not all type conversions are valid or safe. For example, 
// you cannot directly cast a string to an integer without using a conversion function like `stoi`. Similarly, 
// casting between unrelated pointer types can lead to undefined behavior. Always ensure that the type conversion is 
// meaningful and safe in the context of your program.

// do we have like stod or stof for converting string to double or float in c++17?
// Yes, in C++17, you have functions like `stod` (string to double) and `stof` (string to float) for converting strings to 
// floating-point numbers. These functions are part of the C++ Standard Library and are defined in the `<string>` header.