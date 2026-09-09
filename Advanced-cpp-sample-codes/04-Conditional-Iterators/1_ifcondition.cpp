#include<iostream>
using namespace std;

int main() {
    cout << "Welcome to C++ Learnings with Raushan!" << endl;
    cout << "Please Enter Your Name:" << endl;
    string name;
    cin >> name;
    cout << "Hello, " << name << "! Let's start learning C++ together." << endl;

    cout << "\nEnter any number" << endl;
    int number;

    cin >> number;
    if(number>0)
    {
        cout << number << " is a Positive Number." << endl;
    }
    else if(number<0)
    {
        cout << number << " is a Negative Number." << endl;
    }
    else
    {
        cout << "You entered Zero." << endl;
    }
    return 0;
}