// #include <iostream>

// using namespace std;
 
// class Employee {

// protected:

//     string name;

//     int id;

// public:

//     void setEmployee(string n, int i) {

//         name = n; id = i;

//     }

// };
 
// class Manager : public Employee {

// protected:

//     string department;

// public:

//     void setDepartment(string d) {

//         department = d;

//     }

// };
 
// class SeniorManager : public Manager {

// public:

//     void showDetails() {

//         cout << "Name: " << name << ", ID: " << id
// << ", Department: " << department << endl;

//     }

// };
 
// int main() {

//     SeniorManager sm;

//     sm.setEmployee("Ravi", 101);

//     sm.setDepartment("Finance");

//     sm.showDetails();

//     return 0;

// }

 #include <iostream>

using namespace std;
 
class Payment {

public:

    virtual void processPayment() = 0; // pure virtual

};
 
class CreditCard : public Payment {

public:

    void processPayment() override {

        cout << "Processing Credit Card Payment" << endl;

    }

};
 
class PayPal : public Payment {

public:

    void processPayment() override {

        cout << "Processing PayPal Payment" << endl;

    }

};
 
int main() {

    Payment* p1 = new CreditCard();

    Payment* p2 = new PayPal();
 
    p1->processPayment();

    p2->processPayment();
 
    delete p1;

    delete p2;

    return 0;

}
 
