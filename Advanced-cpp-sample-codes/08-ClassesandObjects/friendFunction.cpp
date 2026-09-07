// Friend Function in C++
#include <iostream>
using namespace std;

class Box {
private:
    double width;
public:
    Box(double w) : width(w) {}

    // Function declaration
    void printW(Box b)
    {
        cout << "Width of box: " << b.width << endl;
    }

    // Declare friend function
    friend void printWidth(Box box)
    {
        // Access private member
        cout << "Width of box: " << box.width << endl;
        // Purpose of friend function is to access private members of the class.
        // It is not a member of the class but has access to its private and protected members.
        // It is useful when two or more classes need to work closely together and share data.
        // Analogy: Think of a friend function like a trusted friend who is allowed to enter your house (class) and see things that others cannot (private members).
    }

};

int main() {
    Box box(10.0);
    printWidth(box); // Call friend function
    box.printW(box); // Call member function
    return 0;
}