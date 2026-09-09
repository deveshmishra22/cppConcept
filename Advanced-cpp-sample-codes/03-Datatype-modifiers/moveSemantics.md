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

# Move Semantics: Rvalue References & Move Constructors

Move semantics allows an object to take ownership of another object's dynamically allocated resources (like heap memory or file handles) without creating duplicate copies.

---

## 1. Rvalue References (`T&&`)

C++11 introduced the double ampersand (`&&`) to create an **Rvalue Reference**. Unlike a standard reference (`T&`), an rvalue reference can **only** bind to temporary objects (rvalues).

```cpp
int x = 10;

int& lref = x;       // ✅ OK: lvalue reference binds to lvalue 'x'
// int&& rref1 = x;  // ❌ Compile Error: rvalue reference cannot bind to lvalue 'x'

int&& rref2 = 20;    // ✅ OK: rvalue reference binds to temporary rvalue '20'
```
### Why Rvalue References Matter
When a function parameter accepts T&&, it signals: "This object is temporary and about to be destroyed. You have full permission to modify it or steal its internal data."

## Function Overloading: Copy vs. Move
```c++
#include <iostream>
#include <string>

void process(const std::string& s) {
    std::cout << "Copying (lvalue): " << s << '\n';
}

void process(std::string&& s) {
    std::cout << "Moving (rvalue): " << s << '\n';
}

int main() {
    std::string name = "Alice";

    process(name);                 // Calls process(const std::string&): 'name' is an lvalue
    process("Temporary String");   // Calls process(std::string&&): string literal is converted to temporary rvalue
}
```
## 3. Writing a Move Constructor
A Copy Constructor allocates brand-new memory and copies every element. A Move Constructor simply steals the memory address and resets the source pointer to prevent double-deletion.
```c++
#include <iostream>
#include <utility>

class Buffer {
private:
    int* data;
    size_t size;

public:
    // Constructor
    Buffer(size_t s) : size(s), data(new int[s]) {}

    // Destructor
    ~Buffer() {
        delete[] data; // Safely deletes nullptr if moved from
    }

    // 1. COPY CONSTRUCTOR (Slow: Allocates new memory)
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 2. MOVE CONSTRUCTOR (Fast: Steals pointer in O(1) time)
    Buffer(Buffer&& other) noexcept 
        : data(other.data), size(other.size) { // Step 1: Copy pointer address
        
        other.data = nullptr;                 // Step 2: Clear old pointer (nullify)
        other.size = 0;
    }
};
```
Note: Always mark move constructors with noexcept. Standard library containers (like std::vector) will ignore move constructors and fall back to slow copying if they can throw exceptions.

## 4. The std::move Function
std::move does not move any data at runtime. It is purely a compile-time cast (static_cast<T&&>) that turns an lvalue into an rvalue.

Use std::move when you have a named variable (lvalue) that you no longer need, and you want to explicitly force C++ to treat it as a temporary object so it gets moved.
```c++
int main() {
    Buffer b1(1000000); // Heap buffer allocated
    
    // b1 is an lvalue, but std::move(b1) casts it to an rvalue reference (Buffer&&).
    // This forces the invocation of the Move Constructor instead of the Copy Constructor.
    Buffer b2 = std::move(b1); 

    // WARNING: 'b1' is now in a "moved-from" state. Its data pointer is nullptr!
}
```

# Key Takeaways
* Copying duplicates resources; Moving transfers ownership of resources.
* T&& binds strictly to temporaries (rvalues).
* Move constructors copy the raw pointer and set the source pointer to nullptr.
* std::move(x) casts x to an rvalue, giving the compiler permission to steal from x.

# The Rule of Five in Modern C++

In C++11 and beyond, managing dynamic resources directly requires understanding **The Rule of Five**. 

---

## 1. What is The Rule of Five?

If a class customizes or declares **any one** of the following five special member functions, it should explicitly declare or customize **all five**:

1. **Destructor** (`~Class()`)
2. **Copy Constructor** (`Class(const Class&)`)
3. **Copy Assignment Operator** (`Class& operator=(const Class&)`)
4. **Move Constructor** (`Class(Class&&)`)
5. **Move Assignment Operator** (`Class& operator=(Class&&)`)

> **Why?** If you write a custom destructor (e.g., to free heap memory), the compiler assumes your class manages a non-trivial resource. Consequently, the compiler automatically suppresses default move operations, falling back to slow copies or breaking resource ownership logic.

---

## 2. Complete Implementation Example

Here is a resource-managing class (`ArrayWrapper`) implementing all five special member functions:

```cpp
#include <iostream>
#include <utility>

class ArrayWrapper {
private:
    int* data;
    size_t size;

public:
    // 0. Regular Constructor
    explicit ArrayWrapper(size_t s) 
        : size(s), data(s > 0 ? new int[s]() : nullptr) {}

    // 1. Destructor
    ~ArrayWrapper() noexcept {
        delete[] data;
    }

    // 2. Copy Constructor (Deep Copy)
    ArrayWrapper(const ArrayWrapper& other) 
        : size(other.size), data(other.size > 0 ? new int[other.size] : nullptr) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 3. Copy Assignment Operator (Deep Copy with cleanup)
    ArrayWrapper& operator=(const ArrayWrapper& other) {
        if (this != &other) { // Guard against self-assignment (a = a)
            delete[] data;    // Free existing memory

            size = other.size;
            data = (size > 0) ? new int[size] : nullptr;
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // 4. Move Constructor (Steal Resources)
    ArrayWrapper(ArrayWrapper&& other) noexcept 
        : data(other.data), size(other.size) {
        
        other.data = nullptr; // Reset source object
        other.size = 0;
    }

    // 5. Move Assignment Operator (Steal Resources with cleanup)
    ArrayWrapper& operator=(ArrayWrapper&& other) noexcept {
        if (this != &other) { // Guard against self-assignment (a = std::move(a))
            delete[] data;    // Free existing memory

            data = other.data; // Steal resource
            size = other.size;

            other.data = nullptr; // Reset source object
            other.size = 0;
        }
        return *this;
    }
};
```
