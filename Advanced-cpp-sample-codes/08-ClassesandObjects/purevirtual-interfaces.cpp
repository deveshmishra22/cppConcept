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
 