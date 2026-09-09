#include <iostream>
#include <fstream>

using namespace std;

void fileWriteOperations() {
    cout << "File operations will be handled here." << endl;
    // RAII principle ensures that resources are released when they go out of scope.
    // In this case, file streams will automatically close when they go out of scope.
    cout << "Enter your name: ";
    string name;
    cin >> name;
    cout << "Enter your age: ";
    int age;
    cin >> age;
    
    ofstream outfile("user_data.txt");
    if (outfile.is_open()) {
        outfile << "Name: " << name << endl;
        outfile << "Age: " << age << endl;
        //outfile.close();        // No need to explicitly close the file, RAII will handle it.

        cout << "User data written to file successfully." << endl;
    } else {
        cerr << "Error creating file." << endl;
    }

}

void fileReadOperations() {
    cout << "File read operations will be handled here." << endl;
    // RAII principle ensures that resources are released when they go out of scope.
    // In this case, file streams will automatically close when they go out of scope.
    ifstream infile("user_data.txt");
    if (infile.is_open()) {
        string line;
        cout << "Reading user data from file:" << endl;
        while (getline(infile, line)) {
            cout << line << endl;
        }
        // infile.close();        // No need to explicitly close the file, RAII will handle it.

    } else {
        cerr << "Error opening file." << endl;
    }
}


int main() {

    // Demonstrating RAII with file operations
    fileWriteOperations();
    fileReadOperations();
    return 0;
}   




// with fstream class we don't need to explicitly close the files
// as the destructor of fstream will take care of closing the files using RAII principle.

// then in which case i need to handle is explicitly closing the files?
// One such case is when you are dealing with temporary files or when you want to ensure
// that a file is closed at a specific point in your code before the object goes out of scope.
// or, when you want to handle errors during file operations
// and need to close the file immediately after an error is detected.
// or, when working with c style file handling using FILE* pointers
// where you need to explicitly call fclose() to release the resource.
