// Module 7: Smart Pointers — one-file walkthrough
// Problem -> unique_ptr -> shared_ptr -> weak_ptr, each part builds on the last.
// Every claim below is proven by a cout line, not just described in a comment.
//
// Build: g++ -std=c++17 5_smart_pointers_demo.cpp -o smart_pointers_demo
// Run:   ./smart_pointers_demo

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
using namespace std;

// A "resource" whose constructor/destructor prints, so we can SEE exactly
// when it is created and destroyed -- that's the whole point of this file.
//
// A smart pointer is not the Resource itself. It is a small owner object that
// points to a Resource and automatically destroys it at the correct time.
struct Resource {
    string name;
    Resource(string n) : name(move(n)) { cout << "  [+] " << name << " constructed\n"; }
    ~Resource() { cout << "  [-] " << name << " destructed\n"; }
};

// ---------- PART 0: The problem smart pointers exist to solve ----------
void part0_the_problem() {
    cout << "==================== PART 0: The problem with raw pointers ====================\n";
    Resource* r = new Resource("RawResource");
    cout << "Using " << r->name << "...\n";
    delete r; // forget this ONE line, or throw before reaching it, and it leaks forever
    cout << "delete worked here because we remembered it. But every return, throw, or break\n";
    cout << "path in a real function must also remember it. Miss one path -> memory leak.\n";
    cout << "Delete it twice by accident -> crash (double free). Smart pointers remove both risks.\n";
}

// ---------- PART 1: unique_ptr — exactly one owner ----------
void part1_unique_ptr() {
    cout << "\n==================== PART 1: unique_ptr - exclusive ownership ====================\n";
    {
        unique_ptr<Resource> owner = make_unique<Resource>("Ticket");
        cout << "Holding: " << owner->name << endl;

        // unique_ptr<Resource> copy = owner;   // will NOT compile: copying is disabled on purpose
        unique_ptr<Resource> movedOwner = move(owner); // ownership TRANSFERS, nothing is duplicated
        cout << "owner is now empty? " << (owner == nullptr ? "yes" : "no") << endl;
        cout << "movedOwner holds: " << movedOwner->name << endl;
    } // movedOwner's scope ends here -> destructor fires automatically, no delete written anywhere
    cout << "Scope ended -- the [-] line above already fired by itself.\n";
    cout << "Real use: a single, non-shareable owner -- a file handle, a DB connection,\n";
    cout << "a PIMPL pointer. Only one part of the program is ever responsible for it.\n";

    cout << "\n-- unique_ptr also cleans up when an exception interrupts the scope --\n";
    try {
        unique_ptr<Resource> safeOwner = make_unique<Resource>("ExceptionSafeResource");
        cout << "Throwing before the closing brace...\n";
        throw runtime_error("simulated failure");
    } // safeOwner is destroyed here during stack unwinding
    catch (const runtime_error& error) {
        cout << "Caught: " << error.what() << "\n";
        cout << "The destructor line above proves cleanup happened before catch.\n";
    }
}

// ---------- PART 2: shared_ptr — many owners, reference counted ----------
void part2_shared_ptr() {
    cout << "\n==================== PART 2: shared_ptr - shared ownership, reference counted ====================\n";
    shared_ptr<Resource> a = make_shared<Resource>("CachedImage");
    cout << "use_count right after creation: " << a.use_count() << endl;
    {
        shared_ptr<Resource> b = a; // second owner of the SAME object, not a copy of the object
        cout << "use_count after b = a:         " << b.use_count() << endl;
        cout << "a and b point to the same object: " << (a.get() == b.get() ? "yes" : "no") << endl;
    } // b's scope ends -> count drops, but the object is NOT destroyed yet -- a still owns it
    cout << "use_count after b went out of scope: " << a.use_count() << endl;
    cout << "Real use: a texture shared by many sprites, a config object read by several\n";
    cout << "modules -- nobody knows who finishes last, so the LAST owner frees it, automatically.\n";
} // a's scope ends here -> use_count hits 0 -> destructor fires

// ---------- PART 3: weak_ptr — breaking a shared_ptr ownership cycle ----------
struct Child;
struct Parent {
    string name;
    shared_ptr<Child> child;
    Parent(string n) : name(move(n)) { cout << "  [+] Parent " << name << " constructed\n"; }
    ~Parent() { cout << "  [-] Parent " << name << " destructed\n"; }
};
struct Child {
    string name;
    shared_ptr<Parent> parent; // <-- THE BUG: keeps Parent's use_count above 0 forever
    Child(string n) : name(move(n)) { cout << "  [+] Child " << name << " constructed\n"; }
    ~Child() { cout << "  [-] Child " << name << " destructed\n"; }
};

struct ChildFixed;
struct ParentFixed {
    string name;
    shared_ptr<ChildFixed> child;
    ParentFixed(string n) : name(move(n)) { cout << "  [+] ParentFixed " << name << " constructed\n"; }
    ~ParentFixed() { cout << "  [-] ParentFixed " << name << " destructed\n"; }
};
struct ChildFixed {
    string name;
    weak_ptr<ParentFixed> parent; // <-- THE FIX: observes Parent, does NOT keep it alive
    ChildFixed(string n) : name(move(n)) { cout << "  [+] ChildFixed " << name << " constructed\n"; }
    ~ChildFixed() { cout << "  [-] ChildFixed " << name << " destructed\n"; }
};

void part3_weak_ptr() {
    cout << "\n==================== PART 3: weak_ptr - breaking a shared_ptr cycle ====================\n";

    cout << "-- 3a. The leak: Parent and Child both hold a shared_ptr to each other --\n";
    {
        // make_shared<Parent>("Alice") creates a Parent object and returns a
        // shared_ptr<Parent> that owns it. The explicit type makes that visible.
        shared_ptr<Parent> parent = make_shared<Parent>("Alice");
        shared_ptr<Child> child = make_shared<Child>("Bob");
        // There are now two local owners: parent owns Parent, child owns Child.
        parent->child  = child;
        // This copies the shared_ptr, so Child also owns Parent.
        child->parent  = parent; // each keeps the other's count >= 1, forever
        cout << "parent use_count: " << parent.use_count() << ", child use_count: " << child.use_count() << endl;
    } // scope ends here
    cout << "^ Look closely: NO '[-] Parent/Child destructed' lines appeared above. Both leaked.\n";

    cout << "\n-- 3b. The fix: Child holds a weak_ptr to Parent instead --\n";
    {
        // make_shared creates the object; the returned shared_ptr is the owner.
        shared_ptr<ParentFixed> parent = make_shared<ParentFixed>("Charlie");
        shared_ptr<ChildFixed> child = make_shared<ChildFixed>("Dave");
        parent->child  = child;
        // Parent owns Child through shared_ptr. Child only observes Parent
        // through weak_ptr, so this assignment does not increase parent's count.
        child->parent  = parent;
        cout << "parent use_count (weak_ptr doesn't add to it): " << parent.use_count() << endl;
        // A weak_ptr cannot use -> directly. lock() temporarily creates a
        // shared_ptr if the observed Parent is still alive.
        if (shared_ptr<ParentFixed> locked = child->parent.lock()) {
            cout << "child can still reach parent right now: " << locked->name << endl;
        }
    } // scope ends here
    cout << "^ This time both '[-] ... destructed' lines appeared. weak_ptr broke the cycle.\n";
    cout << "Real use: parent/child trees, an Observer watching a Subject, a cache entry --\n";
    cout << "anywhere you need to *look at* an object without forcing it to stay alive just because you're looking.\n";
}

int main() {
    part0_the_problem();
    part1_unique_ptr();
    part2_shared_ptr();
    part3_weak_ptr();
    return 0;
}
