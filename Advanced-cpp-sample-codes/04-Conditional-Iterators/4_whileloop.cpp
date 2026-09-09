#include<iostream>
using namespace std;
int main()
{
    cout << "Welcome to C++ Learnings with Raushan!" << endl;
    int x=1;
    while (x >= 0)
    {
        cout << "Enter any number: " << endl;
        int n;
        cin >> n;
        if(n<0)
        {
            cout << "Negative number entered. Exiting the loop." << endl;
            break;
        }
        cout << "You entered: " << n << endl;
        cout << "Square of " << n << " is " << n*n << endl;
        cout << "Cube of " << n << " is " << n*n*n << endl
                << endl;
          
    }
    
    return 0;
}