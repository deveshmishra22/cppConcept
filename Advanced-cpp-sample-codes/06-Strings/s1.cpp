// String manipulation in C++ using C++17 standard
// This program demonstrates various string operations such as concatenation, substring extraction, and searching.
// What is a string? In C++, a string is a sequence of characters used to represent text. The C++ Standard Library provides the `std::string` class, which offers a convenient way to work with strings, including dynamic sizing and various member functions for manipulation.
// Analogy: Think of a string as a necklace made up of individual beads (characters). You can add more beads, remove some, or rearrange them to create different patterns (manipulate the string).

#include <iostream>
#include <string>
using namespace std;

int main() {
    // Creating and initializing strings
    string str1 = "Hello, ";
    string str2 = "World!";
    
    // Concatenation
    string str3 = str1 + str2;
    cout << "Concatenated String: " << str3 << endl;

    // Substring extraction
    string subStr = str3.substr(7, 5); // Extract "World"
    cout << "Extracted Substring: " << subStr << endl;

    // Searching for a substring
    size_t pos = str3.find("World");
    if (pos != string::npos) {
        cout << "'World' found at position: " << pos << endl;
    } else {
        cout << "'World' not found!" << endl;
    }

    // accessing characters using indexing
    cout << "First character of str3: " << str3[0] << endl;
    cout << "str3[9]: " << str3[9] << endl;

    // String length
    cout << "Length of str3: " << str3.length() << endl;

    // Iterating through characters
    cout << "Characters in str3: ";
    for (char c : str3) {
        cout << c << ' ';
    }
    cout << endl;

    return 0;
}