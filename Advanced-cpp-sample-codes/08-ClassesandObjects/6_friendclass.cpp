#include<iostream>  
using namespace std;
class B; // Forward declaration of class B

class A
{
private:
    int n1;
    friend void add(A , B); // Declaration of friend class
};
class B
{
private:
    int n2;
    friend void add(A , B); // Declaration of friend class
};

void add(A obj1, B obj2)
{
    obj1.n1 = 10;
    obj2.n2 = 20;
    cout << "Sum: " << obj1.n1 + obj2.n2 << endl;
    // Purpose of friend class is to allow access to private members of another class.
    // It is useful when two or more classes need to work closely together and share data.
    // Analogy: Think of a friend class like a trusted friend who is allowed to enter your house (class) and see things that others cannot (private members).
}


class Engine
{
private:
    int horsepower;
    friend class Car; // Declare Car as a friend class
};

class Car
{
private:
    string model;
public:
    Car(string m, int hp) : model(m) {
        Engine e;
        e.horsepower = hp; // Accessing private member of Engine
        cout << "Car Model: " << model << ", Horsepower: " << e.horsepower << endl;
    }

};

class BankAccount {
private:
    double balance;
    string name;
    friend class BankManager; // Declare BankManager as a friend class

public:
    BankAccount(string n, double b)
    {
        name = n;
        balance = b;
        cout << "Account created for " << name << " with balance " << balance << endl;
    }  
};

class BankManager {
public:
    void setBalance(BankAccount &account, double amount) {
        account.balance = amount; // Accessing private member of BankAccount
    }

    double getBalance(BankAccount &account) {
        return account.balance; // Accessing private member of BankAccount
    }
};

int main()
{
    BankAccount account("John Doe", 500.0);
    BankManager manager;
    manager.setBalance(account, 1000.0);
    double accbal = manager.getBalance(account);
    cout << "Updated Balance: " << accbal << endl;
    //Car car("Toyota", 150);
    // A a;
    // B b;
    // add(a, b); // Call friend function
    return 0;
}