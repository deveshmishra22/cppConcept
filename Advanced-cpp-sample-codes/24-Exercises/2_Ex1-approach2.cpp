/*
-------------------------------------------------------------
🏁 FINAL CHALLENGE (Exercise 1) | Second Approach: STUDENT MARKS ANALYZER
-------------------------------------------------------------
Hey Learner 👋
You’ve learned about arrays, loops, structs, and functions.
Now, let’s bring it all together!

🎯 Goal:
Create a small marks analyzer that calculates total, average,
and finds the topper among students.

💡 Hints:
- Use struct to hold each student’s info.
- Use functions for calculation.
- Use a loop for input and output.
-------------------------------------------------------------
*/

#include <iostream>
#include <iomanip>  // for setw() and formatting
using namespace std;

struct Student {
    string name;
    int rollNo;
    float marks[3];
    float total;
    float average;
};

// Function Definitions
void calculate(Student &s) {
    s.total = 0;
    for (int i = 0; i < 3; i++)
        s.total += s.marks[i];
    s.average = s.total / 3;
}

void display(const Student s[], int n) {
    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << s[i].rollNo
             << setw(10) << s[i].name
             << setw(10) << s[i].total
             << setw(10) << fixed << setprecision(2)
             << s[i].average << endl;
    }
}

int findTopper(const Student s[], int n) {
    int topperIndex = 0;
    float maxAvg = s[0].average;
    for (int i = 1; i < n; i++) {
        if (s[i].average > maxAvg) {
            maxAvg = s[i].average;
            topperIndex = i;
        }
    }
    return topperIndex;
}

int main() {
    int n;
    cout << "Enter number of students: ";
    cin >> n;

    Student *list = new Student[n]; // dynamically allocated array

    // Input student details
    for (int i = 0; i < n; i++) {
        cout << "\nEnter details for Student " << i + 1 << ":\n";
        cout << "Name: ";
        cin >> list[i].name;
        cout << "Roll No: ";
        cin >> list[i].rollNo;
        cout << "Enter marks in 3 subjects: ";
        for (int j = 0; j < 3; j++) {
            cin >> list[i].marks[j];
        }

        calculate(list[i]); // calculate total & average
    }

    // Display results
    cout << "\n---------------------------------------------\n";
    cout << left << setw(10) << "RollNo"
         << setw(10) << "Name"
         << setw(10) << "Total"
         << setw(10) << "Average" << endl;
    cout << "---------------------------------------------\n";

    display(list, n);

    int topperIndex = findTopper(list, n);
    cout << "---------------------------------------------\n";
    cout << "🏆 Topper: " << list[topperIndex].name
         << " (Avg: " << list[topperIndex].average << ")\n";

    delete[] list; // free memory
    return 0;
}



