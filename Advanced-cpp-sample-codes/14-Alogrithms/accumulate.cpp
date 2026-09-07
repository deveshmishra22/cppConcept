#include <iostream>
#include <algorithm>
#include<vector>
#include<numeric> // For accumulate
using namespace std;

int main()
{
    vector<int> arr = {10, 20, 30, 40, 50};

    // Using std::accumulate to calculate the sum of elements
    int sum = accumulate(arr.begin(), arr.end(),0);
    cout << "Sum of elements: " << sum << endl;

    // Using std::accumulate to calculate the product of elements
    int product = accumulate(arr.begin(), arr.end(), 1, [](int a, int b) {
        return a * b;
    });
    cout << "Product of elements: " << product << endl;

    return 0;
}
