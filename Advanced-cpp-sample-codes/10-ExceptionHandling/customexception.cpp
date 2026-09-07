#include <iostream>
#include <exception>

using namespace std;
 
// Custom exception class for insufficient balance

class InsufficientFundsException : public exception {

    string message;

public:

    InsufficientFundsException(double balance, double withdrawAmount) {

        message = "Error: Insufficient funds! Balance: " + 

                  to_string(balance) + ", Tried to withdraw: " + 

                  to_string(withdrawAmount);

    }
 
    const char* what() const noexcept override {

        return message.c_str();

    }

};
 
void withdraw(double amount, double balance) {

    if (amount > balance)

        throw InsufficientFundsException(balance, amount);

    cout << "Withdrawal successful. New balance: " << (balance - amount) << endl;

}
 
int main() {

    try {
                double balance = 5000.0; // Example balance

                cout << "Current balance: " << balance << endl;
        cout << "Enter amount to withdraw: ";
        double amount;
        cin >> amount;

        withdraw(amount, balance); // more than balance

    }

    catch (const InsufficientFundsException &e) {

        cout << e.what() << endl;

    }
 
    cout << "Transaction complete." << endl;

}
 