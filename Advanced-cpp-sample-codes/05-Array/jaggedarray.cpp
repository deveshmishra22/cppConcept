#include <iostream>
using namespace std;

int main() {
    int classes = 3;
    int* students[3];     // array of 3 int* (each points to an array)

    int sizes[] = {3, 5, 2};  // number of students in each class

    // Allocate memory dynamically for each class
    for (int i = 0; i < classes; ++i) {
        students[i] = new int[sizes[i]];
        cout << "Enter marks for Class " << i + 1 << ": ";
        for (int j = 0; j < sizes[i]; ++j) {
            cin >> students[i][j];
        }
    }

    // Display the data
    cout << "\n--- Marks by Class ---\n";
    for (int i = 0; i < classes; ++i) {
        cout << "Class " << i + 1 << ": ";
        for (int j = 0; j < sizes[i]; ++j) {
            cout << students[i][j] << " ";
        }
        cout << endl;
    }

    // Free memory
    for (int i = 0; i < classes; ++i)
        delete[] students[i];

    return 0;
}
