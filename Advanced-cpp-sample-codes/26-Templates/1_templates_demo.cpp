// Module 5: Templates & Generics — one-file walkthrough
// ("05-Templates: planned function/class templates and Stack<T> examples").
// Build: g++ -std=c++17 1_templates_demo.cpp -o templates_demo
// Run:   ./templates_demo

#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;

// ---------- PART 1: Why templates? ----------
// Without templates, one "maxOf" per type. Same logic, copy-pasted.
int maxOfInt(int a, int b) { return (a > b) ? a : b; }
double maxOfDouble(double a, double b) { return (a > b) ? a : b; }
// Fix a bug in this logic and you must remember to fix it in every copy.

// ---------- PART 2: Function template ----------
// One definition. The compiler generates a version per type at compile time
// (this is called "instantiation") — no runtime cost, full type safety.
template<typename T>
T maxOf(T a, T b) {
    return (a > b) ? a : b;
}

// ---------- PART 3: Class template ----------
// A generic "inventory stack" that can hold any item type, decided by
// whoever uses it (InventoryStack<string>, InventoryStack<int>, ...).
template<typename T>
class InventoryStack {
    vector<T> items;
public:
    void push(const T& item) { items.push_back(item); }
    void pop() {
        if (items.empty()) { cout << "Stack empty, nothing to pop.\n"; return; }
        items.pop_back();
    }
    T top() const { return items.back(); }
    bool empty() const { return items.empty(); }
    size_t size() const { return items.size(); }
};

int main() {
    cout << "==================== PART 1: Why templates ====================\n";
    cout << "maxOfInt(3, 7)       = " << maxOfInt(3, 7) << endl;
    cout << "maxOfDouble(2.5,1.1) = " << maxOfDouble(2.5, 1.1) << endl;
    cout << "Two near-identical functions already. Imagine also needing float, char, string...\n";

    cout << "\n==================== PART 2: Function template ====================\n";
    cout << "maxOf(3, 7)               -> T=int    -> " << maxOf(3, 7) << endl;
    cout << "maxOf(2.5, 1.1)           -> T=double -> " << maxOf(2.5, 1.1) << endl;
    cout << "maxOf(string,string)      -> T=string -> "
         << maxOf(string("apple"), string("banana")) << endl;
    cout << "One definition, three instantiations — the compiler wrote the other two for us.\n";

    cout << "\n==================== PART 3: Class template - Generic inventory stack ====================\n";
    InventoryStack<string> warehouse;
    warehouse.push("Laptop");
    warehouse.push("Monitor");
    warehouse.push("Keyboard");
    cout << "Warehouse has " << warehouse.size() << " items. Top item: " << warehouse.top() << endl;
    warehouse.pop();
    cout << "After popping, top item: " << warehouse.top() << ", size: " << warehouse.size() << endl;

    InventoryStack<int> serialNumbers;
    serialNumbers.push(1001);
    serialNumbers.push(1002);
    cout << "Same InventoryStack<T> class, now holding ints. Top serial: "
         << serialNumbers.top() << endl;

    cout << "\n==================== PART 4: STL containers ARE templates ====================\n";
    // std::stack<T> and std::vector<T> in the standard library are written
    // exactly the way we just wrote InventoryStack<T> above — a class
    // template parameterised on T.
    stack<string> stlStack;
    stlStack.push("Router");
    stlStack.push("Switch");
    cout << "std::stack<string> top: " << stlStack.top() << endl;
    stlStack.pop();
    cout << "After pop, std::stack<string> top: " << stlStack.top() << endl;
    cout << "Our InventoryStack<T> and std::stack<T> follow the same idea:\n";
    cout << "write the container logic once, let the compiler stamp out a version per type.\n";

    return 0;
}
