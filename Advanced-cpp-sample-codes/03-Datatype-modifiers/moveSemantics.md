# Move Semantics 
Before C++11, passing or returning large objects forced the compiler to duplicate heap memory even when the original object was a temporary about to 
be destroyed. Move semantics eliminates this redundant overhead by allowing a new object to "steal" pointers directly from an expiring source object.
## The Core Intuition: Copying vs. Moving
* Copying: You buy a brand new physical binder, print copies of all 500 pages from a colleague's binder, and put them in yours. 
(Expensive, slow heap allocations).
* Moving: Your colleague hands you their entire binder directly. They now hold an empty hand (nullptr), you hold all 500 pages,
and zero pages were printed.

# Lvalues vs. Rvalues in C++

Understanding value categories is the first step toward mastering move semantics. C++ divides expressions into **Lvalues** (persistent objects with memory addresses) and **Rvalues** (temporary values that expire at the end of the statement).

---

## Comparison Matrix

| Concept | Definition | Example | Can take memory address? |
| :--- | :--- | :--- | :---: |
| **Lvalue** | Persistent object with a named location in memory. | `int x = 10;` | ✅ Yes (`&x`) |
| **Lvalue** | Persistent object with a named location in memory. | `std::string s = "text";` | ✅ Yes (`&s`) |
| **Rvalue** | Temporary value/literal expiring at the end of the line. | `42` | ❌ No (`&42` is invalid) |
| **Rvalue** | Temporary value/literal expiring at the end of the line. | `x + 5` | ❌ No (`&(x + 5)` is invalid) |
| **Rvalue** | Temporary value/literal expiring at the end of the line. | `std::string("temp")` | ❌ No (`&std::string("temp")` is invalid) |

---

## Code Demonstration

```cpp
#include <iostream>
#include <string>

int getValue() {
    return 100; // Return value is a temporary (rvalue)
}

int main() {
    // ------------------------------------------------------------------
    // LVALUES: Named objects, exist across multiple lines of code
    // ------------------------------------------------------------------
    int x = 10;               // 'x' is an lvalue
    std::string s = "text";   // 's' is an lvalue
    
    int* ptr = &x;            // ✅ Valid: Taking address of lvalue

    // ------------------------------------------------------------------
    // RVALUES: Anonymous temporaries, destroyed at the semicolon
    // ------------------------------------------------------------------
    // int* p1 = &42;         // ❌ Compile Error: Cannot take address of literal
    // int* p2 = &(x + 5);    // ❌ Compile Error: Cannot take address of expression
    // int* p3 = &getValue(); // ❌ Compile Error: Cannot take address of temporary

    return 0;
}
```
