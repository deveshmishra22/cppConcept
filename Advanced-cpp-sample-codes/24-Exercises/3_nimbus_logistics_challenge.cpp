// CHALLENGE MODE — Nimbus Logistics: Generic Order Fulfillment Engine
//
// Brings together Modules 1-6:
//   Module 1: structs, functions, const-reference parameters
//   Module 2: abstract base class, virtual dispatch
//   Module 3: function overloading, operator overloading
//   Module 4: custom exceptions, try/catch
//   Module 5: class templates
//   Module 6: STL containers + algorithms (vector, find_if, range-based for)
//
// This file has 5 features. TWO are already implemented for you (Feature 1
// and Feature 2) -- read them carefully, they are the pattern the other
// three follow. THREE are your job (Feature 3, 4, 5), marked with TODO.
//
// The file compiles and runs as-is. Every unfinished feature currently
// throws a clear "[TODO] ..." message instead of doing real work, so you can
// build and run after every single change and watch one message at a time
// turn into real output.
//
// Build: g++ -std=c++17 3_nimbus_logistics_challenge.cpp -o nimbus_challenge
// Run:   ./nimbus_challenge

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
using namespace std;

// ---------- GIVEN: Module 1 — a plain data record ----------
struct InventoryItem {
    int id;
    string name;
    int stockQty;
    double unitPrice;
};

// ---------- Feature 1 (GIVEN): a generic Repository<T> ----------
// One container class, reusable for InventoryItem today and for any other
// type tomorrow, as long as that type has an `id` member. This is exactly
// the "write once, let the compiler specialise it" idea from Module 5,
// combined with the STL's find_if algorithm from Module 6.
template<typename T>
class Repository {
    vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }

    T* findById(int id) {
        auto it = find_if(items.begin(), items.end(),
                           [id](const T& x) { return x.id == id; });
        return (it != items.end()) ? &(*it) : nullptr;
    }

    const vector<T>& all() const { return items; }
};

// ---------- GIVEN: Module 4 — custom exception types ----------
class InvalidQuantityException : public runtime_error {
public: explicit InvalidQuantityException(const string& msg) : runtime_error(msg) {}
};
class OutOfStockException : public runtime_error {
public: explicit OutOfStockException(const string& msg) : runtime_error(msg) {}
};

// ---------- GIVEN: Module 2 abstract base + Module 3 operator<< ----------
// Every kind of order must answer two questions on its own terms: how much
// does shipping cost, and how should it describe itself? processOrder-style
// code (here, placeOrder) never needs to know which concrete order it holds.
class Order {
protected:
    int itemId;
    int quantity;
public:
    Order(int id, int qty) : itemId(id), quantity(qty) {}
    virtual ~Order() = default;

    int getItemId() const { return itemId; }
    int getQuantity() const { return quantity; }

    virtual double shippingCost() const = 0;
    virtual string describe() const = 0;

    // Operator overloading (Module 3): printing an Order always calls the
    // correct derived describe() through virtual dispatch.
    friend ostream& operator<<(ostream& os, const Order& o) {
        os << o.describe();
        return os;
    }
};

// ---------- Feature 2 (GIVEN): the simplest concrete Order ----------
class StandardOrder : public Order {
public:
    StandardOrder(int id, int qty) : Order(id, qty) {}
    double shippingCost() const override { return 50.0; }
    string describe() const override { return "Standard Order"; }
};

// ---------- Feature 3 (YOUR TASK): ExpressOrder ----------
// An express order adds a flat premium over standard shipping, and an extra
// charge if the customer also asked for same-day delivery.
//
//   - shippingCost():
//       return 150.0 if sameDayRequested is false
//       return 250.0 if sameDayRequested is true
//   - describe():
//       return "Express Order (same-day)"        if sameDayRequested is true
//       return "Express Order (standard speed)"   if sameDayRequested is false
//
// Nothing else in the program needs to change -- operator<< and placeOrder
// already work with ANY Order, including this one, through virtual dispatch.
class ExpressOrder : public Order {
    bool sameDayRequested;
public:
    ExpressOrder(int id, int qty, bool sameDay) : Order(id, qty), sameDayRequested(sameDay) {}

    double shippingCost() const override {
        throw logic_error("Feature 3 not implemented: ExpressOrder::shippingCost()");
    }
    string describe() const override {
        throw logic_error("Feature 3 not implemented: ExpressOrder::describe()");
    }
};

// ---------- Feature 4 (YOUR TASK): overloaded applyDiscount ----------
// Two different ways to discount a price, distinguished purely by the type
// of the second argument -- this is compile-time overload resolution
// (Module 3), the same idea as calculateCharge() in the earlier capstones.
//
//   applyDiscount(double amount, double percentOff)
//       reduce amount by percentOff percent.
//       example: applyDiscount(1000.0, 10.0) -> 900.0
//
//   applyDiscount(double amount, int flatOff)
//       reduce amount by a flat rupee value, but never return a value below 0.
//       example: applyDiscount(1000.0, 200) -> 800.0
double applyDiscount(double amount, double percentOff) {
    throw logic_error("Feature 4 not implemented: applyDiscount(double, double)");
}
double applyDiscount(double amount, int flatOff) {
    throw logic_error("Feature 4 not implemented: applyDiscount(double, int)");
}

// ---------- Feature 5 (YOUR TASK): stock validation ----------
// Decide whether an order is even allowed to proceed, BEFORE any stock is
// touched.
//
//   - if requestedQty <= 0, throw InvalidQuantityException with a message
//     of your choice (check this condition FIRST).
//   - else if requestedQty > availableStock, throw OutOfStockException with
//     a message of your choice.
//   - otherwise, return normally -- the order is allowed to proceed.
void validateOrderQuantity(int requestedQty, int availableStock) {
    throw logic_error("Feature 5 not implemented: validateOrderQuantity()");
}

// ---------- GIVEN: wiring it all together ----------
void placeOrder(Repository<InventoryItem>& repo, Order& order) {
    InventoryItem* item = repo.findById(order.getItemId());
    if (!item) { cout << "  Item not found.\n"; return; }
    item->stockQty -= order.getQuantity();
    double subtotal = item->unitPrice * order.getQuantity();
    double total = subtotal + order.shippingCost();
    cout << "  " << order << " | Item: " << item->name << " | Total: Rs " << total << "\n";
}

int main() {
    Repository<InventoryItem> warehouse;
    warehouse.add({1, "Wireless Mouse", 10, 799.0});
    warehouse.add({2, "Mechanical Keyboard", 3, 4499.0});
    warehouse.add({3, "USB-C Hub", 0, 1299.0}); // deliberately out of stock

    cout << "NIMBUS LOGISTICS - ORDER FULFILLMENT\n---\n";

    cout << "\n[Feature 2 - given] Standard order:\n";
    StandardOrder mouseOrder(1, 2);
    try { placeOrder(warehouse, mouseOrder); }
    catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }

    cout << "\n[Feature 3 - your task] Express orders:\n";
    ExpressOrder keyboardOrder(2, 1, true);
    ExpressOrder keyboardOrderNoRush(2, 1, false);
    try { placeOrder(warehouse, keyboardOrder); }
    catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }
    try { placeOrder(warehouse, keyboardOrderNoRush); }
    catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }

    cout << "\n[Feature 4 - your task] Discount overloads:\n";
    try {
        double percentResult = applyDiscount(1000.0, 10.0);
        cout << "  Rs 1000 with 10% off    -> Rs " << percentResult << "\n";
    } catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }
    try {
        double flatResult = applyDiscount(1000.0, 200);
        cout << "  Rs 1000 with Rs 200 off -> Rs " << flatResult << "\n";
    } catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }

    cout << "\n[Feature 5 - your task] Stock validation exceptions:\n";
    try {
        StandardOrder hubOrder(3, 1); // item 3 has 0 stock
        validateOrderQuantity(hubOrder.getQuantity(), 0);
        placeOrder(warehouse, hubOrder);
        cout << "  Unexpected: order should have failed (no exception thrown)\n";
    } catch (const OutOfStockException& e) { cout << "  Caught OutOfStockException: " << e.what() << "\n"; }
      catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }

    try {
        StandardOrder badOrder(1, 0); // invalid quantity
        validateOrderQuantity(badOrder.getQuantity(), 10);
        placeOrder(warehouse, badOrder);
        cout << "  Unexpected: order should have failed (no exception thrown)\n";
    } catch (const InvalidQuantityException& e) { cout << "  Caught InvalidQuantityException: " << e.what() << "\n"; }
      catch (const logic_error& e) { cout << "  [TODO] " << e.what() << "\n"; }

    cout << "\nFinal warehouse state:\n";
    for (const auto& item : warehouse.all()) {
        cout << "  #" << item.id << " " << item.name << " -> stock left: " << item.stockQty << "\n";
    }

    return 0;
}
