// Module 5: Templates & Generics - real-world order processing
// Build: g++ -std=c++17 2_real_world_template_demo.cpp -o order_demo
// Run:   ./order_demo
//
// This example models a small online store. The important idea is not the
// store itself; it is that the same generic classes solve several problems.
//
// Real-world template use cases include:
//   1. Containers: vector<T>, queue<T>, map<K, V>, and stack<T> store many types.
//   2. Repositories: one CRUD/search implementation can store any record type.
//   3. Algorithms: sort, find, transform, and accumulate work with many types.
//   4. Work queues: the same queue logic can process orders, emails, or reports.
//   5. Type safety: the compiler prevents putting the wrong kind of object into
//      a collection, without requiring duplicated classes for every type.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// A few domain types. They contain business data, not collection logic.
struct Product {
    int id;
    string name;
    double price;
};

struct Customer {
    int id;
    string name;
};

struct Order {
    int id;
    int customerId;
    double total;
};

// Generic repository: the same storage/search behavior works for every record
// type that has an integer "id" field.
//
// Problem solved: without a template, developers might write ProductRepository,
// CustomerRepository, and OrderRepository with almost identical vector code.
template<typename T>
class Repository {
    vector<T> records;

public:
    void add(const T& record) {
        records.push_back(record);
    }

    const T* findById(int id) const {
        for (const T& record : records) {
            if (record.id == id) {
                return &record;
            }
        }
        return nullptr;
    }

    size_t size() const {
        return records.size();
    }
};

// Generic FIFO queue for work items.
//
// Problem solved: an application may need queues for orders, emails, image
// jobs, or audit events. The queue mechanics should be written once.
template<typename T>
class WorkQueue {
    vector<T> items;

public:
    void add(const T& item) {
        items.push_back(item);
    }

    bool empty() const {
        return items.empty();
    }

    T next() {
        T item = items.front();
        items.erase(items.begin());
        return item;
    }
};

// Generic function template. It can print any value for which operator<< is
// supported. Here it is used for a common progress message.
template<typename T>
void showCount(const string& label, const T& collection) {
    cout << label << collection.size() << endl;
}

int main() {
    cout << "==================== ONLINE STORE ====================\n";

    // One Repository<T> class creates three different, strongly typed stores.
    // Each object owns separate data, even though the implementation is shared.
    Repository<Product> products;
    Repository<Customer> customers;
    Repository<Order> orders;

    products.add({101, "Laptop", 1200.00});
    products.add({102, "Keyboard", 75.50});

    customers.add({1, "Alice"});
    customers.add({2, "Bob"});

    orders.add({5001, 1, 1275.50});
    orders.add({5002, 2, 75.50});

    showCount("Products stored:  ", products);
    showCount("Customers stored: ", customers);
    showCount("Orders stored:    ", orders);

    // The compiler knows what type each repository returns.
    const Product* product = products.findById(101);
    const Customer* customer = customers.findById(1);
    const Order* order = orders.findById(5001);

    if (product && customer && order) {
        cout << "Order " << order->id << " belongs to " << customer->name << endl;
        cout << "Product found: " << product->name
             << " ($" << product->price << ")" << endl;
        cout << "Order total: $" << order->total << endl;
    }

    // The same queue template is reused for two unrelated operational jobs.
    WorkQueue<Order> orderFulfillment;
    WorkQueue<string> emailNotifications;

    orderFulfillment.add(*order);
    emailNotifications.add("Send confirmation to Alice");

    cout << "\n==================== PROCESSING WORK ====================\n";
    while (!orderFulfillment.empty()) {
        Order nextOrder = orderFulfillment.next();
        cout << "Fulfill order " << nextOrder.id
             << " for $" << nextOrder.total << endl;
    }

    while (!emailNotifications.empty()) {
        cout << emailNotifications.next() << endl;
    }

    cout << "\n==================== WHAT TEMPLATES SOLVED ====================\n";
    cout << "1. Repository logic was implemented once for Product, Customer, and Order.\n";
    cout << "2. Queue logic was implemented once for orders and email messages.\n";
    cout << "3. Each object remains type-safe: a Repository<Product> accepts products,\n";
    cout << "   and a WorkQueue<Order> accepts orders.\n";
    cout << "4. Adding a new record type usually requires only a new struct and\n";
    cout << "   Repository<NewType>, not another copy of the repository class.\n";

    return 0;
}
