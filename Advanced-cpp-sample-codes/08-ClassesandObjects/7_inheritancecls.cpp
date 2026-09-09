#include<iostream>  
using namespace std;


// Single Level Inheritance Example:
//Accessing Parent class members in Child class. 
// Inheritance is a mechanism where a new class (derived class or child class) inherits properties and behaviors (data members and member functions) from an existing class (base class or parent class).
// It promotes code reusability and establishes a hierarchical relationship between classes.
class Parent
{
public:
    Parent() {
        cout << "Parent Constructor called!" << endl;
    }
    void showParentMessage() {
        cout << "Hello from Parent class!" << endl;
    }
};

class Child : public Parent // Inheritance
{
public:
    Child() {
        cout << "Child Constructor is called!"<< endl;
    }
    void display() {
        cout << "Hello from Child class!" << endl;
    }
    void ShowChildMessage()
    {
        cout << "Child class accessing Parent class method:" << endl;
    }
};

// Multiple Inheritance Example:
class Teacher
{
public:
    void Teach() {
        cout << "Hello from Teacher class!" << endl;
    }
};

class Researcher
{
public:
    void Research() {
        cout << "Hello from Researcher class!" << endl;
    }
};

class Professor: public Teacher, public Researcher
{
public:
    void Guide() {
        cout << "Hello from Professor class!" << endl;
    }
};

//Multi-Level Inheritance Example:
class GrandParent
{
    public:
    GrandParent() {
        cout << "GrandParent Constructor called!" << endl;
    };   
    void showGrandParentMessage() {
        cout << "Hello from GrandParent class!" << endl;
    };
};
class Parent1 : public GrandParent
{
    public:
    Parent1() {
        cout << "Parent1 Constructor called!" << endl;
    }
    void showParent1Message() {
        cout << "Hello from Parent1 class!" << endl;
    };
};
class Child1 : public Parent1
{
    public:
    Child1() {
        cout << "Child1 Constructor called!" << endl;
    }
    void showChild1Message() {
        cout << "Hello from Child1 class!" << endl;
    };
};

class Employee {
public:
    int id;
    string name;
    static int count; // Declaration of static data member
 
    Employee(string n) {
        name = n;
        id = ++count; // Increment count each time an object is created
    }
 
    void display() {
        cout << "Employee ID: " << id << ", Name: " << name << endl;
    }

    static void showCount() {
        cout << "Total Employees: " << count << endl;
    }
};
class Manager : public Employee {
public:
    Manager(string n) : Employee(n) {
        // Manager-specific initialization can go here
    }
};

class SeniorManager : public Manager {
public:
    SeniorManager(string n) : Manager(n) {
        // SeniorManager-specific initialization can go here
    }
};

int main()
{
    Child c;
    // c.display();
    // c.ShowChildMessage();
    // c.showParentMessage(); // Accessing Parent class method

    // //Example of Multiple Inheritance
    // Professor p;
    // p.Teach();      // From Teacher class
    // p.Research();   // From Researcher class
    // p.Guide();      // From Professor class

    // //Example of Multi-Level Inheritance
    // Child1 c1;
    // c1.showChild1Message();       // From Child1 class
    // c1.showParent1Message();      // From Parent1 class
    // c1.showGrandParentMessage();  // From GrandParent class

    // //Example of Multi-Level Inheritance with Employee classes
    // Employee e1("Pratibha");
    // Manager m1("Raushan");
    // SeniorManager sm("Vatan");
    // sm.display();
    return 0;
}