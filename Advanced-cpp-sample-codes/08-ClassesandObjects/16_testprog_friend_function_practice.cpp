// // // // // // // #include <iostream>
// // // // // // // #include <string>

// // // // // // // struct Learner {
// // // // // // //     std::string name;
// // // // // // //     int completedTopics;
// // // // // // //     int totalTopics;
// // // // // // // };

// // // // // // // void printProgress(const Learner& learner) {
// // // // // // //     std::cout << learner.name << ": "
// // // // // // //               << learner.completedTopics << "/"
// // // // // // //               << learner.totalTopics << std::endl;

// // // // // // //             //   learner.completedTopics++;
// // // // // // // }

// // // // // // // int main() {
// // // // // // //     Learner learner{"Asha", 3, 4};
// // // // // // //     printProgress(learner);
// // // // // // //     return 0;
// // // // // // // }
// // // // // // #include <iostream>
// // // // // // using namespace std;

// // // // // // class BankAccount {
// // // // // // public:
// // // // // //     string owner;
// // // // // //     double balance;
// // // // // //     // BankAccount(double openingBalance){
// // // // // //     //     balance = openingBalance;
// // // // // //     // }
// // // // // //     void deposit(double amount) {
// // // // // //         balance += amount;
// // // // // //         cout << owner << " deposited " << amount
// // // // // //              << ". New balance: " << balance << endl;
// // // // // //     }
// // // // // // };

// // // // // // int main() {
// // // // // //     BankAccount acc1;                     // stack allocation
// // // // // //     acc1.owner = "Ravi";
// // // // // //     acc1.balance = 1000;
// // // // // //     acc1.deposit(500);

// // // // // //     BankAccount* acc2 = new BankAccount(); // heap allocation
// // // // // //     acc2->owner =   "Anita";
// // // // // //     acc2->balance = 2000;
// // // // // //     acc2->deposit(750);

// // // // // //     delete acc2;   // we allocated with new, so we must delete it ourselves
// // // // // //     return 0;
// // // // // // }

// // // // // // #include <iostream>

// // // // // // class BankAccount {
// // // // // // private:
// // // // // //     double balance;

// // // // // // public:
// // // // // //     BankAccount(double openingBalance) : balance(openingBalance) {}
// // // // // //     void printBalance() const {
// // // // // //         std::cout << "Balance: " << balance << std::endl;
// // // // // //     }
// // // // // // };

// // // // // // int main() {
// // // // // //     BankAccount account(1000);
// // // // // //     account.printBalance();
// // // // // //     return 0;
// // // // // // }

// // // // // // #include <iostream>

// // // // // // class BankAccount {
// // // // // // private:
// // // // // //     static int openAccounts;
// // // // // // public:
// // // // // //     BankAccount() {
// // // // // //         std::cout << "Creating a new bank account." << std::endl;
// // // // // //         ++openAccounts; }
// // // // // //     static int accountCount() { return openAccounts; }
// // // // // // };

// // // // // // int BankAccount::openAccounts = 0;

// // // // // // // why it is allowed to be updated/accessed outside the class?
// // // // // // //Because it is a static member variable, which means it is shared among all instances of the
// // // // // // //class and can be accessed without creating an instance of the class.

// // // // // // // why is not allowed inside main() method?
// // // // // // // It is not allowed to access the private static member variable `openAccounts`
// // // // // // // directly inside the `main()` method because it is declared as `private` within the `BankAccount` class.
// // // // // // //Private members can only be accessed by member functions of the class or friends of the class, not from outside the class.

// // // // // // int main() {
// // // // // //     BankAccount first;
// // // // // //     BankAccount second;
// // // // // //     std::cout << "Open accounts: "
// // // // // //               << BankAccount::accountCount() << std::endl;
// // // // // //     return 0;
// // // // // // }

// // // // // #include <iostream>
// // // // // #include <string>

// // // // // class BankAccount
// // // // // {
// // // // // private:
// // // // //     std::string owner;
// // // // //     double balance;
// // // // //     static int openAccounts;

// // // // // public:
// // // // //     BankAccount(
// // // // //         const std::string &accountOwner, 
// // // // //         double openingBalance)
// // // // //         : owner(accountOwner), 
// // // // //         balance(openingBalance)
// // // // //     {
// // // // //         ++openAccounts;
// // // // //     }

// // // // //     void deposit(double amount)
// // // // //     {
// // // // //         if (amount > 0)
// // // // //             balance += amount;
// // // // //     }

// // // // //     bool withdraw(double amount)
// // // // //     {
// // // // //         if (amount <= 0 || amount > balance)
// // // // //             return false;
// // // // //         balance -= amount;
// // // // //         return true;
// // // // //     }

// // // // //     void printSummary() const
// // // // //     {
// // // // //         std::cout << owner << " has balance: " 
// // // // //         << balance << std::endl;
// // // // //     }

// // // // //     static int accountCount() 
// // // // //     { return openAccounts; }
// // // // // };

// // // // // int BankAccount::openAccounts = 0;

// // // // // int main()
// // // // // {
// // // // //     BankAccount ravi("Ravi", 1000);
// // // // //     BankAccount maya("Maya", 500);

// // // // //     ravi.deposit(250);
// // // // //     maya.withdraw(100);
// // // // //     ravi.printSummary();
// // // // //     maya.printSummary();
// // // // //     std::cout << "Open accounts: " << BankAccount::accountCount() << std::endl;
// // // // //     return 0;
// // // // // }
// // // // // // send to Email -> 
// // // // // // raushan.ranjan@koenig-solutions.com
// // // // // // Add transfers and account validation
// // // // // // Create your own program.cpp 
// // // // // // that extends the 
// // // // // //bank account program with:
// // // // // // a transferTo member function 
// // // // // // that moves money 
// // // // // //between two accounts;
// // // // // // a rejected transfer when the 
// // // // // // source has 
// // // // // // insufficient funds;
// // // // // // a clear transaction message and 
// // // // // // final summary 
// // // // // // for both customers.

// // // // // // Hints, not the answer

// // // // // // Use references for the destination account so 
// // // // // // the real object is changed.
// // // // // // Reuse withdraw and deposit instead of changing 
// // // // // // balance directly.
// // // // // // Return true only after the source withdrawal 
// // // // // // succeeds.


// // // // #include <iostream>
// // // // using namespace std;

// // // // class BankAccount {
// // // // private:
// // // //     string owner;
// // // //     double balance;

// // // // public:
// // // //     // Default constructor — every account starts safely at zero
// // // //     BankAccount() {
// // // //         owner = "Unnamed";
// // // //         balance = 0.0;
// // // //         cout << "Default constructor: new empty account created." << endl;
// // // //     }

// // // //     // Parameterized constructor — open an account with real details
// // // //     BankAccount(string ownerName, double openingBalance) {
// // // //         owner = ownerName;
// // // //         balance = openingBalance;
// // // //         cout << "Account opened for " << owner <<"." << endl;
// // // //     }

// // // //     // Copy constructor — e.g. issuing a joint/duplicate account record
// // // //     BankAccount(const BankAccount &source) {
// // // //         owner = source.owner;
// // // //         balance = source.balance;
// // // //         cout <<"Copy constructor: duplicated account for " << owner <<"." << endl;
// // // //     }

// // // //     void display() {
// // // //         cout << owner << "'s balance: " << balance << endl;
// // // //     }
// // // // };

// // // // int main() {
// // // //     BankAccount a1;                          // default constructor runs
// // // //     BankAccount a2("Meena", 5000);           // parameterized constructor runs
// // // //     BankAccount a3 = a2;                     // copy constructor runs

// // // //     a1.display();
// // // //     a2.display();
// // // //     a3.display();
// // // //     return 0;
// // // // }

// // // #include <iostream>
// // // using namespace std;

// // // class BankAccount {
// // // public:
// // //     string owner;

// // //     BankAccount(string ownerName) {
// // //         owner = ownerName;
// // //         cout <<"Account opened for " << owner << endl;
// // //     }

// // //     // Destructor: same name as the class, preceded by ~.
// // //     // Runs automatically — no manual call needed for stack objects.
// // //     ~BankAccount() {
// // //         cout <<"Account closed for " << owner <<" — audit log written." << endl;
// // //     }
// // // };

// // // int main() {
// // //     BankAccount a1("Ravi");                      // stack object
// // //     BankAccount* a2 = new BankAccount("Anita");   // heap object

// // //     // delete a2;   // explicitly triggers ~BankAccount() for the heap object
// // //     return 0;
// // //     // a1 goes out of scope here -> its destructor runs automatically
// // // }

// // #include <iostream>

// // class BankAccount {
    
// // public:
// //     static int openAccounts;
// //     BankAccount() { ++openAccounts; }
// //     static int accountCount() { return openAccounts; }
// // };



// // int main() {
// //     int BankAccount::openAccounts = 0;
// //     BankAccount first;
// //     BankAccount second;
// //     std::cout << "Open accounts: "
// //               << BankAccount::accountCount() << std::endl;
// //     return 0;
// // }

// #include <iostream>
// using namespace std;

// class BankAccount {
// private:
//     double balance;
//     string name;
//     friend class BankManager;   // grants BankManager access to our private data

// public:
//     BankAccount(string n, double b) {
//         name = n;
//         balance = b;
//         cout << "Account created for " << name << " with balance " << balance << endl;
//     }
// };

// class BankManager {
// public:
//     void setBalance(BankAccount &account, double amount) {
//         account.balance = amount;   // legal ONLY because of the friend declaration
//     }

//     double getBalance(BankAccount &account) {
//         return account.balance;     // same here
//     }
// };

// int main() {
//     BankAccount account("John Doe", 500.0);
//     BankManager manager;

//     manager.setBalance(account, 1000.0);
//     double accbal = manager.getBalance(account);

//     cout << "Updated Balance: " << accbal << endl;
//     return 0;
// }

#include <iostream>
using namespace std;

class Box {
private:
    double width;
public:
    Box(double w) : width(w) {}

    // Declares printWidth as a friend — defining it right here is
    // just a convenience; it is still NOT a member function.
    void printWidth(Box box)
    {
        cout << "Width of box: " << box.width << endl;
        // A friend function is like a trusted guest let into the house (class) —
        // it can see the private furniture, but it doesn't live there.
    }
    friend void printWidth(Box box)
    {
        cout << "Width of box: " << box.width << endl;
        // A friend function is like a trusted guest let into the house (class) —
        // it can see the private furniture, but it doesn't live there.
    }
};

int main() {
    Box box(10.0);
    printWidth(box);        // called like any ordinary function — no box.printWidth()
    box.printWidth(box);   // called like a member function, but it is NOT one
    return 0;
}