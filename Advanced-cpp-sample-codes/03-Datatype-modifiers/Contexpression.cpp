#include <iostream>
using namespace std;

constexpr int square(int x) {
    return x * x;
}

//what is constexpr function in c++?
// A constexpr function is a function that can be evaluated at compile time. It allows the compiler to perform computations during compilation, resulting in faster execution at runtime. The function must meet certain criteria, such as having a return type that is a literal type and containing only a single return statement.

int main() {
    constexpr int result = square(5);
    cout << "The square of 5 is: " << result << endl; // This will be evaluated at compile time, and the value of result 
    // will be known at compile time.
    cout<< "Calling square func temp: " << endl;
    cout<< "The square of 10 is: " << square(10) << endl; // This will be evaluated at compile time as well, 
    // since the argument is a constant expression.
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
