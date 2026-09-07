#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <string>
#include <chrono>
using namespace std;

int main() {

    cout<< []() {return 5 + 10;}();
    cout << "==================== Example 1: Basic Lambda ====================" << endl;
    auto sayHello = []() {
        cout << "Hello, Lambda World!" << endl;
    };
    sayHello();

    cout << "\n==================== Example 2: Parameters & Return ====================" << endl;
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "Sum = " << add(10, 20) << endl;

    cout << "\n==================== Example 3: Capture by Value ====================" << endl;
    int x = 5, y = 10;
    auto show = [x, y]() {
        cout << "x = " << x << ", y = " << y << endl;
    };
    show();



    cout << "\n==================== Example 4: Capture by Reference ====================" << endl;
    int counter = 0;
    auto increment = [&counter]() {
        counter++;
        cout << "Counter: " << counter << endl;
    };
    increment();
    increment();
    increment();
    cout << "Final counter = " << counter << endl;

    cout << "\n==================== Example 5: Capture All ====================" << endl;
    int a = 2, b = 3;
    auto addAll = [=]() { cout << "Sum (by value) = " << a + b << endl; };
    addAll();

    auto modifyAll = [&]() {
        a++;
        b++;
        cout << "Modified (by reference): a=" << a << ", b=" << b << endl;
    };
    modifyAll();
// 
    cout << "\n==================== Example 6: Using Lambda with STL sort ====================" << endl;
    vector<int> nums = {5, 2, 9, 1, 7};
    cout << "Before sort: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b; // descending order
    });

    cout << "After sort: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    cout << "\n==================== Example 7: Using Lambda with find_if ====================" << endl;
    vector<int> values = {10, 25, 30, 45, 50};
    auto it = find_if(values.begin(), values.end(), [](int n) { return n > 30; });
    if (it != values.end())
        cout << "Found value > 30: " << *it << endl;

    cout << "\n==================== Example 8: Lambda in Thread ====================" << endl;
    thread t([] {
        cout << "Running in a thread using lambda!" << endl;
    });
    t.join();

    cout << "\n==================== Example 9: Generic Lambda (C++14+) ====================" << endl;
    auto genericAdd = [](auto x, auto y) {
        return x + y;
    };

    cout << "add(int,int) = " << genericAdd(2, 3) << endl;
    cout << "add(double,double) = " << genericAdd(2.5, 3.7) << endl;
    cout << "add(string,string) = " << genericAdd(string("Hi "), string("there!")) << endl;

    cout << "\n==================== Example 10: Capture Behavior with Loop ====================" << endl;
    vector<int> data = {1, 2, 3, 4, 5};
    int total = 0;
    for_each(data.begin(), data.end(), [&](int n) { total += n; });
    cout << "Total using lambda = " << total << endl;

    // cout << "\n==================== Example 11: Lambda with Delay (Thread + chrono) ====================" << endl;
    // thread delayed([] {
    //     for (int i = 0; i < 5; ++i) {
    //         cout << "Processing " << i + 1 << "/5..." << endl;
    //         this_thread::sleep_for(chrono::milliseconds(500));
    //     }
    //     cout << "Done processing." << endl;
    // });
    // delayed.join();

    // cout << "\n==================== Example 12: Custom Sort by String Length ====================" << endl;
    // vector<string> names = {"Radar", "Sensor", "AI", "Telemetry", "BEL"};
    // sort(names.begin(), names.end(), [](const string& s1, const string& s2) {
    //     return s1.size() > s2.size(); // descending order by length
    // });

    // for (auto& n : names)
    //     cout << n << " ";
    // cout << endl;

    cout << "\n==================== End of Demonstration ====================" << endl;
    return 0;
}
