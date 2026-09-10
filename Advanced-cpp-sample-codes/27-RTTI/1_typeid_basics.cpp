// What is RTTI?
// RTTI (Run-Time Type Information) is the mechanism that lets a C++ program
// discover an object's actual type while the program is running, instead of
// only knowing the type the compiler assumed at compile time.
// typeid is one of the two RTTI tools (dynamic_cast is the other, see 2_dynamic_cast.cpp).

// typeid on a NON-polymorphic type (no virtual functions) is resolved entirely
// at compile time, from the static (declared) type of the expression.
// typeid on a POLYMORPHIC type (at least one virtual function in the hierarchy)
// is resolved at run time, from the object's actual, most-derived type.

#include <iostream>
#include <typeinfo>   // required for typeid
using namespace std;

// A plain struct - NOT polymorphic (no virtual functions anywhere in it)
struct PlainPoint {
    int x, y;
};

// A polymorphic base - has at least one virtual function
struct Animal {
    virtual ~Animal() {}   // the virtual destructor is what makes this polymorphic
    virtual void speak() { cout << "Animal makes a sound\n"; }
};
struct Dog : Animal {
    void speak() override { cout << "Dog barks\n"; }
};
struct Cat : Animal {
    void speak() override { cout << "Cat meows\n"; }
};

int main() {
    cout << "=== typeid on non-polymorphic types (compile-time answer) ===\n";
    int i = 10;
    double d = 3.14;
    PlainPoint p{1, 2};
    cout << "typeid(i).name()  = " << typeid(i).name() << "\n";
    cout << "typeid(d).name()  = " << typeid(d).name() << "\n";
    cout << "typeid(p).name()  = " << typeid(p).name() << "\n";
    // Note: names are compiler-mangled (e.g. "i" for int on g++). Use them for
    // comparison, not for pretty-printing to end users.

    cout << "\n=== typeid on a polymorphic hierarchy (run-time answer) ===\n";
    Animal* a1 = new Dog();
    Animal* a2 = new Cat();
    Animal* a3 = new Animal();

    // The STATIC type of a1/a2/a3 is "Animal*" in every case - the compiler
    // only knows "some kind of Animal" at this point in the source.
    // typeid(*pointer) asks the OBJECT, at run time, what it really is.
    cout << "typeid(*a1).name() = " << typeid(*a1).name() << " (expect Dog)\n";
    cout << "typeid(*a2).name() = " << typeid(*a2).name() << " (expect Cat)\n";
    cout << "typeid(*a3).name() = " << typeid(*a3).name() << " (expect Animal)\n";

    cout << "\n=== Comparing typeid results ===\n";
    if (typeid(*a1) == typeid(Dog))
        cout << "a1 really is a Dog\n";
    if (typeid(*a1) != typeid(Cat))
        cout << "a1 is definitely not a Cat\n";

    // Common trap: typeid(a1) (no dereference) always reports "Animal*" -
    // the pointer's own static type - never the pointee's dynamic type.
    cout << "\nTrap: typeid(a1).name() [no *] = " << typeid(a1).name()
         << " -- always the pointer type, not Dog\n";

    delete a1;
    delete a2;
    delete a3;
}

// Compile:  g++ -std=c++17 1_typeid_basics.cpp -o typeid_basics
// Run:      typeid_basics.exe   (Windows)   or   ./typeid_basics   (Linux/macOS)
