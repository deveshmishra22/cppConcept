#include<iostream>
using namespace std;
int main()
{
    cout << "Welcome to C++ Learnings with Raushan!" << endl;
    int n;
    cout << "Enter a number to print its multiplication table: " << endl;
    cin >> n;
    cout << "Multiplication Table of " << n << " is: " << endl;
    for(int i=1; i<=10; i++)
    {
        cout << n << " x " << i << " = " << n*i << endl;
    }
    return 0;
}