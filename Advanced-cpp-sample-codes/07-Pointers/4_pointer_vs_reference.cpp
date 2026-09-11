// Pointer vs reference: choosing a parameter type in modern C++17.
// Compile: g++ -std=c++17 4_pointer_vs_reference.cpp -o pointer_vs_reference
#include <iostream>
#include <string>

using namespace std;

struct BankAccount {
    string owner;
    double balance;
};

// Use const T& when an object is required but should not be copied or changed.
void printAccount(const BankAccount& account) {
    cout << account.owner << ": balance = " << account.balance << '\n';
}

// Use T& when an object is required and the function must update that object.
// A reference cannot be null, so no null check is needed.
void deposit(BankAccount& account, double amount) {
    if (amount > 0) {
        account.balance += amount;
    }
}

// Use T* when an object is optional. nullptr means "no account was supplied".
// This pointer observes an existing account; it does not own or delete it.
bool withdraw(BankAccount* account, double amount) {
    if (account == nullptr) {
        cout << "Withdrawal skipped: no account selected.\n";
        return false;
    }

    if (amount <= 0 || amount > account->balance) {
        cout << "Withdrawal rejected for " << account->owner << ".\n";
        return false;
    }

    account->balance -= amount;
    return true;
}

// A pointer variable can be reassigned to point at another object.
// Account*& is a reference to the POINTER itself, allowing this function to
// change the caller's selected pointer. A normal BankAccount& cannot be rebound.
void selectAccount(BankAccount*& selected, BankAccount* next) {
    selected = next;
}

int main() {
    BankAccount alice{"Alice", 1000.0};
    BankAccount bob{"Bob", 500.0};

    cout << "Initial accounts\n";
    printAccount(alice);                 // required object, read only: const BankAccount&
    printAccount(bob);

    cout << "\nReference update\n";
    deposit(alice, 250.0);               // required object, modify it: BankAccount&
    printAccount(alice);

    cout << "\nPointer update\n";
    BankAccount* selected = &alice;      // pointer can hold an address or nullptr
    withdraw(selected, 100.0);           // optional object: BankAccount*
    printAccount(alice);

    withdraw(nullptr, 50.0);             // valid call; the function handles absence

    cout << "\nPointer reassignment\n";
    selectAccount(selected, &bob);       // selected now points to Bob
    withdraw(selected, 75.0);
    printAccount(bob);

    // Do NOT write: delete selected;
    // selected does not own Alice or Bob. They are stack objects and are cleaned
    // up automatically. Prefer unique_ptr/shared_ptr when ownership is required.
    return 0;
}

/*
Quick decision guide

1. Use const T& for a required input object: no copy, no modification.
2. Use T& for a required object that the function must modify.
3. Use T* for an optional object: nullptr has meaningful "not available" value.
4. Use T*& only when the function must change which object a caller's pointer
   refers to. This is less common than T&.
5. Do not use raw pointers to express ownership. Use unique_ptr or shared_ptr.

Limitations
- A reference must refer to a valid object when it is created; it cannot be null
  and cannot later refer to a different object.
- A raw pointer can be null, dangling, or incorrectly deleted. Always check it
  before dereferencing when null is possible, and make ownership explicit.
*/
