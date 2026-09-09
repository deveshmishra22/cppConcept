//In C++, the built-in relational operators (e.g., <, >, <=, >=, ==, !=) always return a value of type bool. 
//This bool value represents the truth or falsity of the comparison, being either true or false.

#include <iostream>
using namespace std;

int main() {
    int x = 10;
    int y = 20;

    // Using relational operators
    bool result1 = (x < y);   // true
    bool result2 = (x > y);   // false
    bool result3 = (x <= y);  // true
    bool result4 = (x >= y);  // false
    bool result5 = (x == y);  // false
    bool result6 = (x != y);  // true

    // Output the results
    cout << "x < y: " << result1 << endl;
    cout << "x > y: " << result2 << endl;
    cout << "x <= y: " << result3 << endl;
    cout << "x >= y: " << result4 << endl;
    cout << "x == y: " << result5 << endl;
    cout << "x != y: " << result6 << endl;
    
    cout << "===================================" <<endl;
    // Print bool values as true/false
    cout << boolalpha;

    // Output the results as true/false
    cout << "x < y: " << result1 << endl;
    cout << "x > y: " << result2 << endl;
    cout << "x <= y: " << result3 << endl;
    cout << "x >= y: " << result4 << endl;
    cout << "x == y: " << result5 << endl;
    cout << "x != y: " << result6 << endl;

    return 0;
}