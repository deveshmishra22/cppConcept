#include <iostream>
using namespace std;
 
class Student {
private:
    int rollNo;
    string name;
 
public:
    void setDetails(int rollNo, string name) {
        this->rollNo = rollNo;
        this->name = name;
    }
 
    void display() {
        cout << "Roll: " << rollNo << ", Name: " << name << endl;
    }

};
 
int main() {
    Student s1;  // Static allocation -> Stack memory
    Student* s2 = new Student(); // Dynamic allocation -> Heap memory

    s2->setDetails(102, "Anita");
    s2->display();

    s1.setDetails(101, "Ravi");
    s1.display();

    delete s2; // Free dynamically allocated memory -> Heap memory

    return 0;
}

 