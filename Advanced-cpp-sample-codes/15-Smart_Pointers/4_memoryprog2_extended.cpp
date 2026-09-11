#include <iostream>
#include <new> // placement new
#include <cstring>
#include <memory> // for smart pointers
 
struct S {
    int x;
    S(int v) : x(v) { std::cout << "S constructed\n"; }
    ~S() { std::cout << "S destructed\n"; }
};


struct Node
{
    int v;
    Node(int val) : v(val) { std::cout << "Node constructed\n"; }
    ~Node() { std::cout << "Node destructed\n"; }
};

// int x=0 vs thread_local int x=0
// thread_local storage duration means that each thread has its own instance of the variable.   
// and it is initialized when the thread is created and destroyed when the thread exits.
// In contrast, a regular global variable has a single instance shared across all threads in the program.
// Example:
// thread_local int threadVar = 0; // each thread has its own copy
// int globalVar = 0; // shared across all threads
// In a multi-threaded program, modifying threadVar in one thread does not affect its value in another thread,
// whereas modifying globalVar affects all threads.
 
int main() {

    // unique pointer example
    {
        std::unique_ptr<Node> ptr1 = std::make_unique<Node>(10);
        std::cout << "Node value: " << ptr1->v << '\n';
    } // ptr1 goes out of scope here, and Node is automatically deleted

    // shared pointer example
    {
        std::shared_ptr<Node> ptr2 = std::make_shared<Node>(20);
        {
            std::shared_ptr<Node> ptr3 = ptr2; // shared ownership
            std::cout << "Node value from ptr3: " << ptr3->v << '\n';
            std::cout << "Reference count: " << ptr2.use_count() << '\n';
        } // ptr3 goes out of scope here
        std::cout << "Reference count after ptr3 goes out of scope: " << ptr2.use_count() << '\n';
    } // ptr2 goes out of scope here, and Node is automatically deleted

    // weak pointer example
    {
        std::shared_ptr<Node> ptr4 = std::make_shared<Node>(30);
        std::weak_ptr<Node> weakPtr = ptr4; // weak reference
        std::cout << "Reference count: " << ptr4.use_count() << '\n';
        if (auto sharedPtr = weakPtr.lock()) { // try to get shared_ptr
            std::cout << "Node value from weakPtr: " << sharedPtr->v << '\n';
        } else {
            std::cout << "Node has been deleted\n";
        }
    } // ptr4 goes out of scope here, and Node is automatically deleted

    // Placement new example: It allows constructing an object in a pre-allocated memory buffer rather than allocating new memory on the heap.
    char buffer[sizeof(S)];
    S* s = new (buffer) S(42); // placement-new constructs in buffer (stack memory)
    // Means instead of allocating new memory on heap, it uses the provided buffer to construct the object.
    // Note: We must ensure that the buffer is large enough and properly aligned for the object type.
    // Real world use cases of placement new?
    // Placement new is often used in performance-critical applications where memory allocation overhead needs to be minimized
    // It is also used in custom memory management systems, such as memory pools or arenas,
    // where objects are constructed in pre-allocated memory blocks.
    std::cout << "s->x = " << s->x << '\n';
    s->~S(); // must manually call destructor
    return 0;
}

 