#include <iostream>
#include <set>
using namespace std;

int main()
{
    set<int> numSet = {10, 20, 30, 40, 50, 50, 30, 20, 10, 20}; // Duplicates will be ignored

    // Inserting elements
    numSet.insert(60);
    numSet.insert(70);
    numSet.insert(20); // Duplicate, will be ignored
    cout << "Elements in the set after insertions: ";
    for (const auto& num : numSet) {
        cout << num << " ";
    }
    cout << endl;

    cout<< "Has 30? " << (numSet.count(30) ? "Yes" : "No") << endl;
    cout<< "Has 80? " << (numSet.find(80) != numSet.end() ? "Yes" : "No") << endl;

    auto it = numSet.lower_bound(30);;
    if (it != numSet.end()) {
        cout << "Lower bound of 30: " << *it << endl;
    } else {
        cout << "Lower bound of 30 not found." << endl;
    }

    numSet.erase(40); // Remove element 40
    cout << "Elements in the set after erasing 40: ";
    for (const auto& num : numSet) {
        cout << num << " ";
    }
    cout << endl;

    // Custom comparator for descending order
    // Explanation:
    // Here, we create a set with a custom comparator 'greater<int>'
    // which sorts the elements in descending order.
    // This demonstrates how to use custom sorting criteria with sets in C++.
    // How it works?
    // By specifying 'greater<int>' as the second template parameter for the set,
    // we enable custom sorting behavior.
    // Can we use smaller than operator or ascending order?
    // Yes, by default, sets use 'less<int>' which sorts elements in ascending order.
    // Use cases of custom comparators:
    // 1. Custom Sorting: To sort elements in a specific order (e.g., descending).
    // 2. Complex Data Types: When storing user-defined types, custom comparators
    //    can define how to compare those types.
    // 3. Priority Queues: Implementing priority-based data structures.
    
    set<int, greater<int>> descSet;
    descSet.insert(10);
    descSet.insert(20);
    descSet.insert(30);
    cout << "Elements in descending order set: ";
    for (const auto& num : descSet) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}