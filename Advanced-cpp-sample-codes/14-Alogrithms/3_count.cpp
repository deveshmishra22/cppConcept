#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main()
{
    vector<int> arr = {5,5, 3, 8,5, 6, 2, 7, 4, 1};
    
    // Counting occurrences of the number 5
    int count5 = count(arr.begin(), arr.end(), 5);
    cout << "Number of occurrences of 5: " << count5 << endl;

    // Counting occurrences of numbers greater than 4
    int countGreaterThan4 = count_if(arr.begin(), arr.end(), [](int x) { return x > 4; });
    cout << "Number of elements greater than 4: " << countGreaterThan4 << endl;

    return 0;
}