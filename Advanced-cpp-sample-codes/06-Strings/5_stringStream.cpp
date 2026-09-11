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
    // string line;
    // cout << "Enter a line of text: ";
    // getline(cin, line); // read a full line including spaces

    // // Create a stringstream object
    // stringstream ss(line); // why use stringstream?
    // // A stringstream allows us to treat a string as a stream, enabling us to extract
    // // individual words or tokens from the string easily. It provides a convenient way to parse 
    // // and manipulate strings in C++.
    // // Analogy: Think of a stringstream as a tool that takes a long string of text and breaks it 
    // // down into smaller, manageable pieces (words) that can be processed one at a time.
    // // Real-world scenario: When processing user input or reading data from a file, you often
    // // need to break down a line of text into individual words or tokens for further analysis or 
    // // processing.
    // // String streams provide a convenient way to achieve this in C++.

    // string name; int id; int age;
    // // Example: Extracting formatted data from the stringstream
    // cout << "Name: " << (ss >> name, name) << endl;  
    // // Extract name and display it through local variable.
    // //can we directly use ss >> name in cout? Yes, but it would not allow us to store the 
    // // extracted value for later use. 
    // // By using a local variable (name), we can store the extracted value and use it later 
    // // in the program if needed.
    // cout << "ID: " << (ss >> id, id) << endl;   
    // cout << "Age: " << (ss >> age, age) << endl;
    // cout << "-------------------------" << endl;
    // string word;

    // // why to use stringstream to extract words from a line of text, why can't we just use find and 
    // // substr?
    // // Using stringstream to extract words from a line of text is more convenient and efficient than
    // // using find and substr.
    // // 1. Simplicity: Stringstream provides a simple and intuitive way to extract
    // //    words from a line of text using the extraction operator (>>), which automatically handles 
    // // whitespace and tokenization.
    // // 2. Efficiency: Stringstream is optimized for extracting words from a string, making it more 
    // // efficient than manually 
    // // searching for spaces and extracting substrings using find and substr.
    // // 3. Readability: Using stringstream makes the code more readable and easier to understand, as 
    // // it clearly conveys the 
    // // intention of extracting words from a line of text without the need for complex logic or 
    // // manual string manipulation.
    // while (ss >> word) {
    //     cout << "Extracted word: " << word << endl;
    // }
    // // vs
    // string::find and string::substr
    // Using string::find and string::substr to extract words from a line of text is more manual and requires additional 
    // logic to handle whitespace and tokenization.
    // 1. Complexity: You need to manually search for spaces using string::find and extract substrings using string::substr, 
    // which can be more complex and error-prone compared to using stringstream.
    // 2. Efficiency: Manually searching for spaces and extracting substrings can be less efficient than using stringstream, 
    // especially for longer strings or when extracting multiple words.
    // 3. Readability: The code using string::find and string::substr can be harder to read and maintain compared to using 
    // stringstream.
    
    // What is vector? 
    // A vector is a dynamic array that can grow and shrink in size. It is part of the C++ Standard Template Library (STL) 
    // and provides various functionalities to manage a collection of elements.
    // Analogy: Think of a vector like a flexible container that can hold items (elements
    // such as integers, strings, or objects). You can add more items to the container or remove some, and the 
    // container will automatically adjust its size to accommodate the changes.
    // Real-world scenario: Vectors are commonly used in scenarios where the number of elements is
    // not known in advance or can change dynamically, such as storing user inputs, managing a list of tasks, or 
    // handling collections of data in applications.
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
        cout << "joined part by part: " << joined << '\n';
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

// dry-run the code and check the output.
// Explanation:
// In this code, we demonstrate the use of string streams in C++ to extract words from
// a line of text. We use the `stringstream` class to treat a string as a stream, 
// allowing us to extract individual words using the extraction operator (>>). 
// We also show how to store the extracted words in a vector for further processing or display. 
// Additionally, we demonstrate how to join strings using string streams for formatting purposes.

// Real-world scenario: String streams are commonly used in scenarios where you need to parse and 
// process text data,
// such as reading user input, processing log files, or handling data from external sources.

// input:
// Enter a line of text: John 123 25
// output:
// Name: John
// ID: 123
// Age: 25

// if we have different input like "My name is Alice and my id is 456 and my age is 30", the output will be:
// Name: My
// ID: name
// Age: is
// This is because the extraction operator (>>) reads the input until it encounters whitespace,