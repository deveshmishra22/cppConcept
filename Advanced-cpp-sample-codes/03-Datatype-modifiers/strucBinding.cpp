#include <iostream>
#include <chrono>
#include<utility>
#include <tuple>

using namespace std;
using namespace std::chrono;

pair<int, string> getPerson() {
    return {25, "Alice"};
}

constexpr double interestRate = 7.5;
 
tuple<int, string, double> getCustomer() {
    return {101, "Sanjiva", 50000.25};
}
int main()
{
    auto [age, name1] = getPerson();
    cout << "Name: " << name1 << ", Age: " << age << endl;
    cout << "------------------------" << endl; 
    auto [id, name2, balance] = getCustomer();

    cout << "Customer ID: " << id << endl;
    cout << "Name: " << name2 << endl;
    cout << "Balance: " << balance << endl;
 
    decltype(balance) newBalance = balance + (balance * interestRate / 100);
    cout << "New Balance (with interest): " << newBalance << endl;
    return 0;
}

// Explanation:
// This code demonstrates the use of structured bindings in C++17.
// The getPerson function returns a pair containing an integer (age) and a string (name).
// In the main function, structured bindings are used to unpack the returned pair into two separate variables
// age and name. This allows for more readable and convenient access to the individual elements of the pair.
// Structured bindings can be used with various types such as pairs, tuples, and structs,
// making it easier to work with multiple return values and complex data structures.
// How it works?
// When the getPerson function is called, it returns a pair<int, string>.
// The structured binding declaration auto [age, name] unpacks the pair into two variables:
// age and name. The compiler automatically deduces the types of these variables based on the types
// in the pair.
// This feature enhances code readability and reduces the need for explicit type declarations
// when dealing with multiple return values or complex data structures.
// Use cases of structured bindings:
// 1. Returning Multiple Values: Functions can return multiple values in a more readable way.
// 2. Working with Tuples and Pairs: Simplifies access to elements in tuples
//    and pairs without needing to use std::get.
// 3. Range-based for Loops: Can be used to unpack elements in range-based
//    for loops when iterating over collections of pairs or tuples.
// 4. Decomposing Structs: Allows for easy unpacking of struct members into
//    separate variables.
// 5. Improved Readability: Enhances code clarity by providing meaningful
