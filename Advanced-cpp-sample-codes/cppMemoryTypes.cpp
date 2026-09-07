// What is memory leaks: Memory leaks occur when a program allocates memory but fails to release it back to the system after it's no longer needed. This can lead to increased memory usage over time and may eventually cause the program to run out of memory.
// Memory types:
// 1. Stack Memory: Used for static memory allocation. Variables are automatically allocated and deallocated when they go out of scope.
// 2. Heap Memory: Used for dynamic memory allocation. Memory is manually allocated and de
// allocated using new and delete keywords.
// 3. Static/Global Memory: Used for global and static variables. Memory is allocated
// when the program starts and deallocated when the program ends.
// 4. Code/Text Segment: Contains the compiled program code. This memory is read-only and shared among processes.
// How to manage memory effectively in C++:
// 1. Use smart pointers (unique_ptr, shared_ptr) to manage dynamic memory automatically
// 2. Avoid memory leaks by ensuring every new has a corresponding delete
// 3. Use RAII (Resource Acquisition Is Initialization) principles to tie resource management to
// object lifetime
// 4. Regularly profile and monitor memory usage to identify potential leaks or inefficiencies
#include <iostream>
#include <memory> // For smart pointers
#include <vector>
#include <string>
using namespace std;

// global variable (static/global memory)
int globalVar = 50; // data segment (initialized global variable)
static int staticGlobalVar; // bss segment (uninitialized global variable)

int global_var = 42;            // data segment (initialized)
static int static_var;          // BSS (uninitialized)
 
void showAddresses() {
    int local = 10;             // stack
    static int local_static = 20; // data segment (static local)
    int* heap = new int(99);    // heap
 
    std::string s = "literal";  // s (object on stack), internal buffer likely on heap
    const char* lit = "hello";  // string literal (read-only data)
 
    std::cout << "Code (func ptr)    : " << (void*)&showAddresses << '\n';
    std::cout << "Global var         : " << (void*)&global_var << '\n';
    std::cout << "Static var         : " << (void*)&static_var << '\n';
    std::cout << "Local static       : " << (void*)&local_static << '\n';
    std::cout << "Local (stack)      : " << (void*)&local << '\n';
    std::cout << "Heap pointer->     : " << (void*)heap << '\n';
    std::cout << "std::string object : " << (void*)&s << '\n';
    std::cout << "string literal     : " << (const void*)lit << '\n';
 
    delete heap;
}

void heapAllocateDeallocateExample() {

    int * p = new int; // dynamically allocate an integer on the heap
    *p = 42; // assign a value to the allocated memory
    cout << "Dynamically allocated integer value: " << *p << endl;
    delete p; // deallocate the memory to prevent memory leak

    int * arr = new int[5]; // dynamically allocate an array of integers on the heap
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10; // assign values to the array
    }
    cout << "Dynamically allocated array values: ";
    for (int i = 0; i < 5; ++i) {
        cout << arr[i] << " "; // print the array values
    }
    delete[] arr; // deallocate the array memory
}


void leakMemoryExample() {
    int * leak = new int[1000]; // allocate memory but never deallocate
    // Memory leak occurs here as we lose the reference to the allocated memory
    cout << "Leaked memory allocated." << endl;
    // Note: In a real application, you would want to avoid this situation.
    (void)leak; // to avoid unused variable warning

}

void doubefreeExample() {
    int* ptr = new int(10);
    delete ptr; // First deletion
    //delete ptr; // Uncommenting this line would cause a double free error

    int * a = new int(5);
    int * b = a;
    cout << "b: " << *b << endl;
    cout << "a: " << *a << endl;
    delete a; // First deletion
    cout<< "b, after deleting a: " << *b << endl;
}

void safesingleallocExample() {
    auto p = make_unique<int>(10); // Using unique_ptr for safe memory management
    cout << "Value managed by unique_ptr: " << *p << endl;

    auto q = make_shared<int>(20); // Using shared_ptr for shared ownership
    cout << "Value managed by shared_ptr: " << *q << endl;
    cout << "Reference count of shared_ptr: " << q.use_count() << endl; 
    shared_ptr<int> r = q; // Sharing ownership
    cout << "Reference count after sharing ownership: " << q.use_count() << endl;

}

void recurseiveFunction(int count) {
    if (count == 0) return;
    volatile int arr[1000]; // Large array on stack to demonstrate stack usage 
    if(count%100 == 0) cout << "Recursion depth: " << count << endl;
    recurseiveFunction(count - 1);
}

struct Big {
    std::string s;
    Big(const char* t) : s(t) {}
    Big(const Big& o) : s(o.s) { std::cout << "Copy ctor\n"; }
    Big(Big&& o) noexcept : s(std::move(o.s)) { std::cout << "Move ctor\n"; }
};

int main() {

    // Demonstrate move semantics with Big struct
    std::vector<Big> v;
    v.reserve(2);
    v.emplace_back("A"); // construct in-place, no move/copy
    v.emplace_back("B");
    std::cout << "Pushing third element (will reallocate if capacity insufficient)\n";
    v.emplace_back("C"); // if reallocation needed, move ctor used if available
    v.emplace_back("D"); // if reallocation needed, move ctor used if available
    // why now move constructor is called twice?
    // Because when the vector needs to reallocate its internal storage to accommodate new elements,
    // it moves existing elements to the new storage using the move constructor (if available) to avoid expensive copies.
    std::cout << "Done\n";

    // Demonstrate safe memory management using smart pointers
    safesingleallocExample();
    try{
        // Demonstrate recursive function to show stack memory usage
    recurseiveFunction(1000); // Adjust the depth as needed
    }
    catch(const std::exception& e){
        cerr << "Exception occurred: " << e.what() << endl;
    }

    // Demonstrate double free error
    doubefreeExample(); // Uncomment to see double free error (undefined behavior)

    // Demonstrate memory leak
    // for(int i = 0; i < 10000; ++i) {
    //     leakMemoryExample();
    //     // In a real application, we would monitor memory usage here to see the leak effect
    //     // how even it is possible to monitor memory leak in C++?
    //     // We can use tools like Valgrind (on Linux) or Visual Studio's built-in diagnostic tools (on Windows) to monitor memory usage and detect leaks.
    //     // These tools can help identify memory that was allocated but not properly deallocated.

    // }

    // Demonstrate heap allocation and deallocation
    heapAllocateDeallocateExample();

    // Show memory addresses
    showAddresses();

    cout << "C++ Memory Types and Management" << endl;

    // Stack Memory Example
    int stackVar = 10; // Allocated on stack
    cout << "Stack Variable: " << stackVar << endl;

    // Heap Memory Example
    int* heapVar = new int(20); // Allocated on heap
    cout << "Heap Variable: " << *heapVar << endl;
    delete heapVar; // Deallocate heap memory

    // Static/Global Memory Example
    static int staticVar = 30; // Allocated in static memory
    cout << "Static Variable: " << staticVar << endl;

    // Code/Text Segment is not directly accessible in code, but we can mention it.
    cout << "Code/Text Segment contains the compiled program code." << endl;

    const char* str = "Hello, World!"; // string literal stored in code/text segment
    cout << str << endl;

    cout << "Global Variable: " << globalVar << endl;

    // How to verify memory types?
    // We can use tools like Valgrind (on Linux) or Visual Studio's built
    // in diagnostic tools (on Windows) to monitor memory usage and detect leaks.
    // These tools can help identify memory that was allocated but not properly deallocated.

    // When to use which memory type?
    // - Use stack memory for local variables with a short lifetime.
    // - Use heap memory for objects that need to persist beyond the scope of a function or have a large size.
    // - Use static/global memory for variables that need to maintain state across function calls or throughout
    // the program's lifetime.
    // - Code/Text segment is managed by the operating system and is not directly controlled by the programmer.
    // Proper memory management is crucial for efficient and reliable C++ programs.


    // Language terms:
    // 1. Memory Allocation: The process of reserving a block of memory for use by a program.
    // 2. Memory Deallocation: The process of releasing previously allocated memory back to the
    // system.
    // 3. Dangling Pointer: A pointer that points to a memory location that has
    // already been deallocated.
    // 4. Fragmentation: The condition where free memory is divided into small,
    // non-contiguous blocks, making it difficult to allocate large contiguous memory blocks.
    // 5. Garbage Collection: An automatic memory management technique where the system
    // periodically identifies and frees memory that is no longer in use by the program.
    // C++ does not have built-in garbage collection, but smart pointers can help manage memory automatically.
    // 6. RAII (Resource Acquisition Is Initialization): A programming idiom where resource allocation
    // is tied to the lifetime of objects. Resources are acquired during object creation and released during
    // object destruction.

    // what is storage duration in C++?
    // Storage duration determines the lifetime of a variable and when its memory is allocated and deallocated.
    // There are four types of storage duration in C++:
    // 1. Automatic Storage Duration: Variables are automatically allocated and deallocated when they go out of scope.
    // 2. Static Storage Duration: Variables are allocated when the program starts and deallocated when it ends.
    // 3. Dynamic Storage Duration: Variables are allocated and deallocated manually using new and delete.
    // 4. Thread Storage Duration: Variables are allocated for the lifetime of a thread.

    // Safety and Common bugs:
    // 1. Memory Leaks: Failing to deallocate memory can lead to increased
    // memory usage over time.
    // 2. Dangling Pointers: Accessing memory that has already been deallocated
    // can lead to undefined behavior.
    // 3. Buffer Overflows: Writing beyond the allocated memory can corrupt data
    // and lead to security vulnerabilities.
    // 4. Double Free: Deallocating the same memory block more than once can
    // lead to undefined behavior. But how it happens?
    // It happens when a program calls delete or free on the same memory address multiple times.
    // or shared pointers going out of scope together.
    // This can corrupt the memory management data structures, leading to crashes or unpredictable behavior.
    // 5. Use After Free: Accessing memory after it has been deallocated can
    // lead to undefined behavior.
    // To avoid these issues, use smart pointers, follow RAII principles, and utilize
    // memory profiling tools to monitor and manage memory effectively.
    return 0;
}


//& vs && in C++
// & is the address-of operator, used to get the memory address of a variable.
// && is the rvalue reference operator, used to bind to temporary objects (rvalues) for move semantics.