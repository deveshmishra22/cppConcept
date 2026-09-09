#include<iostream>
using namespace std;

int main()
{
    cout << "How many elements you want to store in array: ";
    int n;
    cin >> n;
    int arr[n];
    for(int i = 0; i < n; i++)
    {
        cout << "Enter element " << i + 1 << ": ";
        cin >> arr[i];
    }   
    
    cout << "The elements in the array are: ";
    for(int n: arr)
    {
        cout << n << "  ";
    }
}