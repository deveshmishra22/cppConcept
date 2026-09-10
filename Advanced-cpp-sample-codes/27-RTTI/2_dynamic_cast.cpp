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
    Shape* shapes[] = { new Circle(4.0), new Rectangle(3.0, 5.0) };

    for (Shape* s : shapes) {
        cout << "area = " << s->area() << " -> ";

        // Try to treat s as a Circle. If it really is one, we get a usable
        // pointer. If not, dynamic_cast returns nullptr instead of crashing.
        if (Circle* c = dynamic_cast<Circle*>(s)) {
            c->spin();
        } else if (Rectangle* r = dynamic_cast<Rectangle*>(s)) {
            r->stretch();
        } else {
            cout << "Unknown shape kind\n";
        }
    }

    cout << "\n=== dynamic_cast on references: fails loudly (throws std::bad_cast) ===\n";
    Circle onlyCircle(2.0);
    Shape& shapeRef = onlyCircle;
    try {
        // shapeRef really refers to a Circle, so this succeeds.
        Circle& c = dynamic_cast<Circle&>(shapeRef);
        cout << "Reference cast succeeded, radius = " << c.radius << "\n";

        // shapeRef is NOT a Rectangle - a reference cast cannot return
        // "no reference", so the language throws instead.
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
    Circle* wrong = dynamic_cast<Circle*>(s);
    cout << "dynamic_cast<Circle*> on a real Rectangle = "
         << (wrong == nullptr ? "nullptr (safe)" : "non-null (unexpected)") << "\n";

    for (Shape* s2 : shapes) delete s2;
}

// Compile:  g++ -std=c++17 2_dynamic_cast.cpp -o dynamic_cast_demo
// Run:      dynamic_cast_demo.exe   (Windows)   or   ./dynamic_cast_demo   (Linux/macOS)
