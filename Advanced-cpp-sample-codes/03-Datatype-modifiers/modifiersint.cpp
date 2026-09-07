#include <iostream>

using namespace std;
 
int main() {

    short int a = 32000;         // short: typically 2 bytes

    int b = 2147483647;          // int: typically 4 bytes

    long int c = 2147483647L;    // long: 4 or 8 bytes (platform dependent)

    long long int d = 9223372036854775807LL; // long long: 8 bytes
 
    unsigned int e = 4294967295; // unsigned: 0 to 4,294,967,295 (no negatives)
    
    signed int f = -2147483648; // signed: -2,147,483,648 to 2,147,483,647

    cout << "short int: " << a << endl;

    cout << "int: " << b << endl;

    cout << "long int: " << c << endl;

    cout << "long long int: " << d << endl;

    cout << "unsigned int: " << e << endl;
 
    return 0;

}

 