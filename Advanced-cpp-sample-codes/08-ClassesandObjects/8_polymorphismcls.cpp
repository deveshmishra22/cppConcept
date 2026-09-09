// Polymorphism in C++
// Compile with: g++ -std=c++17 polymorphismcls.cpp -o polymorphismcls
// Compile time and Run time polymorphism:
// Compile time polymorphism is achieved through function overloading and operator overloading.
// Run time polymorphism is achieved through inheritance and virtual functions.
// Analogy: Think of polymorphism like a Swiss Army knife. It can perform multiple functions (like a knife, scissors, screwdriver) depending on how you use it. Similarly, in programming, polymorphism allows methods to do different things based on the object that it is acting upon.
// Function Overloading Example (Compile Time Polymorphism)
// Function overloading allows multiple functions to have the same name but different parameters (different type or number of parameters).
// Operator Overloading Example (Compile Time Polymorphism)
// Operator overloading allows you to redefine the way operators work for user-defined types (classes).
// Virtual Functions Example (Run Time Polymorphism)
// A virtual function is a member function in the base class that you expect to override in derived
// classes. When you use a base class pointer to refer to a derived class object, the derived class's version of the function is called.
// Early Binding (Static Binding) vs Late Binding (Dynamic Binding): 
// Early binding (Compile Time Polymorphism) occurs when the function to be called is determined at compile time.
// Late binding (Run Time Polymorphism) occurs when the function to be called is determined.
// What is VTable?
// A VTable (Virtual Table) is a mechanism used in C++ to support dynamic (run-time) polymorphism. It is essentially a table of function pointers maintained per class that has virtual functions. Each entry in the VTable corresponds to a virtual function that can be called on objects of that class.
// analogy for virtual functions and VTable: Think of a VTable like a menu in a restaurant. Each dish on the menu corresponds to a virtual function in a class. When you order a dish (call a virtual function), the restaurant (program) looks up the menu (VTable) to see which specific dish (function implementation) to prepare based on your order (the actual object type at runtime).
// Function overriding Example (Run Time Polymorphism)

#include <iostream>
using namespace std;

class Account
{
public:
    virtual void displayAccountType() {
        cout << "This is a general account." << endl;
    }
    virtual void calculateInterest() {
        cout << "Calculating interest for general account." << endl;
    }
};

class SavingsAccount : public Account
{
public:
    void displayAccountType() override {
        cout << "This is a savings account." << endl;
    }
    void calculateInterest() override {
        cout << "Calculating interest for savings account." << endl;
    }
};
class CurrentAccount : public Account
{
public:
    void displayAccountType() override {
        cout << "This is a current account." << endl;
    }
    void calculateInterest() override {
        cout << "Calculating interest for current account." << endl;
    }
};


class Base {
public:
    void show() {
        cout << "Base class show function called!" << endl;
    }

    virtual void display() {
        cout << "Base class display function called!" << endl;
    }
};  

class Derived : public Base {
public:
    void show() {
        cout << "Derived class show function called!" << endl;
    }

    void display() override {
        cout << "Derived class display function called!" << endl;
    }
};  

int main() {


    Account* accountPtr[2];
    accountPtr[0] = new SavingsAccount();
    accountPtr[1] = new CurrentAccount();
    
    for(int i = 0; i < 2; i++) {
        accountPtr[i]->calculateInterest(); // Calls respective version
        // accountPtr[i]->calculateInterest();   // Calls respective version
    }

    delete accountPtr[0];
    delete accountPtr[1];

    cout << "End of Polymorphism Example-01!" << endl;

    // Account* accPtr = new SavingsAccount();
    // accPtr->displayAccountType(); // Calls SavingsAccount version

    // accPtr = new CurrentAccount();
    // accPtr->displayAccountType(); // Calls CurrentAccount version

    // delete accPtr; // Free allocated memory

    // Base b;
    // Derived d;

    // // Compile Time Polymorphism
    // b.show(); // Calls Base class show
    // d.show(); // Calls Derived class show

    // // Run Time Polymorphism
    // Base* basePtr;
    // basePtr = &b;
    // basePtr->display(); // Calls Base class display

    // basePtr = &d;
    // basePtr->display(); // Calls Derived class display due to virtual function

    return 0;
}