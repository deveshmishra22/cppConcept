/*
-------------------------------------------------------------
🏁 FINAL CHALLENGE (Exercise 1) | Approach 1: STUDENT MARKS ANALYZER (Simplified)
-------------------------------------------------------------
Hey Learner 👋
This is your final mission for this C++ journey!

🎯 Goal:
Create a marks analyzer that:
1️⃣ Takes student details
2️⃣ Calculates total and average
3️⃣ Finds the topper
4️⃣ Displays results neatly in console

Concepts you’ll apply:
✅ Structs
✅ Arrays
✅ Functions
✅ Loops
✅ Conditionals
✅ Dynamic memory
-------------------------------------------------------------
*/

#include <iostream>
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
    for (int i = 0; i < 3; i++) {
        s.total += s.marks[i];
    }
    s.average = s.total / 3;
}

void display(const Student s[], int n) {
    for (int i = 0; i < n; i++) {
        cout << s[i].rollNo << "\t"
             << s[i].name << "\t"
             << s[i].total << "\t"
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

    cout << "\n---------------------------------------------\n";
    cout << "RollNo\tName\tTotal\tAverage\n";
    cout << "---------------------------------------------\n";

    display(list, n);

    int topperIndex = findTopper(list, n);
    cout << "---------------------------------------------\n";
    cout << "🏆 Topper: " << list[topperIndex].name
         << " (Avg: " << list[topperIndex].average << ")\n";

    delete[] list; // free memory
    return 0;
}

