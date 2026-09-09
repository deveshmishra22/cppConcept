#include <iostream>

using namespace std;
 
struct Person

{

    string first_name;
    string last_name;
    int age;
    float salary;

    void introduce() {
        cout << "Hi, my name is " << first_name << " and I am " << age << " years old." << endl;
    }

};
 
int main()

{

    Person p1;
    cout << "Enter first name: ";
    cin >> p1.first_name;

    cout << "Enter last name: ";
    cin >> p1.last_name;

    cout << "Enter age: ";
    cin >> p1.age;

    cout << "Enter salary: ";
    cin >> p1.salary;
 
    cout << "\nDisplaying Information." << endl;
    cout << "First Name: " << p1.first_name << endl;
    cout << "Last Name: " << p1.last_name << endl;
    cout << "Age: " << p1.age << endl;
    cout << "Salary: " << p1.salary << endl;
 
    p1.introduce();
    return 0;

}
 