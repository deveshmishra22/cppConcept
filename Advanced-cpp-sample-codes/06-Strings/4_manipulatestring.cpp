// Find, substring, replace, insert, erase in C++ strings
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Hello, World! Welcome to C++ programming.";

    // Find substring
    int pos = str.find("World");
    cout << pos << endl; // prints the index of the first occurrence of "World" or -1 if not found
    cout << string::npos << endl; // prints the maximum value of size_t, which is used to indicate "not found"
    if (pos != string::npos) {
        // this condition should be always true as -1 is never equals to 
        // string::npos, but we are checking for the sake of demonstration
        // is this getting implicitly converted to size_t? 
        // Yes, the comparison between pos (int) and string::npos (size_t) 
        // involves an implicit conversion of pos to size_t for the comparison. 
        // This is because string::npos is of type size_t, and when comparing 
        // different types, C++ performs implicit type conversions to ensure both 
        // operands are of the same type. In this case, pos is converted to 
        // size_t before the comparison.
        cout << "'World' found at position: " << pos << endl;
    }
    cout << "Word 'World' not found in the string." << endl;
    // why we used int not size_t? Because find() returns size_t, 
    // but we are using int for simplicity in this example. In production code, 
    // it's better to use size_t to avoid potential issues with large strings.

    // Replace substring
    // replace(para1, para2, para3) -> 
    // para1 = starting index, para2 = length of substring to replace, para3 = new substring
    str.replace(pos, 5, "Universe");
    cout << "After replacement: " << str << endl;

    // Insert substring
    // insert(para1, para2) -> para1 = starting index, para2 = substring to insert
    str.insert(0, "Greeting: ");
    cout << "After insertion: " << str << endl;

    // Erase substring
    // erase(para1, para2) -> para1 = starting index, para2 = length of substring to erase
    str.erase(0, 10); // Remove "Greeting: "
    cout << "After erasure: " << str << endl;

    cout << "--------------------------------------------" << endl;

    string s = "The quick brown fox jumps over the lazy dog";
 
    // find
    auto pos1 = s.find("fox");
    // what is auto? The auto keyword in C++ is used for type inference, 
    // allowing the compiler to automatically deduce the type of a variable from its initializer. 
    // In this case, pos1 will be of type size_t, which is the return type of the find() function.

    // can we use auto always or in specific use-cases? 
    // Yes, auto can be used in most cases where the type of the variable is clear from its initializer.
    // However, it is generally recommended to use auto when the type is complex or verbose, or 
    // when the exact type is not important for the context.
    if (pos1 != string::npos) cout << "'fox' at index: " << pos1 << '\n';
 
    // substring
    // substr(para1, para2) -> para1 = starting index, para2 = length of substring to extract
    string w = s.substr(pos1, 3); // "fox"
    cout << "substr: " << w << '\n';
 
    // replace: replace 'dog' with 'cat'
    // find(para1) -> para1 = substring to find
    pos1 = s.find("dog");
    if (pos1 != string::npos) s.replace(pos1, 3, "cat");
    cout << "After replace: " << s << '\n';
 
    // insert: insert "very " before 'lazy'
    pos1 = s.find("lazy");
    // insert(para1, para2) -> para1 = starting index, para2 = substring to insert
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
