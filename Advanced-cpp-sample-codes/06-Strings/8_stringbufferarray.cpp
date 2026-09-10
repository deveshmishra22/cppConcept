// c string interoperability using string buffer and array
#include <iostream>
#include <string>
#include <cstring> // for std::strlen, std::strcpy, std::strcat
using namespace std;

int main() 
{
    char buffer[100]; // Create a character buffer (C-style string)
    strcpy(buffer, "Hello, C-Style String!"); // Copy a C-style string into the buffer
    cout << "C-Style String: " << buffer << endl;
    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination
    cout << "buffer: "<< buffer << endl;

    //safer copy: strncpy ensures no buffer overflow
    char safeBuffer[100];
    strncpy(safeBuffer, buffer, sizeof(safeBuffer) - 1);
    safeBuffer[sizeof(safeBuffer) - 1] = '\0'; // Ensure
    cout << "Safe Buffer: " << safeBuffer << endl;

    // concatenate using strcat
    strcat(safeBuffer, " - Appended Text");
    cout << "After strcat: " << safeBuffer << endl;

    //compare using strcmp
    if (strcmp(safeBuffer, "Hello, C-Style String! - Appended Text") == 0) {
        cout << "Strings are equal." << endl;
    } else {
        cout << "Strings are not equal." << endl;
    }

    // strcmp vs (s.compare) vs (==) in c++ strings?
    // strcmp is a C-style function that compares two C-style strings and returns an integer.
    // s.compare() is a C++ string method that compares two C++ strings and returns an integer.
    // == is the equality operator for C++ strings, which returns a boolean value.

    // what is the differemce between c-style string vs c++ string?
    // A C-style string is a null-terminated array of characters, 
    // while a C++ string is an object of the std::string class that provides a more 
    // convenient and safer way to work with strings. 
    // Which one to use depends on the context and requirements of your program. 

    // For example, if you are working with legacy C code or need to interface with C libraries,
    // you may need to use C-style strings. However, if you are writing modern C++ code, 
    // it is generally recommended to use C++ strings for their safety and convenience.

    // Initialize a C++ string
    string cppStr = "Hello, C++ String!";
    cout << "C++ String: " << cppStr << endl;

    // Convert C++ string to C-style string (char array)
    const char* cStr = cppStr.c_str();
    cout << "C-style String (from C++ string): " << cStr << endl;

    // Create a char array (C-style string) and copy the C++ string into it
    char charArray[50];
    strcpy(charArray, cStr);
    cout << "Char Array after strcpy: " << charArray << endl;

    // Modify the char array
    strcat(charArray, " - Modified in C-style");
    cout << "Char Array after strcat: " << charArray << endl;

    // Convert back to C++ string
    string newCppStr(charArray);
    cout << "New C++ String (from Char Array): " << newCppStr << endl;

    newCppStr.replace(newCppStr.find("C"), 1, "P");
    // replace(para1, para2, para3) 
    // -> para1 = starting index, para2 = length of substring to replace, para3 = new substring
    cout << "After replace in C++ String: " << newCppStr << endl;
    return 0;
}