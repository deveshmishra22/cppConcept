#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <fstream>
#include <exception>
#include <iomanip>

using namespace std;

// Custom Exception
class MachineException : public exception {
    string message;
public:
    explicit MachineException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Base Class: Machine (Abstract)
class Machine {
protected:
    string name;
    int id;
    double powerUsage;
    double efficiency;

public:
    Machine(string n, int i, double p, double e)
        : name(n), id(i), powerUsage(p), efficiency(e) {
        cout << "[Machine::ctor] Initializing base class with values.\n";
    }

    virtual ~Machine() {
        cout << "[Machine::dtor] Cleaning up Machine: " << name << endl;
    }

    virtual double calculateHealthScore() const = 0; // pure virtual
    virtual string typeName() const = 0;              // to identify derived type

    string getName() const { return name; }
    int getId() const { return id; }
    double getPowerUsage() const { return powerUsage; }
    double getEfficiency() const { return efficiency; }
};

// Derived Classes
class Assembler : public Machine {
public:
    Assembler(string n, int i, double p, double e)
        : Machine(n, i, p, e) {
        cout << "[Assembler::ctor] Assembler created. Calling Base class constructor.\n";
    }

    ~Assembler() override {
        cout << "[Assembler::dtor] Destroying Assembler.\n";
    }

    double calculateHealthScore() const override {
        cout << "[Assembler::calculateHealthScore].\n";
        double result =  efficiency * 0.9
        return result; 
    }

    string typeName() const override {
        return "Assembler";
    }
};

class Conveyor : public Machine {
public:
    Conveyor(string n, int i, double p, double e)
        : Machine(n, i, p, e) {
        cout << "[Conveyor::ctor] Conveyor created. Calling base class constructor.\n";
    }

    ~Conveyor() override {
        cout << "[Conveyor::dtor] Destroying Conveyor.\n";
    }

    double calculateHealthScore() const override {
        cout << "[Conveyor::calculateHealthScore] Calculate \n";
        double result = efficiency * 0.85;
        return result; 
    }

    string typeName() const override {
        return "Conveyor";
    }
};

class InspectionRobot : public Machine {
public:
    InspectionRobot(string n, int i, double p, double e)
        : Machine(n, i, p, e) {
        cout << "[InspectionRobot::ctor] InspectionRobot created. Calling Base constructor\n";
    }

    ~InspectionRobot() override {
        cout << "[InspectionRobot::dtor] Destroying InspectionRobot.\n";
    }

    double calculateHealthScore() const override {
        cout << "[InspectionRobot::calculateHealthScore] Calculate .\n";
        double result = efficiency * 0.95;
        return result; // placeholder
    }

    string typeName() const override {
        return "InspectionRobot";
    }
};

// Template Class: Monitor<T>
template <typename T>
class Monitor {
    vector<unique_ptr<T>> items;

public:
    Monitor() {
        cout << "[Monitor::ctor] Monitor created.\n";
    }

    ~Monitor() {
        cout << "[Monitor::dtor] Monitor destroyed.\n";
    }

    void addMachine(unique_ptr<T> machine) {
        cout << "[Monitor::addMachine] Adding machine to list.\n";
        items.push_back(move(machine));
    }

    template <typename U, typename... Args>
    void emplaceMachine(Args&&... args) {
        cout << "[Monitor::emplaceMachine] Creating and adding machine using template.\n";
        items.push_back(make_unique<U>(forward<Args>(args)...));
    }

    void displayAll() const {
        cout << "[Monitor::displayAll] Display all machine details here.\n";
    }

    void sortByHealth() {
        cout << "[Monitor::sortByHealth] Sort machines by health score using lambda + std::sort.\n";
    }

    void exportToFile(const string& filename) const {
        cout << "[Monitor::exportToFile] Export all data to file: " << filename << endl;
    }

    T* findByName(const string& searchName) const {
        cout << "[Monitor::findByName] Searching for machine: " << searchName << endl;
        return nullptr; // placeholder
    }

    size_t size() const {
        cout << "[Monitor::size] Return count of machines.\n";
        return items.size();
    }

    void clear() {
        cout << "[Monitor::clear] Remove all machines.\n";
        items.clear();
    }
};

// MAIN
int main() {
    cout << "Smart Manufacturing Dashboard\n\n";

    try {
        // Step 1: Create Monitor for Machines
        Monitor<Machine> monitor;
        cout << "Created Monitor for Machine objects.\n";

        // Step 2: Add few derived machine types
        monitor.emplaceMachine<Assembler>("Assembler", 101, 120.0, 95.0);
        monitor.emplaceMachine<Conveyor>("Conveyor", 102, 140.0, 88.5);
        monitor.emplaceMachine<InspectionRobot>("InspectionRobot", 103, 160.0, 99.0);

        // Step 3: Display all
        monitor.displayAll();

        // Step 4: Sort by Health
        monitor.sortByHealth();

        // Step 5: Export to file
        monitor.exportToFile("MachineReport.txt");

        // Step 6: Search for a specific machine
        monitor.findByName("Assembler");

        cout << "\nProgram flow completed (no logic yet, just skeleton).\n";
    }
    catch (const MachineException& ex) {
        cerr << "[MachineException] " << ex.what() << endl;
    }
    catch (const exception& ex) {
        cerr << "[std::exception] " << ex.what() << endl;
    }

    cout << "\nEnd of Prototype Execution\n";
    return 0;
}
