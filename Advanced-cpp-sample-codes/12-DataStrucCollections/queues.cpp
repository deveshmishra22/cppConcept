#include <iostream>
#include <queue>
using namespace std;

int main()
{
    queue<int> numQueue;

    // Adding elements to the queue
    numQueue.push(10);
    numQueue.push(20);
    numQueue.push(30);
    cout << "Front element: " << numQueue.front() << endl; // Outputs 10
    cout << "Back element: " << numQueue.back() << endl;   // Outputs 30
    cout << "Queue size: " << numQueue.size() << endl;     // Outputs 3

    // Removing elements from the queue
    numQueue.pop();
    cout << "Front element after pop: " << numQueue.front() << endl; // Outputs 20
    cout << "Queue size after pop: " << numQueue.size() << endl;     // Outputs 2


    numQueue.emplace(40); // Adds 40 to the back of the queue
    cout << "Back element after emplace: " << numQueue.back() << endl;
    cout << "Final Queue size: " << numQueue.size() << endl;

    // Priority Queue Example
    // What it is? 
    // A priority queue is a special type of queue where elements are
    // ordered based on their priority rather than their insertion order.
    // How it works?
    // In a priority queue, each element has a priority associated with it.
    // Elements with higher priority are served before elements with lower priority.
    priority_queue<int> maxHeap;
    maxHeap.push(10);
    maxHeap.push(5);
    maxHeap.push(20);
    maxHeap.push(15);

    // Here we are using a max-heap priority queue,
    // which means the largest element has the highest priority.    

    cout << "Max element: " << maxHeap.top() << endl; // Outputs 20
    maxHeap.pop();
    cout << "Max element after pop: " << maxHeap.top() << endl; // Outputs 15


    // less than operator is used to create max-heap (default behavior)
    priority_queue<int, vector<int>, less<int>> maxHeap2;
    maxHeap2.push(10);
    maxHeap2.push(5);
    maxHeap2.push(20);
    maxHeap2.push(15);
    cout << "Max element from maxHeap2: " << maxHeap2.top() << endl; // Outputs 20
    maxHeap2.pop();
    cout << "Max element after pop from maxHeap2: " << maxHeap2.top() << endl; // Outputs 15

    // Example 2: Min-Heap Priority Queue
    // By default, C++ priority_queue is a max-heap.
    // To create a min-heap, we can use greater<int> as the comparator.
    // This will ensure that the smallest element has the highest priority.
    // Use cases of priority queues:
    // 1. Task Scheduling: Managing tasks based on priority levels.
    // 2. Dijkstra's Algorithm: Finding the shortest path in graphs.
    // 3. Event Simulation: Processing events in order of occurrence time.
    priority_queue<int, vector<int>, greater<int>> minHeap;
    // Explanation:
    // Here, we create a min-heap priority queue by specifying greater<int> as the comparator.  
    // This changes the default behavior of the priority queue to prioritize smaller elements.
    // How it works?
    // By using greater<int>, the priority queue organizes elements such that
    // the smallest element is always at the top, allowing for efficient retrieval and removal.
    // Can we create a max-heap using less than operator?
    // Yes, by default, priority_queue uses less<int> which creates a max-heap
    // Use cases of custom comparators in priority queues:
    // 1. Custom Ordering: To define specific ordering criteria for elements.
    // 2. Complex Data Types: When storing user-defined types, custom comparators
    //    can define how to compare those types.

    minHeap.push(10);
    minHeap.push(5);
    minHeap.push(20);
    minHeap.push(15);
    cout << "Min element: " << minHeap.top() << endl; // Outputs 5
    minHeap.pop();
    cout << "Min element after pop: " << minHeap.top() << endl; // Outputs 10
    return 0;
}