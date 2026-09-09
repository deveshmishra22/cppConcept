//What is a deadlock?
//A deadlock is a situation in concurrent programming where two or more threads are unable to proceed because each is waiting for the other to release a resource.
//This results in a standstill where none of the threads can make progress, effectively halting the program's execution.

// How to avoid deadlocks in C++?
//1. Lock Ordering: Always acquire locks in a predefined global order. This prevents circular wait
//   conditions that lead to deadlocks.
//2. Try-Lock Mechanism: Use try_lock to attempt to acquire locks without blocking.
//   If a thread cannot acquire all the required locks, it can release any it has already acquired
//   and retry later.
//3. Lock Timeout: Implement timeouts when trying to acquire locks. If a thread cannot acquire a lock within a certain time frame,
//   it can release any locks it holds and retry later.
//4. Deadlock Detection: Periodically check for deadlocks in the system and take corrective actions,
//   such as terminating one of the threads involved in the deadlock.

// Deadlock prevention strategies:
// 1. Mutual Exclusion: Ensure that at least one resource is non-shareable.
// 2. Hold and Wait: Require threads to request all required resources at once, preventing them from holding one resource while waiting for another.
// 3. No Preemption: Allow the system to preempt resources from threads if necessary
// 4. Circular Wait: Impose a total ordering of all resource types and require that each thread requests resources in an increasing order of enumeration.
// 5. Timeout locks: Use timeouts when trying to acquire locks to avoid indefinite waiting.
// 6. Lock hierarchy: Establish a hierarchy for acquiring locks and ensure that threads acquire locks in a specific order based on this hierarchy.
// 7. Try-locks: Use try-lock mechanisms to attempt to acquire locks without blocking, allowing threads to back off and retry if they cannot acquire all required locks.
// 8. Avoid nested locks: Minimize the use of nested locks where possible, as they increase the risk of deadlocks.

#include <iostream>
#include <thread>
#include <mutex>
#include <future>
using namespace std;

mutex mtx1; // Mutex for first resource
mutex mtx2; // Mutex for second resource

void thread1Func() {
    cout << "Thread 1 attempting to lock mtx1 and mtx2" << endl;

    // Thread 1 locks mtx1 then mtx2
    mtx1.lock();
    this_thread::sleep_for(chrono::seconds(5)); // Simulate some work
    mtx2.lock();    
    //this_thread::sleep_for(chrono::seconds(5)); // Simulate some work


    cout << "Thread 1 acquired both locks." << endl;

    mtx2.unlock();
    mtx1.unlock();
    cout << "Thread 1 released both locks." << endl;
}

void thread2Func() {
    cout << "Thread 2 attempting to lock mtx2 and mtx1" << endl;

    // Thread 2 locks mtx2 then mtx1
    mtx2.lock();
    this_thread::sleep_for(chrono::seconds(5)); // Simulate some work
    mtx1.lock();
    //this_thread::sleep_for(chrono::seconds(5)); // Simulate some work

    cout << "Thread 2 acquired both locks." << endl;

    mtx1.unlock();
    mtx2.unlock();
    cout << "Thread 2 released both locks." << endl;
}

int longTask()
{
    this_thread::sleep_for(chrono::seconds(10));
    return 11;
}
int main() {

    // future<int> result = std::async(std::launch::async, longTask);
    // cout << "Doing something....... " << endl;
    // int value = result.get();
    // cout << "Value from long task: " << value << endl;

    thread t1(thread1Func);
    thread t2(thread2Func);

    t1.join();
    t2.join();

    cout << "Main thread finished execution." << endl;
    return 0;
}

// async and future explanation:
// In C++, std::async is a function template that runs a function asynchronously (potentially in a separate thread)
// and returns a std::future object that can be used to retrieve the result of the function once it has completed.
// The std::future object acts as a placeholder for the result of the asynchronous operation,
// allowing the main thread to continue executing while the asynchronous task is being performed.
// When you call std::async, you can specify a launch policy (e.g., std::launch::async) to control how the task is executed.
// The std::future object provides a get() method that blocks the calling thread until the result is available,
// at which point it returns the result of the asynchronous function.