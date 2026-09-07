#include <iostream>
using namespace std;
 
int a = 5;
int* p = &a;
int** q = &p;
int *** x = &q;

int main() {
    cout << "Value of a: " << a << endl;
    cout << "Value via *p: " << *p << endl;
    cout << "Value via **q: " << **q << endl;
    cout << "Value via ***x: " << ***x << endl;
return 0;
}
 