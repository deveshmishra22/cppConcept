#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

int main()
{
    vector<int> arr = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int n = arr.size();
    int target = 70;

    // Using std::find to search for the target element
    auto it = find(arr.begin(), arr.end(), target);

    if (it != arr.end()) {
        cout << "Element " << target << " found at index: " << distance(arr.begin(), it) << endl;
    } else {
        cout << "Element " << target << " not found in the array." << endl;
    }

    return 0;
}

// Find algorithm in C++
// The find algorithm is a part of the C++ Standard Library's <algorithm> header.
// It is used to search for a specific value within a range of elements.
// How it works?
// The find function takes three parameters: two iterators defining the range to search
// (begin and end) and the value to search for.
// It returns an iterator pointing to the first occurrence of the value within the range.
// If the value is not found, it returns the end iterator.
// In this example, we create a vector of integers and use the find function
// to search for a target value (70).
// If the value is found, we calculate the index using the distance function
// and print it; otherwise, we indicate that the element was not found.
// Use cases of find algorithm:
// 1. Searching Elements: To locate a specific value in a collection.
// 2. Validating Presence: To check if an element exists in a dataset.
// 3. Data Processing: In algorithms that require searching for elements as part of their logic
// Can we use find with other data structures?
// Yes, the find algorithm can be used with various data structures that support iterators,
// such as arrays, vectors, lists, and deques.
// Example with array:
// int arr[] = {10, 20, 30, 40, 50};
// auto it = find(begin(arr), end(arr), 30);
// In this example, we use the find function to search for the value 30 in a C-style array.
// Example with list:
// #include <list>
// list<int> myList = {10, 20, 30, 40, 50};
// auto it = find(begin(myList), end(myList), 30);
// In this example, we use the find function to search for the value 30 in a std::list.
// Conclusion:
// The find algorithm is a powerful and versatile tool in C++ for searching elements
// within various data structures, making it easier to implement search functionality
// in your programs.
