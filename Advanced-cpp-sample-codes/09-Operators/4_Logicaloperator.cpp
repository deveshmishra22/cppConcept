#include <iostream>

using namespace std;
 
int main() {

    bool result;
    cout << boolalpha; // Print bool values as true/false
 
    result = (3 != 5) && (3 < 5);     // true

    cout << "(3 != 5) && (3 < 5) is " << result << endl;
 
    result = (3 == 5) && (3 < 5);    // false

    cout << "(3 == 5) && (3 < 5) is " << result << endl;
 
    result = (3 == 5) && (3 > 5);    // false

    cout << "(3 == 5) && (3 > 5) is " << result << endl;
 
    result = (3 != 5) || (3 < 5);    // true

    cout << "(3 != 5) || (3 < 5) is " << result << endl;
 
    result = (3 != 5) || (3 > 5);    // true

    cout << "(3 != 5) || (3 > 5) is " << result << endl;
 
    result = (3 == 5) || (3 > 5);    // false

    cout << "(3 == 5) || (3 > 5) is " << result << endl;
 
    result = !(5 == 2);    // true

    cout << "!(5 == 2) is " << result << endl;
 
    result = !(5 == 5);    // false

    cout << "!(5 == 5) is " << result << endl;
 
    return 0;

}

 