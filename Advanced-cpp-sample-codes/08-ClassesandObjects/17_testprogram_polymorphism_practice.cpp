#include<iostream>
using namespace std;

class Parent
{
    public:
        virtual void func1(){cout<<"Parent-Func1" << endl;}
};
class Child : public Parent
{
    public:
        void func1(){cout<<"Child-Func1" << endl;}
};

int main()
{
    // Child c;
    Parent* obj = new Child();
    obj->func1();

    return 0;
}
