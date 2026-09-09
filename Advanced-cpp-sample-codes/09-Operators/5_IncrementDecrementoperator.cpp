// Pre-increment will increment the value before it is used in the expression
// Post-increment will increment the value after it is used in the expression
// Similarly for pre-decrement will decrement the value before it is used in the expression
// Post-decrement will decrement the value after it is used in the expression



//Exercise 1
#include <iostream>
using namespace std;

int main() {
    int a = 5;
    cout << "Initial value of a: " << a << endl;

    // Pre-increment
    cout << "Pre-increment: " << ++a << endl; // a becomes 6, then used
    cout << "Value of a after pre-increment: " << a << endl;

    // Post-increment
    cout << "Post-increment: " << a++ << endl; // a used as 6, then becomes 7
    cout << "Value of a after post-increment: " << a << endl;

    // Pre-decrement
    cout << "Pre-decrement: " << --a << endl; // a becomes 6, then used
    cout << "Value of a after pre-decrement: " << a << endl;

    // Post-decrement
    cout << "Post-decrement: " << a-- << endl; // a used as 6, then becomes 5
    cout << "Value of a after post-decrement: " << a << endl;

    return 0;
}   


// //Exercise 2
// #include <iostream>

// using namespace std;
 
// int main() {

//     int a = 200, b = 300, result_x, result_y;
 
//     // incrementing a by 1 and storing the result in result_x

//     result_x = ++a;

//     cout << "result_x = " << result_x << endl;
 
 
//     // decrementing b by 1 and storing the result in result_y   

//     result_y = --b;

//     cout << "result_y = " << result_y << endl;
 
//     return 0;

// }

 