#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;
 
void readFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open())
        throw runtime_error("File not found: " + filename);
    string line;

    while (getline(file, line)) {
        cout << line << endl;
    }
}
 
int main() {
    try {
        readFile("C:\\Users\\raush\\Downloads\\CppTasks.txt");
    }

    catch (runtime_error &e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << "Program finished safely." << endl;

}

 
