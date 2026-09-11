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
    // --- Added explanation: typeid(i) / typeid(d) / typeid(p) ------------------
    // Generic syntax:  typeid( expression )
    //   expression - the only "operand": any expression whose type you want
    //                to inspect. Here it is used three times back-to-back
    //                with i (an int), d (a double), and p (a PlainPoint)
    //                substituted in turn. typeid returns a const
    //                std::type_info& describing expression's type; calling
    //                .name() on that reference yields a compiler-mangled
    //                string naming the type.
    //   Because none of int, double, or PlainPoint has any virtual function,
    //   the compiler can resolve all three of these typeid calls purely from
    //   the source code - no run-time lookup happens for any of them.
    cout << "typeid(i).name()  = " << typeid(i).name() << "\n";
    cout << "typeid(d).name()  = " << typeid(d).name() << "\n";
    cout << "typeid(p).name()  = " << typeid(p).name() << "\n";
    // Note: names are compiler-mangled (e.g. "i" for int on g++). Use them for
    // comparison, not for pretty-printing to end users.

    cout << "\n=== typeid on a polymorphic hierarchy (run-time answer) ===\n";
    // --- Added explanation: new Dog() / new Cat() / new Animal() ---------------
    // Generic syntax:  new ClassName( ctorArg1, ctorArg2, ... )
    //   ClassName - which class's constructor to run and whose object to
    //               allocate on the heap; here Dog, Cat, and Animal in turn.
    //   ctorArgs  - none of these three classes declares its own
    //               constructor, so each call uses the compiler-generated
    //               default constructor and takes no arguments.
    // Each `new` returns a pointer to the freshly constructed object, which
    // is stored in a variable whose STATIC type is Animal* (the base class)
    // even though the object underneath is actually a Dog, a Cat, or an
    // Animal respectively - this mismatch between "declared pointer type"
    // and "actual object type" is exactly what typeid below is used to
    // resolve.
    Animal* a1 = new Dog();
    Animal* a2 = new Cat();
    Animal* a3 = new Animal();

    // The STATIC type of a1/a2/a3 is "Animal*" in every case - the compiler
    // only knows "some kind of Animal" at this point in the source.
    // typeid(*pointer) asks the OBJECT, at run time, what it really is.
    // --- Added explanation: typeid(*a1) / typeid(*a2) / typeid(*a3) ------------
    // Generic syntax:  typeid( *pointerExpr )
    //   *pointerExpr - the operand is a DEREFERENCED pointer, so it refers
    //                  to the pointed-to Animal OBJECT itself, not the
    //                  pointer variable. Because Animal declares a virtual
    //                  destructor, Animal (and therefore Dog and Cat) are
    //                  polymorphic, so this typeid must look at the
    //                  object's run-time vtable to answer correctly - it
    //                  cannot be resolved at compile time the way
    //                  typeid(i) above could.
    //   In this call *a1 is really a Dog object, *a2 is really a Cat
    //   object, and *a3 is really an Animal object, so each
    //   typeid(...).name() reports the object's true most-derived type
    //   (Dog, Cat, Animal) rather than the pointer's declared type
    //   (Animal*).
    cout << "typeid(*a1).name() = " << typeid(*a1).name() << " (expect Dog)\n";
    cout << "typeid(*a2).name() = " << typeid(*a2).name() << " (expect Cat)\n";
    cout << "typeid(*a3).name() = " << typeid(*a3).name() << " (expect Animal)\n";

    cout << "\n=== Comparing typeid results ===\n";
    // --- Added explanation: typeid(*a1)==typeid(Dog) / typeid(*a1)!=typeid(Cat)
    // Generic syntax:  typeid( operandA ) == typeid( operandB )
    //   operandA - here *a1 (the object a1 points to, run-time type Dog).
    //   operandB - here Dog on the first line, Cat on the second: a TYPE
    //              NAME used directly as typeid's operand, which asks for
    //              the type_info of that named type rather than of a
    //              variable.
    //   operator== on the two std::type_info results is true only when
    //   both operands name the exact same type, so the first comparison is
    //   true (a1 really is a Dog) and, on the != line below, the
    //   comparison is also true (a1 is really not a Cat).
    if (typeid(*a1) == typeid(Dog))
        cout << "a1 really is a Dog\n";
    if (typeid(*a1) != typeid(Cat))
        cout << "a1 is definitely not a Cat\n";

    // Common trap: typeid(a1) (no dereference) always reports "Animal*" -
    // the pointer's own static type - never the pointee's dynamic type.
    // --- Added explanation: typeid(a1)  [no dereference] -----------------------
    // Generic syntax:  typeid( pointerExpr )
    //   pointerExpr - here a1 itself, WITHOUT the * dereference used
    //                 earlier. The operand is now the POINTER VARIABLE,
    //                 whose static (declared) type is always "Animal*" no
    //                 matter what object it points to - so this typeid
    //                 never needs to look at the pointed-to object at all,
    //                 and always reports "Animal*" even though a1 points
    //                 at a Dog.
    cout << "\nTrap: typeid(a1).name() [no *] = " << typeid(a1).name()
         << " -- always the pointer type, not Dog\n";

    delete a1;
    delete a2;
    delete a3;
}

// Compile:  g++ -std=c++17 1_typeid_basics.cpp -o typeid_basics
// Run:      typeid_basics.exe   (Windows)   or   ./typeid_basics   (Linux/macOS)

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() starts and prints the "non-polymorphic types" header.
// #2  i = 10 (int), d = 3.14 (double), p = PlainPoint{1, 2} are declared.
// #3  typeid(i), typeid(d), typeid(p) are each resolved entirely at compile
//     time (none of int/double/PlainPoint is polymorphic), printing their
//     compiler-mangled names (e.g. g++ typically prints "i" for int,
//     "d" for double, and something like "10PlainPoint" for the struct).
// #4  The "polymorphic hierarchy" header is printed.
// #5  a1 = new Dog(), a2 = new Cat(), a3 = new Animal() construct three heap
//     objects. Each is stored in an Animal* variable, so the STATIC type of
//     a1/a2/a3 is "Animal*" in every case, even though the real objects are
//     a Dog, a Cat, and an Animal respectively.
// #6  typeid(*a1), typeid(*a2), typeid(*a3) each dereference the pointer
//     first, so the operand is the OBJECT, not the pointer. Because Animal
//     has a virtual destructor (making the hierarchy polymorphic), each of
//     these performs a genuine run-time vtable lookup and correctly reports
//     Dog, Cat, and Animal - matching the "(expect ...)" comments.
// #7  typeid(*a1) == typeid(Dog) is true (a1's real object is a Dog), so
//     "a1 really is a Dog" is printed. typeid(*a1) != typeid(Cat) is also
//     true (a1 is definitely not a Cat), so that line is printed too.
// #8  typeid(a1) (no dereference this time) uses a1's STATIC type, which is
//     always "Animal*" regardless of what a1 points to - so it prints the
//     pointer type, NOT "Dog", demonstrating the common "forgot the *" trap.
// #9  delete a1; delete a2; delete a3; free the three heap objects. Because
//     Animal has a virtual destructor, deleting through an Animal* still
//     correctly invokes each object's actual (most-derived) destructor.
// #10 main() falls off the end without an explicit return statement; since
//     main's return type is int, this implicitly returns 0, ending the
//     program successfully.
// ============================================================================
