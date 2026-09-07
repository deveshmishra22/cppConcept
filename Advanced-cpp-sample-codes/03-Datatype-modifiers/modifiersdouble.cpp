#include <iostream>

#include <iomanip> // for setprecision

using namespace std;
 
int main() {

    float f = 3.14159f;

    double d = 3.1415926535;

    long double ld = 3.141592653589793238L;
  
    cout << "float: " << f << endl;
    cout << fixed << setprecision(10); // show 15 digits

    cout << "double: " << d << endl;
    cout << fixed << setprecision(15); // show 15 digits

    cout << "long double: " << ld << endl;
 
    return 0;

}

 