// What is dynamic_cast?
// dynamic_cast is the safe way to convert a base-class pointer or reference
// back down to a derived-class pointer or reference, WHILE checking - at run
// time - that the object really is that derived type.
// It only works on polymorphic types (the hierarchy must have a virtual
// function, usually the destructor), because it relies on the same RTTI data
// that typeid uses to identify the object's real type.

#include <iostream>
#include <typeinfo>
using namespace std;

struct Shape {
    virtual ~Shape() {}
    virtual double area() const = 0;
};
struct Circle : Shape {
    double radius;
    Circle(double r) : radius(r) {}
    double area() const override { return 3.14159 * radius * radius; }
    void spin() const { cout << "Circle spins in place\n"; }   // Circle-only behaviour
};
struct Rectangle : Shape {
    double w, h;
    Rectangle(double w_, double h_) : w(w_), h(h_) {}
    double area() const override { return w * h; }
    void stretch() const { cout << "Rectangle stretches its width\n"; } // Rectangle-only
};

int main() {
    cout << "=== dynamic_cast on pointers: fails softly (returns nullptr) ===\n";
    // --- Added explanation: new Circle(4.0) / new Rectangle(3.0, 5.0) ---------
    // Generic syntax:  new ClassName( ctorArg1, ctorArg2, ... )
    //   ClassName - Circle or Rectangle here; each derives from Shape and
    //               overrides area().
    //   ctorArg1  - Circle's constructor takes one arg, radius (4.0 here);
    //               Rectangle's constructor takes two, w_ and h_ (3.0 and
    //               5.0 here, forwarded into members w and h).
    // Both calls return a pointer whose STATIC type is narrowed to Shape*
    // by this array's element type (Shape*[]), even though the actual
    // objects are a Circle and a Rectangle - exactly the situation
    // dynamic_cast below is used to see through safely.
    Shape* shapes[] = { new Circle(4.0), new Rectangle(3.0, 5.0) };

    for (Shape* s : shapes) {
        cout << "area = " << s->area() << " -> ";

        // Try to treat s as a Circle. If it really is one, we get a usable
        // pointer. If not, dynamic_cast returns nullptr instead of crashing.
        // --- Added explanation: dynamic_cast<Circle*>(s) / dynamic_cast<Rectangle*>(s)
        // Generic syntax:  dynamic_cast<TargetType*>( sourcePointer )
        //   TargetType    - template parameter: the derived (or otherwise
        //                   related) type you want to end up with a
        //                   pointer to; Circle in the first attempt below,
        //                   Rectangle in the second. Written inside <...>
        //                   because it must be known at compile time, even
        //                   though the check it triggers happens at run
        //                   time.
        //   sourcePointer - the operand in parentheses: any pointer to a
        //                   polymorphic base (or related) type; here s in
        //                   both attempts, whose static type is Shape* but
        //                   whose actual pointee, on this loop iteration,
        //                   might be a Circle OR a Rectangle.
        // At run time each cast looks at s's object's real type: if it
        // really is a Circle (or derives from Circle), dynamic_cast<Circle*>
        // returns a valid Circle* pointing at the same object; if s
        // actually points at a Rectangle instead, it returns nullptr -
        // which is exactly what the if-condition below tests via the
        // implicit bool conversion of c, before falling through to try
        // Rectangle next.
        if (Circle* c = dynamic_cast<Circle*>(s)) {
            c->spin();
        } else if (Rectangle* r = dynamic_cast<Rectangle*>(s)) {
            r->stretch();
        } else {
            cout << "Unknown shape kind\n";
        }
    }

    cout << "\n=== dynamic_cast on references: fails loudly (throws std::bad_cast) ===\n";
    // --- Added explanation: Circle onlyCircle(2.0) ----------------------------
    // Generic syntax:  ClassName variableName( ctorArg1 )
    //   ctorArg1 (2.0) - Circle's constructor parameter `r`, stored into
    //                    the new object's radius member.
    Circle onlyCircle(2.0);
    Shape& shapeRef = onlyCircle;
    try {
        // shapeRef really refers to a Circle, so this succeeds.
        // --- Added explanation: dynamic_cast<Circle&>(shapeRef) ---------------
        // Generic syntax:  dynamic_cast<TargetType&>( sourceReference )
        //   TargetType      - template parameter: the reference type
        //                     wanted; Circle& here.
        //   sourceReference - the operand: a reference to a polymorphic
        //                     base type; here shapeRef, a Shape& that
        //                     actually refers to onlyCircle (a real Circle
        //                     object).
        // Unlike the pointer form, a reference can never be "null", so
        // this either returns a valid Circle& bound to the same object (as
        // it does here, since shapeRef really refers to a Circle) or the
        // program never reaches the next line at all (see the Rectangle&
        // attempt just below, which throws instead).
        Circle& c = dynamic_cast<Circle&>(shapeRef);
        cout << "Reference cast succeeded, radius = " << c.radius << "\n";

        // shapeRef is NOT a Rectangle - a reference cast cannot return
        // "no reference", so the language throws instead.
        // --- Added explanation: dynamic_cast<Rectangle&>(shapeRef) ------------
        // Generic syntax:  dynamic_cast<TargetType&>( sourceReference )
        //   TargetType      - Rectangle& here.
        //   sourceReference - shapeRef again; but shapeRef's real object
        //                     (onlyCircle) is NOT a Rectangle.
        // Because a reference result cannot be nullptr to signal failure,
        // an impossible reference dynamic_cast instead throws
        // std::bad_cast - so this line throws immediately and the cout on
        // the next line never executes; control jumps straight to the
        // catch block below.
        Rectangle& r = dynamic_cast<Rectangle&>(shapeRef);
        cout << "This line never runs: " << r.w << "\n";
    } catch (const bad_cast& e) {
        cout << "Caught std::bad_cast: " << e.what() << "\n";
    }

    cout << "\n=== Why not just use a C-style cast? ===\n";
    Shape* s = shapes[1]; // this is really a Rectangle
    // (Circle*)s would compile and silently produce a garbage Circle* that
    // points at Rectangle's memory layout - reading c->radius would read
    // whatever bytes happen to sit where radius would be. dynamic_cast makes
    // that impossible: it checks the real type before handing back a pointer.
    // --- Added explanation: dynamic_cast<Circle*>(s) ---------------------------
    // Generic syntax:  dynamic_cast<TargetType*>( sourcePointer )
    //   TargetType    - Circle* here, same as the earlier loop's attempt.
    //   sourcePointer - s, which now points at shapes[1] - the Rectangle
    //                   created earlier via new Rectangle(3.0, 5.0).
    // Since s's real object is a Rectangle, not a Circle, this returns
    // nullptr - proving that dynamic_cast checks the ACTUAL object, not
    // the Shape* type visible at compile time.
    Circle* wrong = dynamic_cast<Circle*>(s);
    cout << "dynamic_cast<Circle*> on a real Rectangle = "
         << (wrong == nullptr ? "nullptr (safe)" : "non-null (unexpected)") << "\n";

    for (Shape* s2 : shapes) delete s2;
}

// Compile:  g++ -std=c++17 2_dynamic_cast.cpp -o dynamic_cast_demo
// Run:      dynamic_cast_demo.exe   (Windows)   or   ./dynamic_cast_demo   (Linux/macOS)

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() prints the "dynamic_cast on pointers" header. shapes[] is
//     constructed: shapes[0] = new Circle(4.0) (radius 4.0), shapes[1] =
//     new Rectangle(3.0, 5.0) (w=3.0, h=5.0); both are stored as Shape*.
// #2  Loop iteration 1: s = shapes[0] (real object is a Circle). s->area()
//     computes 3.14159 * 4.0 * 4.0 = 50.26544 and is printed.
//     dynamic_cast<Circle*>(s) succeeds (s's real type IS Circle), so c is
//     non-null and c->spin() runs, printing "Circle spins in place". The
//     else-if branch is skipped entirely.
// #3  Loop iteration 2: s = shapes[1] (real object is a Rectangle). area()
//     computes 3.0 * 5.0 = 15 and is printed. dynamic_cast<Circle*>(s)
//     fails (returns nullptr, since s is really a Rectangle), so the first
//     branch is skipped; dynamic_cast<Rectangle*>(s) then succeeds, so r is
//     non-null and r->stretch() runs, printing "Rectangle stretches its
//     width".
// #4  The "dynamic_cast on references" header is printed. onlyCircle is
//     constructed with radius 2.0; shapeRef (a Shape&) is bound to it.
// #5  dynamic_cast<Circle&>(shapeRef) succeeds (shapeRef's real object IS a
//     Circle), so c is bound and "Reference cast succeeded, radius = 2" is
//     printed.
// #6  dynamic_cast<Rectangle&>(shapeRef) is attempted next: shapeRef's real
//     object (onlyCircle) is not a Rectangle, and a reference cast cannot
//     return a null sentinel, so this line THROWS std::bad_cast
//     immediately. r is never initialized, and "This line never runs" is
//     indeed never printed.
// #7  The catch(const bad_cast& e) block catches that exception and prints
//     "Caught std::bad_cast: " followed by e.what()'s message.
// #8  The "Why not just use a C-style cast?" header is printed. s is
//     reassigned to shapes[1] (the Rectangle).
// #9  dynamic_cast<Circle*>(s) is tried again on this Rectangle: it returns
//     nullptr (safe failure), so "dynamic_cast<Circle*> on a real
//     Rectangle = nullptr (safe)" is printed - demonstrating that, unlike a
//     C-style cast, dynamic_cast never hands back a pointer that
//     misinterprets a Rectangle's memory as a Circle's.
// #10 The final loop deletes shapes[0] (Circle) and shapes[1] (Rectangle);
//     because Shape has a virtual destructor, deleting through a Shape*
//     still runs each object's own (trivial, here) destructor correctly.
// #11 main() falls off the end without an explicit return statement; since
//     main's return type is int, this implicitly returns 0.
// ============================================================================
