#include<iostream>  
using namespace std;

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
 
// Definition (Initialization) of static member outside the class
int Employee::count = 0;

// class Demo{  
// public: 
//     // Static member variable
//     static int staticVar;

//     // Static member function
//     static void func1() {
//         cout << "Static member function called!" << endl;
//     }

//     Demo(){  
//         cout<<"Constructor called!"<<endl;  
//         staticVar++; // Increment static variable
//     }
// };

int main()
{  

    // Calling static member function without creating an object
    Employee::showCount();
    Employee e1("Ravi");
    Employee e2("Meena");
    Employee e3("Karan");
 
    e1.display();
    e2.display();
    e3.display();
 
    cout << "Total Employees: " << Employee::count << endl;
    cout << "Total Employees: " << e2.count << endl;

    return 0;
    // static int x = 10; // static memory allocation
    // cout << "Static variable x: " << x << endl;
    
    // // Dynamic memory allocation
    // int* p = new int; // allocating memory on heap
    // *p = 20;
    // cout << "Dynamically allocated variable *p: " << *p << endl;

    // delete p; // freeing dynamically allocated memory

    // Demo d1;
    // // Accessing static member variable
    // cout << "Static member variable Demo::staticVar: " << d1.staticVar << endl;
    // // Calling static member function
    // d1.func1();

    // return 0;  
}

// Email: raushan.ranjan@koenig-solutions.com
//Assessment
// Extend the shape report
// Add a Triangle class and report the total area of all three shapes.
