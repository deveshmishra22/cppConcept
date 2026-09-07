#include <iostream>
using namespace std;
//using namespace MyNamespace; // importing our custom namespace
namespace FirstNamespace {
    void display() {
        std::cout << "Inside FirstNamespace" << std::endl;
    }
}

namespace SecondNamespace {
    class Test {
    public:
        void show() {
            std::cout << "Inside Test class of SecondNamespace" << std::endl;
        }
    };
    void display() {
        std::cout << "Inside SecondNamespace" << std::endl;
    }
}
int main() {
    SecondNamespace::Test obj;
    obj.show();  // Calls show from Test class
    FirstNamespace::display();  // Calls display from FirstNamespace
    SecondNamespace::display(); // Calls display from SecondNamespace
    return 0;
}

// can we use using namespace here?
// Yes, we can use 'using namespace' to avoid prefixing the namespace each time.
// How i can use this namespace in different files?
// To use namespaces across different files, you can define the namespace in a header file
// and include that header file in other source files where you want to use the namespace.
// Example:
// In MyNamespace.h
