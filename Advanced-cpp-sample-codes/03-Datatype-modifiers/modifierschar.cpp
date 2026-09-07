#include <iostream>

using namespace std;
 
int main() {

    char b = 'B';              // signed by default (−128 to 127)

    unsigned char d = 200;     // range 0–255

    signed char f = -50;       // explicit signed char
 
    cout << "char: " << b << " (" << int(b) << ")" << endl;

    cout << "unsigned char: " << int(d) << endl;

    cout << "signed char: " << int(f) << endl;
 
    return 0;

}

 