#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "Elements in the vector using for_each and lambda function:" << endl;
    for_each(numbers.begin(), numbers.end(), [](int num) {
        cout << num << " ";
    });

    // Above code: this uses lambda function internally:
    // for_each is a standard algorithm that applies the lambda function to each element in the range.
    // The lambda function takes an integer parameter num and prints it.

    cout << "\n\nElements in the vector using range-based for loop:" << endl;
    for (const auto& num : numbers) {
        cout << num << " ";
    }

    return 0;
}