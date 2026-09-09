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

// Explanation:
// In this code, we demonstrate the use of pointers in C++. We declare an integer variable
// `a` and a pointer variable `p` that holds the address of `a`. We print the value of `a`, its address, 
// and the value of the pointer `p`. We then change the value of `a` and show that the pointer still 
// holds the same address, but the value it points to has changed.
// We also demonstrate how to use pointers with arrays. We declare an array `arr` and a 
// pointer `ptr` that points to the first element of the array. We use pointer arithmetic to 
// access and print the elements of the array.
// Note: The output of the program will show the value of `a`, its address, the value of 
// the pointer `p`, and the elements of the array accessed through the pointer `ptr`.
// Real-world scenario: Pointers are often used in dynamic memory allocation, data structures 
// like linked lists, and for efficient array manipulation. Understanding pointers is crucial for 
// low-level programming and performance optimization in C++.
// Analogy: Think of a pointer as a signpost that points to a specific location (memory address) 
// where data is stored. Just like a signpost can point to different locations, a pointer can point 
// to different variables or array elements in memory.
// Note: Always ensure that pointers are initialized before use and avoid dereferencing null or 
// uninitialized pointers to prevent undefined behavior.