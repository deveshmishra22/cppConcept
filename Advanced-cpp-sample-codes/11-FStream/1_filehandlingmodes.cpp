#include <iostream>
#include <fstream>
using namespace std;
 
int main() {
    // Step 1: Write initial content (overwrite mode)

    ofstream outFile("output.txt"); // Default is std::ios::out
    if (outFile.is_open()) {
        outFile << "Hello, this is a test file.\n";
        outFile << "Writing to a file using ofstream.\n";
        outFile.close();
        cout << "Initial data written to output.txt\n";
    } else {
        cerr << "Failed to open file for writing.\n";
    }
 
    ofstream outFile2("output.txt"); // Default is std::ios::out
    if (outFile2.is_open()) {
        outFile2 << "Hello, this is a test2 file.\n";
        outFile2 << "2. Writing to a file using ofstream.\n";
        outFile2.close();
        cout << "Initial data written to output.txt\n";
    } else {
        cerr << "Failed to open file for writing.\n";
    }
    // Step 2: Append new content
    ofstream appendFile("output.txt", std::ios::app); // Open in append mode
    if (appendFile.is_open()) {
        appendFile << "This line is appended to the file.\n";
        appendFile << "Appending more data without overwriting.\n";
        appendFile.close();
        cout << "Additional data appended to output.txt\n";
    } else {
        cerr << "Failed to open file for appending.\n";
    }

    // Step 3: Read and display the final content of the file
    ifstream inFile("output.txt");
    if (inFile.is_open()) {
        string line;
        cout << "Final content of output.txt:\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    } else {
        cerr << "Failed to open file for reading.\n";
    }
    return 0;
}
 
