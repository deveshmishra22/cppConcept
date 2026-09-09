#include <iostream>

using namespace std;
 
class Student {

private:
    string name;
    int rollNo;
 
public: 
    // Constructor
    Student() {
        this->name = "Test User";
        this->rollNo = 0;
        cout << "Constructor called!" << endl;
    }

    // Constructor overloading with parameters
    Student(string n, int r) {
        this->name = n;
        this->rollNo = r;
        cout << "Constructor called!" << endl;
    }

    // Copy Constructor (optional)
    /* 
    Copy constructor is used to create a new object as a copy of an existing object. It is called when an object is passed by value, returned from a function, or explicitly copied.
    It is defined with a single parameter that is a reference to an object of the same class.
    In real-world scenarios, if you don't define a copy constructor, the compiler provides a default one that performs a shallow copy. However, if your class manages resources like dynamic memory, file handles, etc., you should define your own copy constructor to ensure a deep copy and proper resource management.
    Analogy: Think of a copy constructor like making a photocopy of a document. The photocopy is a separate entity that contains the same information as the original document, but changes made to the photocopy do not affect the original document. 
    */
    Student(const Student &s) {
        this->name = s.name;
        this->rollNo = s.rollNo;
        cout << "Copy Constructor called!" << endl;
    }

    void display() {
        cout << "Name: " << name << ", Roll: " << rollNo << endl;
    }

};
 
int main() {

    Student s1;   // Constructor is called automatically
    Student s2("Raushan", 11); // Parameterized constructor is called automatically
    s1.display();
    s2.display();

    Student s3 = s2; // Copy constructor is called automatically
    s3.display();
    return 0;

}
 