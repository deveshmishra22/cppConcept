#include<iostream>
#include<string>
#include<algorithm>
#include<cctype>
using namespace std;

int main(){
    string str1;
    cout << "Enter a string: ";
    getline(cin, str1); // Using getline to read string with spaces
    cout << "You entered: " << str1 << endl;

    string str2;
    cout << "Enter another string: ";
    cin >> str2; // Using cin to read string without spaces
    cout << "You entered: " << str2 << endl;

    // Comparing strings
    cout << "Comparing the two strings..." << endl;
    cout << boolalpha; // to print bool as true/false
    cout << "str1 == str2: " << (str1 == str2) << endl;

    cout << "string1.compare(string2): " << str1.compare(str2) << endl; // 0 if equal, <0 if str1<str2, >0 if str1>str2
    
    // case-sensitive comparison
    string str3 = str1, str4 = str2;
    std::transform(str3.begin(), str3.end(), str3.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    std::transform(str4.begin(), str4.end(), str4.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    cout << "Case-insensitive comparison (after converting to lowercase): " << (str3 == str4) << endl;
    cout << "----------------------------------------" << endl;
    

    return 0;
}

// When to use a.compare(b) vs a==b?
// a.compare(b) provides more detailed information about the lexicographical relationship between the strings (
// whether one is less than, equal to, or greater than the other), while a==b simply checks for equality.
// Use a.compare(b) when you need to know the ordering of the strings, and use a==b when you only need to check if they are identical.
// Analogy: Think of a.compare(b) like comparing two books to see which one comes first in alphabetical order, while a==b is like checking if two books are exactly the same edition.
// Note: The comparison is case-sensitive. "Hello" and "hello" are considered different strings.
// For case-insensitive comparison, you can convert both strings to the same case (either upper or lower) before comparing.
// Example of case-insensitive comparison is shown above using std::transform and std::tolower.
// Additional string comparison methods:
// 1. Using relational operators: <, >, <=, >=
// 2. Using std::equal from <algorithm> for custom comparison logic
// 3. Using locale-aware comparison with std::use_facet and std::collate for internationalization support
// Remember to include <algorithm> and <cctype> for case conversion functions.
