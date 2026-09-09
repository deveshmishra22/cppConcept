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
 

// how many levels of indirection are there in the above code?
// In the provided code, there are three levels of indirection:1. The variable `a` is a simple integer variable.
// 2. The pointer `p` points to the integer variable `a`, which is the first level of indirection.
// 3. The pointer `q` points to the pointer `p`, which is the second level of indirection.
// 4. The pointer `x` points to the pointer `q`, which is the third level of indirection.

// how many levels of indrection is allowed in c++?
// In C++, there is no strict limit on the number of levels of indirection (i.e., pointers to pointers) that you can use. 
// You can create pointers to pointers to pointers, and so on, as long as it makes sense in your 
// program and you manage memory correctly. However, excessive levels of indirection can make code 
// harder to read and maintain, so it's generally advisable to keep the levels of indirection to a 
// reasonable number.