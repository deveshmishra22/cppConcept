// In C++, operators like +=, -=, *=, /=, and %= are known as compound assignment operators. 
//They provide a shorthand way to perform an arithmetic operation on a variable and then assign the result back to the same variable.

#include <iostream>

using namespace std;
 
int main() {

    int a, b;
 
    // 4 is assigned to a

    a = 4;
 
    // 25 is assigned to b

    b = 25;
 
    cout << "a = " << a << endl;

    cout << "b = " << b << endl;

    cout << "\nAfter a += b;" << endl;
 
    // assigning the sum of a and b to a

    a += b;  // a = a +b

    cout << "a = " << a << endl;
 
    return 0;

}
 
 