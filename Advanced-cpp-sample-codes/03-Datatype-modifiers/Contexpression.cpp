#include <iostream>
using namespace std;

constexpr int square(int x) {
    return x * x;
}

int main() {
    constexpr int result = square(5);
    cout << "The square of 5 is: " << result << endl;
    
    cout<< "Calling square func temp: " << endl;
    cout<< "The square of 10 is: " << square(10) << endl;
    return 0;
}


// #include <iostream>
// #include <chrono>
// using namespace std;
// using namespace std::chrono;

// constexpr int square(int x) {
//     return x * x;
// }

// int runtime_square(int x) {
//     return x * x;
// }

// int main() {
//     // Compile-time computation
//     constexpr int result = square(5);
//     cout << "Compile-time square of 5: " << result << endl;

//     // Runtime computation (constexpr used at runtime)
//     auto start1 = high_resolution_clock::now();
//     int val1 = square(1000000);
//     auto end1 = high_resolution_clock::now();
//     cout << "Runtime constexpr square(1000000): " << val1 << endl;
//     cout << "Time taken: " 
//          << duration_cast<nanoseconds>(end1 - start1).count() 
//          << " ns" << endl;

//     // Regular function
//     auto start2 = high_resolution_clock::now();
//     int val2 = runtime_square(1000000);
//     auto end2 = high_resolution_clock::now();
//     cout << "Normal square(1000000): " << val2 << endl;
//     cout << "Time taken: " 
//          << duration_cast<nanoseconds>(end2 - start2).count() 
//          << " ns" << endl;

//     return 0;
// }
