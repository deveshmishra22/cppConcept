# C++ `const` Core Concepts: Functions & References

A structured reference guide for `const` correctness in C++.

---

## 1. `const` with Function Parameters

### 1.1 Pass-by-Value (`const T`)
Prevents modification of the local parameter copy inside the function body.

```cpp
void printSquare(const int number) {
    // number *= number; // ❌ Compile Error: read-only parameter
    std::cout << number * number << '\n';
}
```

### 1.2 Pass-by-Const-Reference (const T&)
Avoids object copying while guaranteeing read-only access. Accepts both lvalues (variables) and temporary rvalues.

```cpp
#include <iostream>
#include <string>

void printMessage(const std::string& msg) {
    // msg += "!"; // ❌ Compile Error: read-only reference
    std::cout << msg << '\n';
}

int main() {
    std::string text = "Hello World";
    printMessage(text);          // Binds to lvalue
    printMessage("Direct Text"); // Binds to temporary rvalue
}
```
### 1.3 Return Values
Returning a const T& allows callers to inspect internal data (like class members) without copying it, while preventing them from modifying it.
```cpp
class Account {
    std::string accountNumber = "ACC-12345";
public:
    // Returns read-only reference to private member
    const std::string& getAccountNumber() const {
        return accountNumber;
    }
};
```
Note: Avoid returning values as const T (by value) in modern C++, as it disables C++11 move semantics.

### 1.4 Const Member Functions
Placing const after a class method's parameter list guarantees that the method will not modify any member variables of the calling object (unless marked mutable).
```cpp
class Circle {
    double radius;
public:
    Circle(double r) : radius(r) {}

    // Const member function: Promises not to modify 'radius'
    double getArea() const {
        // radius = 5.0; // ❌ Compile Error: cannot modify member in const method
        return 3.14159 * radius * radius;
    }
};
```

## 2. Accessing Member Functions via `const` References

When an object is accessed through a `const` reference (`const T&`), C++ enforces a strict rule:
**you can ONLY call member functions that are explicitly marked as `const`.**

---

### The Core Rule

If an object is accessed via a `const` reference, calling a non-`const` member function causes a **compile-time error** because non-`const` functions reserve the right to mutate the object.

```cpp
#include <iostream>

class Counter {
private:
    int count = 0;

public:
    // 1. Non-const member function (modifies state)
    void increment() {
        count++;
    }

    // 2. Const member function (read-only guarantee)
    int getValue() const {
        return count;
    }
};

void inspectCounter(const Counter& c) {
    // ✅ ALLOWED: getValue() is a const member function
    std::cout << "Count: " << c.getValue() << '\n';

    // ❌ COMPILE ERROR: increment() is non-const!
    // c.increment(); 
}

int main() {
    Counter myCounter;
    myCounter.increment();  // ✅ Allowed on non-const instance
    inspectCounter(myCounter);
    return 0;
}
```
