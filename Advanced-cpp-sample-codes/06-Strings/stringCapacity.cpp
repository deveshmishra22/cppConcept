#include<iostream>
#include<string>
using namespace std;

int main() {
    string str = "Hello, World!";

    cout << "String: " << str << endl;
    cout << "Size: " << str.size() << endl;          // Number of characters in the string
    cout << "Length: " << str.length() << endl;      // Same as size()
    cout << "Capacity: " << str.capacity() << endl;  // Size of allocated storage
    cout << "Max Size: " << str.max_size() << endl;  // Maximum size the string can reach

    str.reserve(500); // Reserve space for at least 50 characters
    cout << "After reserving capacity of 500:" << endl;
    cout << "String: " << str << endl;
    cout << "Size: " << str.size() << endl;          // Number of characters in the string
    cout << "Length: " << str.length() << endl;      // Same as size()
    cout << "Capacity: " << str.capacity() << endl;  // Size of allocated storage
    cout << "Max Size: " << str.max_size() << endl; // Maximum size the string can reach

    string s;
    cout << "Initial capacity of empty string: " << s.capacity() << endl;
    s += "C++ Programming";
    cout << "Capacity after adding 'C++ Programming': " << s.capacity() << endl;

    s.reserve(1000); // Reserve space for at least 1000 characters
    cout << "Capacity after reserving 1000: " << s.capacity() << endl;
    for(int i = 0; i < 800; i++) 
    {
        s += 'a'; // Append 800 'a' characters
    };
    cout << "Capacity after adding 800 'a' characters: " << s.capacity() << endl;

    s.shrink_to_fit(); // Reduce capacity to fit size
    cout << "Capacity after shrink_to_fit: " << s.capacity() << endl;

    s.clear(); // Clear the string
    cout << "Capacity after clearing the string: " << s.capacity() << endl;

    cout << "Final String: '" << s << "'" << endl;

    return 0;
}

// Where do we use string capacity functions?
// 1. Performance Optimization: Pre-allocating memory to avoid multiple reallocations.
// 2. Memory Management: Controlling memory usage in applications with large strings.
// 3. Dynamic String Handling: Efficiently managing strings that change size frequently.
// 4. Game Development: Managing in-game text and dialogues efficiently.
// 5. Real-time Systems: Ensuring predictable memory usage and performance.
// 6. Text Processing Applications: Handling large volumes of text data efficiently.
// 7. Network Programming: Managing buffers for incoming and outgoing data.
// 8. Embedded Systems: Optimizing memory usage in resource-constrained environments.
// 9. Database Systems: Efficiently managing string data in databases.
// 10. User Interface Development: Handling dynamic text in UI elements efficiently.
// This code demonstrates the use of string capacity functions in C++.
// It shows how to check and manipulate the capacity of strings for performance optimization and memory management.

// How it works?
// 1. The program starts by including the necessary headers and using the std namespace.
// 2. It initializes a string with "Hello, World!" and prints its size, length, capacity, and max size.
// 3. It then reserves a capacity of 500 for the string and prints the updated properties.
// 4. An empty string is created, and its initial capacity is printed.
// 5. The program appends "C++ Programming" to the string and prints the new capacity.
// 6. It reserves a capacity of 1000 and appends 800 'a' characters, printing the capacity after each operation.
// 7. The shrink_to_fit function is called to reduce the capacity to fit the current
//    size, and the new capacity is printed.
// 8. The string is cleared, and the capacity is printed again.
// 9. Finally, the program prints the final state of the string and returns 0 to indicate successful execution. size, and the new capacity is printed.
// 8. The string is cleared, and the capacity is printed again.
// 9. Finally, the program prints the final state of the string and returns 0 to indicate successful execution. size, and the new capacity is printed.

