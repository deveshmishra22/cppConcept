#include<iostream>
#include<string>
using namespace std;

int main()
{
    int age;
    cout << "Enter your age: ";
    cin >> age;

    string eligibility = (age >= 18) ? "You are eligible to vote." : "You are not eligible to vote.";
    cout << eligibility << endl;

    return 0;
}