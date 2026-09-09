/*
What is a map in C++?
A map in C++ is an associative container that stores elements in key-value pairs.
Each key is unique, and it is used to access the corresponding value.

How does a map work?
Internally, a map is typically implemented as a balanced binary search tree (like a Red-Black tree).
When you insert a key-value pair into the map, the map organizes the elements based on the keys,
allowing for efficient searching, insertion, and deletion operations.

When to use a map?
Use a map when you need to associate unique keys with specific values and require fast lookups, insertions, and deletions.

*/

#include <iostream>
#include <map>
using namespace std;

int main()
{
    // Creating and initializing a map
    map<int, string> studentMap;
    studentMap[101] = "Raushan";
    studentMap[102] = "Ritu";
    studentMap[103] = "Suchitra";

    // Inserting elements using insert() method
    studentMap.insert({104, "Rahul"});
    studentMap.insert(make_pair(105, "Suman"));

    // Accessing elements using keys
    cout << "Student with ID 102: " << studentMap[102] << endl;
    cout << "Student with ID 104: " << studentMap.at(104) << endl;

    // Iterating through the map
    cout << "All students in the map:" << endl;
    for (const auto& pair : studentMap) {
        cout << "ID: " << pair.first << ", Name: " << pair.second << endl;
    }

    // Checking if a key exists
    int searchID = 101;
    if (studentMap.find(searchID) != studentMap.end()) 
    // Lets understnad this condition: 
    // studentMap.find(searchID) returns an iterator to the element with key searchID if it exists,
    // or studentMap.end() if it does not exist.
    // The condition checks if the iterator returned by find is not equal to end(),
    // which means the key exists in the map.
    {

        cout << "Student with ID " << searchID << " found: " << studentMap[searchID] << endl;
    } else {
        cout << "Student with ID " << searchID << " not found." << endl;
    }

    // Removing an element by key
    studentMap.erase(103); // Removes Suchitra
    cout << "After removing student with ID 103:" << endl;
    for (const auto& pair : studentMap) {
        cout << "ID: " << pair.first << ", Name: " << pair.second << endl;
    }

    return 0;
}