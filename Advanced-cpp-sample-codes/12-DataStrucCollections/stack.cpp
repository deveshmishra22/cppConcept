#include<iostream>
#include<stack>
using namespace std;

int main()
{
    stack<int> numStack;

    // Adding elements to the stack
    numStack.push(10);
    numStack.push(20);
    numStack.push(30);
    cout << "Top element: " << numStack.top() << endl; // Outputs 30
    cout << "Stack size: " << numStack.size() << endl; // Outputs 3

    // Removing elements from the stack
    numStack.pop();
    cout << "Top element after pop: " << numStack.top() << endl; // Outputs 20
    cout << "Stack size after pop: " << numStack.size() << endl; // Outputs 2

    numStack.emplace(40); // Adds 40 to the top of the stack
    cout << "Top element after emplace: " << numStack.top() << endl; // Outputs 40
    cout << "Final Stack size: " << numStack.size() << endl; // Outputs 3

    // pop untill stack is empty
    while (!numStack.empty()) {
        cout << "Popping element: " << numStack.top() << endl;
        numStack.pop();
    }

    stack<int> a,b;
    a.push(1);
    a.push(2);
    a.push(3);
    b.push(40);
    b.push(50);
    b.push(60);
    a.swap(b); // Swaps contents of stack a and b
    cout << "Top element of stack a after swap: " << a.top() << endl; // Outputs 6
    cout << "Top element of stack b after swap: " << b.top() << endl; // Outputs 3

    return 0;
}