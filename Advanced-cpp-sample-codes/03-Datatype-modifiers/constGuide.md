# C++ `const` Core Concepts: Functions & References

A structured reference guide for `const` correctness in C++.

---

## 1. `const` with Function Parameters

### Pass-by-Value (`const T`)
Prevents modification of the local parameter copy inside the function body.

```cpp
void printSquare(const int number) {
    // number *= number; // ❌ Compile Error: read-only parameter
    std::cout << number * number << '\n';
}
```

### Pass-by-Const-Reference (const T&)
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

