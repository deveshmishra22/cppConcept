// Capstone exercise sample -- combines inheritance, abstract classes, virtual
// dispatch, function overloading and operator overloading in one program.
// See capstone-work1.html for the full brief.
//
// Use case: a smart office building that has to grant or deny badge access to
// employees, contractors and visitors -- each under a different rule -- without
// giving each type its own reporting logic.
#include <iostream>
#include <string>
using namespace std;

// Every person who badges in is a Person first (inheritance).
class Person {
protected:
    string name;
    int id;

public:
    Person(const string& n, int i) : name(n), id(i) {}
    virtual ~Person() = default;

    // Pure virtual functions: every concrete role must define its own rule (abstract class).
    virtual bool canAccess() const = 0;
    virtual string statusLine() const = 0;

    virtual void display() const {
        cout << name << " | ID: " << id << " | " << statusLine();
    }

    // Operator overloading: two people share a badge record if their IDs match,
    // regardless of which derived type either one is.
    friend bool operator==(const Person& a, const Person& b) {
        return a.id == b.id;
    }
};

// One function, every role: virtual dispatch decides the rest at runtime.
void processAccess(const Person& person) {
    person.display();
    cout << " -> " << (person.canAccess() ? "GRANTED" : "DENIED") << "\n";
}

class Employee : public Person {
public:
    Employee(const string& n, int i) : Person(n, i) {}
    bool canAccess() const override { return true; }
    string statusLine() const override { return "Employee"; }
};

class Contractor : public Person {
    bool contractActive;
public:
    Contractor(const string& n, int i, bool active)
        : Person(n, i), contractActive(active) {}
    bool canAccess() const override { return contractActive; }
    string statusLine() const override {
        return contractActive ? "Contractor (active)" : "Contractor (expired)";
    }
};

class Visitor : public Person {
    bool approved;
public:
    Visitor(const string& n, int i, bool appr)
        : Person(n, i), approved(appr) {}
    bool canAccess() const override { return approved; }
    string statusLine() const override {
        return approved ? "Visitor (approved)" : "Visitor (not approved)";
    }
};

// Function overloading: two ways to price a desk booking.
double calculateCharge(int wholeHours) {
    return wholeHours * 50.0;
}
double calculateCharge(double hours, double ratePerHour) {
    return hours * ratePerHour;
}

int main() {
    Employee rahul("Rahul", 101);
    Contractor priya("Priya", 202, true);
    Contractor amit("Amit", 203, false);
    Visitor sarah("Sarah", 301, true);
    Visitor john("John", 302, false);

    cout << "SMART OFFICE ACCESS REPORT\n---\n";
    processAccess(rahul);
    processAccess(priya);
    processAccess(amit);
    processAccess(sarah);
    processAccess(john);

    cout << "\nDesk booking, 4 whole hours: Rs " << calculateCharge(4) << "\n";
    cout << "Desk booking, 2.5 hours @ Rs 60/hr: Rs " << calculateCharge(2.5, 60.0) << "\n";

    Visitor sarahAgain("Sarah Verma", 301, true);
    cout << "\nSame badge ID? sarah == sarahAgain -> " << (sarah == sarahAgain ? "true" : "false") << "\n";
    cout << "Same badge ID? priya == amit -> " << (priya == amit ? "true" : "false") << "\n";
}
