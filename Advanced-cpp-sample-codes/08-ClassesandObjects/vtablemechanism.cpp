// What is Vtable Mechanism in C++?
// Vtable (Virtual Table) is a mechanism used in C++ to support dynamic (runtime) polymorphism.
// It is essentially a table of function pointers maintained per class that has virtual functions.
#include <iostream>
using namespace std;

class Plugin {
public:
    virtual void execute() = 0; // Pure virtual function
    virtual ~Plugin() {}
};
class VideoPlugin : public Plugin {
public:
    void execute() override {
        cout << "Executing Video Plugin" << endl;
    }
};

class AudioPlugin : public Plugin {
public:
    void execute() override {
        cout << "Executing Audio Plugin" << endl;
    }
};

void runPlugin(Plugin* plugin) {
    plugin->execute(); // Calls the appropriate execute() based on the actual object type
}


class Base {
public:
    virtual void show() { // Virtual function
        cout << "Base class show function called." << endl;
    }
    void display() {
        cout << "Base class display function called." << endl;
    }
    virtual ~Base() {} // Virtual destructor
};
class Derived : public Base {
public:
    void show() override { // Overriding the virtual function
        cout << "Derived class show function called." << endl;
    }
    void display() {
        cout << "Derived class display function called." << endl;
    }
};
int main() {

    // Demonstrating Vtable Mechanism
    VideoPlugin vp;
    AudioPlugin ap;

    Plugin* p1 = &vp;
    Plugin* p2 = &ap;

    runPlugin(p1);
    runPlugin(p2);

    delete p1;
    delete p2;

    Base* bptr = new Derived(); // Base class pointer

    // Virtual function, binded at runtime (Runtime polymorphism)
    bptr->show(); // Calls Derived's show()
    bptr->display(); // Calls Base's display()
    // how we can see vtable mechanism in action
    cout << "Address of Base class pointer: " << bptr << endl;
    cout << "Address of Derived class object: " << static_cast<Derived*>(bptr) << endl;
    // Note: The actual vtable address is not directly accessible in standard C++.
    // how to call the base class function
    bptr->Base::show(); // Calls Base's show()
    // how to call the base class function from derived class object
    Derived* dptr = static_cast<Derived*>(bptr); // Downcasting to Derived class pointer. Detailed explanation: https://stackoverflow.com/questions/2954555/what-is-downcasting-in-c
    dptr->Base::show(); // Calls Base's show()

    return 0;
}

// what is difference between pointer variable and reference variable in C++?
// A pointer variable holds the memory address of another variable and can be reassigned to point to different variables.
// A reference variable is an alias for another variable and cannot be changed to refer to a different variable after its initialization.

// Base b vs Base* bptr
// Base b; // 'b' is an object of type Base, allocated on the stack
// Base* bptr = new Base(); // 'bptr' is a pointer to a Base object, allocated on the heap
// delete bptr; // Free the memory allocated for the Base object
// bptr = nullptr; // Avoid dangling pointer
// In summary, 'b' is a direct instance of Base, while 'bptr' is a pointer that can reference a Base object.

// Is this possible Base b = new Derived();?
// No, this is not possible. 'new Derived()' returns a pointer to a Derived object, while 'Base b' expects an object of type Base. You would need to use a pointer or reference to achieve polymorphism, like so: Base* bptr = new Derived();   
