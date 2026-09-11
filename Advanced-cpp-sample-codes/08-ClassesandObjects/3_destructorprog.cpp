#include<iostream>  
using namespace std;

class Demo{  
    public:  
    // Constructor
    Demo(){  
        cout<<"Constructor called!"<<endl;  
    }

    // Destructor 
    //why it is public? because when object goes out of scope destructor is called automatically
    // Destructor has same name as class name but is preceded by a tilde(~) symbol.
    // It does not take any parameters and does not return any value.
    // It is called automatically when the object goes out of scope or is explicitly deleted.
    // It is used to release resources that were acquired by the object during its lifetime, such as memory, file handles, network connections, etc. 
    ~Demo(){  
        cout<<"Destructor called!"<<endl;  
    }  
};

int main(){  
    Demo d1;
    Demo* d2 = new Demo(); //dynamic object creation
    delete d2; //explicitly calling destructor for dynamic object  
    return 0;  
}