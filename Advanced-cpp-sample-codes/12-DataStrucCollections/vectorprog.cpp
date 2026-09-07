// include<bits/stdc++.h> vs include<vector>
// #include<bits/stdc++.h> is a non-standard header file that includes all standard library headers.
// It is often used in competitive programming for convenience, but it can lead to longer compilation times
// and is not portable across different compilers.
// On the other hand, #include<vector> specifically includes only the vector header from the standard library.
// This is the preferred approach in production code as it improves compilation time and code clarity.

#include <iostream>
#include <vector>
#include<bits/stdc++.h> // Not recommended for production code
using namespace std;

int main()
{
    // Creating and initialising a vector 
    vector<int> numbers = {10, 20, 30, 40, 50};
    vector<string> fruits = {"Apple", "Banana", "Cherry"};
    vector<int> v (5, 100); // Vector of size 5, initialized with 100s

    // Accessing elements using index
    cout << "First number: " << numbers[0] << endl;
    cout << "First fruit: " << fruits.at(0) << endl;

    // Iterating through the vector using range-based for loop
    cout << "Numbers: ";
    for ( auto n: numbers)
    {
        cout << n << " ";
    }

    numbers.reserve(10); // Reserves space for 10 elements
    cout << "\nCapacity after reserve(10): " << numbers.capacity() << endl;
    cout << "Size of vector: " << numbers.size() << endl;

    numbers.push_back(60); // Adds an element at the end
    numbers.push_back(70);
    numbers.push_back(80);
    numbers.push_back(90);
    numbers.push_back(100);
    numbers.push_back(110);
    numbers.shrink_to_fit(); // Reduces capacity to fit size
    cout << "Capacity after shrink_to_fit(): " << numbers.capacity() << endl;


    fruits.erase(fruits.begin() + 1); // Removes the second element (Banana)
    fruits.insert(fruits.begin() + 1, "Blueberry"); // Inserts "Blueberry" at index 1
    fruits.insert(fruits.begin() + 1, "Mango"); // Inserts "Mango" at index 1
    fruits[0] = "Avocado"; // Modifies the first element
    cout << "\nFruits after erasing second element: ";
    for(auto i = fruits.begin(); i != fruits.end(); ++i)
    {
        cout<< *i << " ";
    }
    // for( auto f: fruits)
    // {
    //     cout<< f << " ";
    // }
    numbers.push_back(120);
    numbers.emplace_back(130); // Constructs element in place at the end
    cout << "\nNumbers after emplace_back(130): ";
    numbers.erase(numbers.begin() + 2); // Removes the element at index 2 (30)
    
    sort(numbers.begin(), numbers.end(), greater<int>()); // Sorts in descending order
    numbers.swap(v); // Swaps contents with vector v
    // difference between push_back and emplace_back
    // push_back adds a copy of the element to the end of the vector,
    // while emplace_back constructs the element in place at the end of the vector,
    for( auto &n: numbers)
    {
        cout<< n << " ";
    }



    // why do we need to reserve vector? 
    // Because reserving space in a vector helps to minimize the number of reallocations
    // that occur when new elements are added. Each reallocation involves allocating new memory,
    // copying existing elements to the new memory, and freeing the old memory, which can be expensive.
    // By reserving enough space in advance, we can improve performance when we know the number of elements
    // that will be added to the vector.
    // Real world example: Preallocating memory for a dynamic array that will store user inputs.
    // If you expect to store 1000 integers based on user input, reserving space for 1000 elements
    // in advance can help avoid multiple reallocations as the user adds integers to the array.

}