#include<iostream>
#include<string>
using namespace std;

int main()
{
    string oneword;
    cout << "Enter a single word: ";
    cin >> oneword; // Input stops at whitespace
    cout << "You entered: " << oneword << endl;
    cin.ignore(); // Clear the newline character from the input buffer
  
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush rest of line
    string sentence;
    cout << "Enter a sentence: ";
    getline(cin, sentence); // Using getline to allow spaces in the sentence
    cout << "You entered: " << sentence << endl;
    

    // string feedback;
    // cout << "Enter your feedback: ";
    // getline(cin, feedback); // Using getline to allow spaces in the name
    // cout << "Feedback: " << feedback << "\nWelcome to C++ programming raining session with Raushan!" << endl;
    
    
    return 0;
}

