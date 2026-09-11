#include <iostream>

using namespace std;

// declaring a function
void greet() {
    cout << "Hello from greet() function!";
}

double AddAreaofCircle(int radius) {
    return 3.14 * radius * radius;
}

int Add(int a, int b) {
    return a + b;
}   

int Add(int a, int b, int c) {
    return a + b + c;
}

double Add(double x, double y) {
    return x + y;
}

void PrintValue(double x){
    cout << "Value is: " << x << endl;
}

int main() {

    int n = 11;
    PrintValue(n);
    // calling the function
    // greet();

    // int radius;
    // cout << "\nEnter radius of circle: ";
    // cin >> radius;
    // double area = AddAreaofCircle(radius);
    // cout << "Area of circle with radius " << radius << " is: " << area << endl;
    
    // int sum1 = Add(5, 10);
    // cout << "Sum of 5 and 10 is: " << sum1 << endl;
    // int sum2 = Add(5, 10, 15);
    // cout << "Sum of 5, 10 and 15 is: " << sum2 << endl;
    // double sum3 = Add(5.5, n);
    // cout << "Sum of 5.5 and 10.3 is: " << float(sum3) << endl;

    return 0;
}


 