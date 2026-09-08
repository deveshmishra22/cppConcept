A summary of C++ method resolution, virtual dispatch, signature matching, and compile-time safety rules:

**C++ Function Overriding and Hiding Matrix**

| Base Class Function | Derived Class Declaration | Binding Type | Runtime & Compile Behavior |
| --- | --- | --- | --- |
| `virtual` | Exact signature + `override` | **Dynamic** (Runtime) | Correct override with full compile-time safety checks. |
| `virtual` | Exact signature without `override` | **Dynamic** (Runtime) | Correct override, but lacks protection against future signature changes. |
| `virtual` | Mismatched signature + `override` | **N/A** | **Compiler Error:** Fails build due to signature mismatch. |
| `virtual` | Mismatched signature without `override` | **Static** (Compile-time) | **Silent Bug:** Creates a brand-new function and hides the base version. |
| Non-`virtual` | Any signature + `override` | **N/A** | **Compiler Error:** `override` cannot be applied to non-virtual functions. |
| Non-`virtual` | Same signature without `override` | **Static** (Compile-time) | **Method Hiding / Redefinition:** Object/pointer type dictates which function runs. |

---

**Core Concepts Breakdown**

**Virtual Functions & Dynamic Binding**

* Declaring a method `virtual` enables runtime polymorphism using a hidden virtual table (`vtable`) and virtual pointer (`vptr`).
* Function calls are resolved dynamically based on the actual object created in memory, regardless of whether it is accessed through a base pointer or reference.

**The `override` Keyword**

* Serves exclusively as a compile-time safety check and documentation tool.
* Forces the compiler to verify that an exact signature match (including parameter types, return types, and `const` modifiers) exists in a base class virtual function.
* Does not alter memory layout, execution timing, or the number of function definitions created in binary memory.
* Strictly forbidden on non-virtual functions.

**Method Hiding (Redefinition)**

* Occurs when a derived class defines a method with the same name as a base method without dynamic dispatch (either because the base function is non-virtual or the signatures do not match).
* Uses static binding: calling the method via a base pointer executes the base version, while calling it via a derived object executes the derived version.
* Shadowing a method name in a derived class automatically hides all base class overloads of that name, unless explicitly brought into scope using the `using Base::method_name` directive.


**Core Takeaway on Non-Virtual Redefinition**

* Static Binding Rule: When a base function is non-virtual, virtual tables do not exist. The compiler determines which function to invoke at compile time based purely on the variable's declared type, not the object's actual underlying type in memory.

* Pointer Execution: Calling ptr->show() on a Parent* pointer will always execute Parent::show(), even if ptr points to a Child instance.

* Direct Object Execution: Calling c.show() directly on a Child instance will execute Child::show() because the child class scope shadows (hides) the parent's function name.
