// Iterators in C++
// An iterator is an object that enables a programmer to traverse a container, particularly lists.
// The iterator pattern decouples algorithms from containers; in some cases, algorithms are implemented independently of the container's structure.
// Accessing elements through iterators allows for more generic and reusable code.
// C++ provides several types of iterators, including input iterators, output iterators, forward iterators, bidirectional iterators, and random access iterators.
// Mover iterators are a type of iterator that allows the resources of an object to be moved rather than copied, enhancing performance by eliminating unnecessary deep copies.
// Compared to regular iterators, mover iterators transfer ownership of the resources they point to, leaving the original object in a valid but unspecified state.

//std::vector with begin() and end() iterators: 
// std::vector is a sequence container that encapsulates dynamic size arrays.
// The begin() function returns an iterator pointing to the first element in the vector.
// The end() function returns an iterator pointing to one past the last element in the vector.

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int main() {
    vector<int> numbers = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    cout << "Elements in the vector using iterators:" << endl;
    for (auto iterator = numbers.begin(); iterator != numbers.end(); ++iterator) {
        cout << *iterator << " ";
    }

    // range-based for loop (C++11 and later)
    cout << "\n\nElements in the vector using range-based for loop:" << endl;
    for (const auto& num : numbers) {
        cout << num << " ";
    }

    map<string, int> ageMap = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };
    cout << "\n\nElements in the map using iterators:" << endl;
    for (auto it = ageMap.begin(); it != ageMap.end(); ++it) {
        cout << it->first << ": " << it->second << endl;
    }

    // Reverse iteration using reverse iterators
    cout << "\nElements in the vector in reverse order using reverse iterators:" << endl;
    for (auto rit = numbers.rbegin(); rit != numbers.rend(); ++rit) {
        cout << *rit << " ";
    }

    // Is there any fbegin() and fend()?
    // No, there are no fbegin() and fend() functions in the C++ Standard Library.
    // However, C++17 introduced the functions std::cbegin() and std::cend(), which return constant iterators to the beginning and end of a container, respectively.
    // These functions are useful when you want to ensure that the iterators cannot be used to modify the elements of the container.

    cout << "\n\nElements in the vector using std::cbegin() and std::cend():" << endl;
    for (auto it = std::cbegin(numbers); it != std::cend(numbers); ++it) {
        cout << *it << " ";
    }

    // difference between reverse function vs rbegin() and rend() iterators?
    // The reverse() function is a standard algorithm in C++ that reverses the order of elements in a given range (e.g., a vector or array).
    // It modifies the original container in place.
    // On the other hand, rbegin() and rend() are member functions of standard containers (like vector, list, etc.) that return reverse iterators.
    // rbegin() returns an iterator to the last element of the container, and rend() returns an iterator to one before the first element.
    // Using rbegin() and rend(), you can iterate over the container in reverse order without
    // modifying the original container.

    cout << "\nReversing the vector using std::reverse():" << endl;
    reverse(numbers.begin(), numbers.end());
    for (const auto& num : numbers) {
        cout << num << " ";
    }


    // What is const auto& here?
    // In the range-based for loop, 'const auto& num' means that 'num' is a constant reference to each element in the 'numbers' vector.
    // Using 'const' ensures that the elements are not modified within the loop.
    // Using '&' avoids unnecessary copies of the elements, making the loop more efficient.
    // This is particularly important for large objects or complex data types.
    // For simple data types like int, the performance difference is negligible.
    // how it is different from auto num?
    // Using 'auto num' would create a copy of each element in the vector for use within the loop.
    // This means that any modifications to 'num' would not affect the original elements in the vector.
    // In contrast, 'const auto& num' creates a reference to the original element, preventing modifications and avoiding the overhead of copying.
    //     for (auto iterator = numbers.begin(); iterator != numbers.end(); ++iterator) vs for (const auto& num : numbers)
    // The first loop uses explicit iterators to traverse the vector, while the second loop uses a range-based for loop for more concise syntax.
    // Both loops achieve the same result, but the range-based for loop is often preferred for its simplicity and readability.

    // ++iterator vs iterator++ here:
    // In this context, both ++iterator (pre-increment) and iterator++ (post-increment) will yield the same result.
    // However, ++iterator is generally preferred for iterators because it can be more efficient.
    // Pre-increment modifies the iterator and returns the modified iterator, while post-increment creates a copy of the iterator before incrementing it, which can be less efficient for complex iterator types.
    cout << endl;

    return 0;
}


// What is the meaning of noexcept in C++?
// In C++, noexcept is a specifier used to indicate that a function does not throw exceptions
// It can be applied to function declarations and definitions.
// When a function is marked as noexcept, the compiler can make optimizations based on the guarantee that the function will not throw exceptions.
// If a noexcept function does throw an exception, the program will call std::terminate, leading to abnormal termination.
// Example usage of noexcept:
// void myFunction() noexcept {
//     // Function implementation that guarantees no exceptions will be thrown
// }
// Why use noexcept?
// Performance Optimization: The compiler can optimize calls to noexcept functions better than those that may throw exceptions
// Code Clarity: It provides clear documentation to users of the function about its exception behavior
// Exception Safety: It helps in designing exception-safe code by clearly indicating which functions are not expected to throw exceptions