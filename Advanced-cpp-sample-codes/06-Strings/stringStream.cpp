// Takes Line as input and converts it to a stream of words
// Compile with: g++ -std=c++17 stringStream.cpp -o stringStream
// Analogy: Think of a string stream like a conveyor belt that takes a long string of text and breaks it down into smaller, manageable pieces (words) that can be processed one at a time.
// Real-world scenario: When processing user input or reading data from a file, you often need to break down a line of text into individual words or tokens for further analysis or processing. String streams provide a convenient way to achieve this in C++.

#include<iostream>
#include<sstream> // for stringstream
#include<string>
#include<vector>
using namespace std;

int main()
{
    string line;
    cout << "Enter a line of text: ";
    getline(cin, line); // read a full line including spaces

    // Create a stringstream object
    stringstream ss(line);

    string name; int id; int age;
    // Example: Extracting formatted data from the stringstream
    cout << "Name: " << (ss >> name, name) << endl;
    cout << "ID: " << (ss >> id, id) << endl;   
    cout << "Age: " << (ss >> age, age) << endl;
    cout << "-------------------------" << endl;
    string word;
    
    // What is vector? 
    // A vector is a dynamic array that can grow and shrink in size. It is part of the C++ Standard Template Library (STL) and provides various functionalities to manage a collection of elements.
    // Analogy: Think of a vector like a flexible container that can hold items (elements
    // such as integers, strings, or objects). You can add more items to the container or remove some, and the container will automatically adjust its size to accommodate the changes.
    // Real-world scenario: Vectors are commonly used in scenarios where the number of elements is
    // not known in advance or can change dynamically, such as storing user inputs, managing a list of tasks, or handling collections of data in applications.
    // Remember to include <vector> header to use vectors.
    // How it helps?
    // 1. Dynamic Sizing: Vectors can grow and shrink as needed, eliminating
    //    the need to predefine the size like traditional arrays.
    // 2. Easy Insertion/Deletion: Vectors provide functions to easily add or
    //    remove elements from the collection.
    // 3. Random Access: Vectors allow direct access to elements using
    //    indexing, similar to arrays.

    // join with stringstream (formatting)
    vector<string> parts = {"one","two","three"};
    stringstream out;
    for (auto &p : parts) 
    {
        out << p << '|' << '\t';
        string joined = out.str();
    }
    cout << "--------------------------------" << endl;
    string joined = out.str();
    cout << "joined: " << joined << '\n';

    // vector<string> words;

    // // Extract words from the stringstream
    // while (ss >> word) {
    //     words.push_back(word);
    // }

    // // Display the extracted words
    // cout << "Extracted words:" << endl;
    // for (const string& w : words) {
    //     cout << w << endl;
    // }

    return 0;
}