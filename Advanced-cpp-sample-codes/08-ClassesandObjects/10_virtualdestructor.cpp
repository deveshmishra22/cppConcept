#include<iostream>
using namespace std;

class Base {
public:
    Base() {
        cout << "Base class constructor called." << endl;
    }
    ~Base() {
        cout << "Base class destructor called." << endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        cout << "Derived class constructor called." << endl;
    }
    ~Derived() {
        cout << "Derived class destructor called." << endl;
    }
};

class CorrectBase {
public:
    CorrectBase() {
        cout << "CorrectBase class constructor called." << endl;
    }
    virtual ~CorrectBase() {
        cout << "CorrectBase class destructor called." << endl;
    }
};
class CorrectDerived : public CorrectBase {
public:
    CorrectDerived() {
        cout << "CorrectDerived class constructor called." << endl;
    }
    ~CorrectDerived() {
        cout << "CorrectDerived class destructor called." << endl;
    }
};

class A
{
    public:
    A() { cout << "Constructor A" << endl; }
};
class B : public A
{
    public:
    B() { cout << "Constructor B" << endl; }
};

int main() {
    // cout << "Creating Derived class object using Base class pointer." << endl;
    // Base* obj = new Derived();
    // cout << "Deleting Base class pointer pointing to Derived class object." << endl;
    // delete obj; // Undefined behavior: Derived destructor won't be called

    cout << "\nCreating CorrectDerived class object using CorrectBase class pointer." << endl;
    CorrectBase* correctObj = new CorrectDerived();
    cout << "Deleting CorrectBase class pointer pointing to CorrectDerived class object." << endl;
    delete correctObj; // Correct behavior: CorrectDerived destructor will be called
    
    // Demonstrating constructor calls in inheritance
    // cout << "\nCreating object of class B which inherits from A." << endl;
    // A* newObjwithoutDestructor = new B();
    // delete newObjwithoutDestructor;

    // Note: In this case, since there are no destructors defined in classes A and B,
    // the default destructors will be called, and there won't be any output related to destruct
    // ion. However, if there were resources to free in class B, not having a virtual destructor
    // in class A could lead to resource leaks when deleting through a base class pointer.
    // Always ensure to define virtual destructors in base classes when dealing with inheritance.
    // to avoid such issues.
    return 0;
}   

// Output Explanation:
// In the first case, when we delete the Base class pointer pointing to a Derived class object
// without a virtual destructor, only the Base class destructor is called, leading to potential
// resource leaks if the Derived class allocated resources in its constructor.
// In the second case, with a virtual destructor in the CorrectBase class, both the CorrectDerived
// and CorrectBase destructors are called in the correct order, ensuring proper resource cleanup.

// why virtual destructor is important in C++?
// When a base class has a virtual destructor, it ensures that the destructor of the derived class
// is called when an object of the derived class is deleted through a base class pointer.
// This is crucial for proper resource management and avoiding memory leaks in polymorphic scenarios.
// If the base class destructor is not virtual, deleting a derived class object through a base class pointer
// results in undefined behavior, as the derived class destructor will not be invoked.

// what is memory leak?
// A memory leak occurs when a program allocates memory on the heap but fails to release it
// when it is no longer needed. This can lead to increased memory usage over time,
// potentially exhausting available memory and causing the program to crash or behave unexpectedly.
