#include<iostream>
using namespace std;

int main()
{
    int a=11;
    int * p = &a; // pointer variable p holds the address of variable a
    cout << "Value of a: " << a << endl;
    cout << "Address of a: " << &a << endl;
    cout << "Value of pointer p (Address of a): " << p << endl;
    a = 15; // changing value of a
    cout << "New value of a: " << a << endl;
    cout << "Value of pointer p (Address of a): " << p << endl; //
    cout << "Value pointed by pointer p: " << *p << endl; //


    int arr[3] = {10, 20, 30};
    int *ptr = arr; // pointer variable ptr holds the address of first element of array arr
    cout << "Array elements using pointer: " << endl;
    cout << "Address of first element of array arr: " << ptr << endl;
    for(int i = 0; i < 3; i++)
    {
        cout << *(ptr + i) << " "; // accessing array elements using pointer arithmetic
    }
    return 0;
}