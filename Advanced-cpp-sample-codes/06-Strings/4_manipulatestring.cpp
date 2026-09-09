// Find, substring, replace, insert, erase in C++ strings
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Hello, World! Welcome to C++ programming.";

    // Find substring
    int pos = str.find("World");
    if (pos != string::npos) {
        cout << "'World' found at position: " << pos << endl;
    }

    // Replace substring
    str.replace(pos, 5, "Universe");
    cout << "After replacement: " << str << endl;

    // Insert substring
    str.insert(0, "Greeting: ");
    cout << "After insertion: " << str << endl;

    // Erase substring
    str.erase(0, 10); // Remove "Greeting: "
    cout << "After erasure: " << str << endl;

    cout << "--------------------------------------------" << endl;

    string s = "The quick brown fox jumps over the lazy dog";
 
    // find
    auto pos1 = s.find("fox");
    if (pos1 != string::npos) cout << "'fox' at index: " << pos1 << '\n';
 
    // substring
    string w = s.substr(pos1, 3); // "fox"
    cout << "substr: " << w << '\n';
 
    // replace: replace 'dog' with 'cat'
    pos1 = s.find("dog");
    if (pos1 != string::npos) s.replace(pos1, 3, "cat");
    cout << "After replace: " << s << '\n';
 
    // insert: insert "very " before 'lazy'
    pos1 = s.find("lazy");
    if (pos1 != string::npos) s.insert(pos1, "very ");
    cout << "After insert: " << s << '\n';
 
    // erase: remove 'brown '
    pos1 = s.find("brown ");
    if (pos1 != string::npos) s.erase(pos1, 6);
    cout << "After erase: " << s << '\n';

    return 0;
}

// Explanation:
// In this code, we demonstrate various string manipulation functions in C++. We use `find` to locate a substring within a string, 
// `replace` to replace a portion of the string with another substring, `insert` to add a substring at a specific position, 
// and `erase` to remove a portion of the string. These functions are part of the C++ Standard Library and provide powerful 
// tools for working with strings.
// Real-world scenario: String manipulation is essential in many applications, such as text processing, data parsing, and 
// user input handling. Understanding how to effectively manipulate strings allows developers to create more dynamic and 
// responsive programs.
// Analogy: Think of string manipulation functions as tools in a toolbox. Each function serves a specific purpose, allowing 
// you to modify and work with strings in various ways, much like how different tools are used for different tasks in 
// construction or repair work.
// Note: The output of the program will show the results of each string manipulation operation, demonstrating how the 
// original string is modified step by step.
