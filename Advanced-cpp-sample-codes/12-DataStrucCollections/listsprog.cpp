#include<iostream>
#include<list>
using namespace std;

int main()
{
    list<int> myList = {100, 20, 30, 140, 60};
    list<string> fruits = {"Apple", "Banana", "Cherry"};
    list<int> numbers(5, 100); // List of size 5, initialized with 100s
    list<string> moreFruits;

    auto it = myList.begin();
    advance(it, 1); // Move iterator to the 2nd position
    myList.insert(it, 25); // Insert 25 at the 2nd position

    cout << "Elements in myList after adding 25 at 2nd Position: ";
    for (const auto& elem : myList) {
        cout << elem << " ";
    }
    cout << endl;
    myList.remove(30); // Remove element with value 30
    cout << "myList after removing 30: ";
    for (const auto& elem : myList) {
        cout << elem << " ";
    }
    cout << endl;

    myList.sort(); // Sort the list
    cout << "Sorted myList: ";
    for (const auto& elem : myList) {
        cout << elem << " ";
    }
    cout << endl;

    fruits.reverse(); // Reverse the list
    cout << "Reversed fruits list: ";
    for (const auto& fruit : fruits) {
        cout << fruit << " ";
    }
    cout << endl;

    moreFruits.splice(moreFruits.begin(), fruits, next(fruits.begin()), next(fruits.begin(), 3)); // Move all elements from fruits to moreFruits except the first one
    cout << "moreFruits after splicing from fruits: ";
    for (const auto& fruit : moreFruits) {
        cout << fruit << " ";
    }
    cout << endl;

    cout << "fruits after splicing to moreFruits: ";
    for (const auto& fruit : fruits) {
        cout << fruit << " ";
    }

    list<int> a = {1, 2, 3, 4, 5};
    list<int> b;

    b.splice(b.begin(), a, next(a.begin()), next(a.begin(), 3)); // Move all elements from list a to list b
    cout << "\nElements in list b after splicing from list a: ";
    for (const auto& elem : b) {
        cout << elem << " ";
    }
    cout << endl;
    cout << "Elements in list a after splicing to list b: ";
    for (const auto& elem : a) {
        cout << elem << " ";
    }
    cout << endl;
    return 0;

}

// splice explanation:
// The splice operation transfers elements from one list to another.
// In this example, we transfer all elements from the fruits list to the moreFruits list except the first one.
// We use the splice method of the moreFruits list, specifying the position to insert (the beginning of moreFruits),
// the source list (fruits), and the range of elements to transfer (from the second element to the end).
// This operation is efficient because it does not involve copying or moving the elements;
// it simply adjusts the internal pointers of the lists.
// After the splice operation, the moreFruits list contains "Banana" and "Cherry",
// while the fruits list retains only "Apple".
