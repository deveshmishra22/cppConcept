#include<iostream>
#include<algorithm>
using namespace std;

int main()
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr)/sizeof(arr[0]);

    // Using sort function from algorithm header
    sort(arr, arr + n);

    cout << "Sorted array Ascending: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;

    // Using sort function to do ascending order
    sort(arr, arr + n, greater<int>());

    cout << "Sorted array Descending: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;

    return 0;
}

// Explanation:
// This code demonstrates the use of the sort function from the C++ Standard Library's algorithm header
// to sort an array of integers in ascending order.
// How it works?
// The sort function takes two iterators as arguments: the beginning and the end of the range
// to be sorted. It rearranges the elements in that range in ascending order by default.
// In this example, we calculate the size of the array and pass the beginning (arr)
// and the end (arr + n) of the array to the sort function.
// After sorting, we print the sorted array to the console.

//  how it is calulating n?
// The expression sizeof(arr) gives the total size in bytes of the array arr.
// The expression sizeof(arr[0]) gives the size in bytes of a single element of the array (which is an int).
// By dividing the total size of the array by the size of one element, we get the
// number of elements in the array.
// Use cases of sort function:
// 1. Sorting Data: To arrange data in a specific order (ascending or descending).
// 2. Preparing for Search: Sorted data allows for efficient searching algorithms like binary search.
// 3. Data Analysis: Sorting is often a preliminary step in data analysis tasks.
// 4. Custom Sorting: The sort function can also take a custom comparator to sort based on specific criteria.
// Can we sort in descending order?
// Yes, by providing a custom comparator function or using predefined comparators like greater<int>,
// we can sort the array in descending order.
// Example of sorting in descending order:
// sort(arr, arr + n, greater<int>());