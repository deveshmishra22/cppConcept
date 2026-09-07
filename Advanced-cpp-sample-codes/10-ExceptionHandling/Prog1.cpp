#include<iostream>
#include<stdexcept>
using namespace std;

double Divide(double a, double b) {
    if (b == 0) {
        throw "Division by zero error"; // Throwing a string literal as an exception
    }
    return a / b;
}

void FunctionThatThrows(string n) {
    if(n!="Raushan"){
        throw 401; // Throwing an integer as an exception
    }
    else{
        cout << "Welcome " << n << "! Happy to see you back." << endl;
    }
}

// Runtime Exception Handling using Standard Exceptions
void StandardExceptionDemo() {
    try {
        int* arr = new int[5];
        // Accessing out of bounds to trigger an exception
        for (int i = 0; i <= 5; i++) {
            arr[i] = i;
        }
        delete[] arr;
    }
    catch (const std::out_of_range& e) {
        cerr << "Out of Range error: " << e.what() << endl;
    }
    catch (const std::exception& e) {
        cerr << "Standard exception: " << e.what() << endl;
    }
}
int main() {
    int num1, num2;
    cout << "Enter two integers: ";
    cin >> num1 >> num2;

    try {
        double result = Divide(num1, num2);
        cout << "Result: " << result << endl;
    }
    catch (const char* msg) { // Catching the string literal exception
        cerr << "Error: " << msg << endl;
    }

    cout << "Demonstrating with exception Program didn't stopped." << endl;
    
    cout << "Now demonstrating another exception type." << endl;
    try {
        cout << "Enter name: ";
        string name;
        cin >> name;
        FunctionThatThrows(name);
    }
    catch (int e) { // Catching the integer exception
        cerr << "Error Code: " << e << endl;
    }
    cout << "Demonstrating with exception Program didn't stopped." << endl;
    return 0;
}