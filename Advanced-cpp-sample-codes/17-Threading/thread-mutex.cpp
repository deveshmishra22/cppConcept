#include<iostream> 
#include<thread>
#include<mutex>
using namespace std;

mutex mtx; // Mutex for synchronizing access to shared resource
int sharedCounter = 0; // Shared resource

void incrementCounter(int id, int increments) {
    for (int i = 0; i < increments; ++i) {
        // Lock the mutex before accessing the shared resource
        mtx.lock();
        ++sharedCounter; // Critical section
        mtx.unlock(); // Unlock the mutex after accessing the shared resource

        // What does it mean by critical section here?
        // A critical section is a part of the code that accesses a shared resource (like shared
        // variables, data structures, etc.) and must not be concurrently accessed by more than one thread.
        // In this example, the increment operation on sharedCounter is a critical section because
        // if multiple threads try to increment it simultaneously without synchronization,
        // it can lead to race conditions and inconsistent results.
        // To prevent this, we use a mutex to ensure that only one thread can access the critical
        // section at a time.
    }
    cout << "Thread " << id << " finished." << endl;
}

void incrementCounterwithLockGuard(int id, int increments) {
    for (int i = 0; i < increments; ++i) {
        // Using lock_guard to automatically manage mutex locking and unlocking
        lock_guard<mutex> lock(mtx);
        ++sharedCounter; // Critical section
    }
    cout << "Thread " << id << " finished." << endl;
}

int main() {
    const int numThreads = 5;
    const int incrementsPerThread = 100000;

    thread threads[numThreads];

    // Create multiple threads to increment the shared counter
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = thread(incrementCounter, i + 1, incrementsPerThread);
    }

    // Join all threads to the main thread
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    cout << "Final value of sharedCounter: " << sharedCounter << endl;

    // Reset sharedCounter for lock_guard demonstration
    sharedCounter = 0;
    cout << "\nDemonstrating with lock_guard:\n";
    // Create multiple threads to increment the shared counter using lock_guard
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = thread(incrementCounterwithLockGuard, i + 1, incrementsPerThread);
    }
    // Join all threads to the main thread
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }
    cout << "Final value of sharedCounter with lock_guard: " << sharedCounter << endl;
    return 0;
}

// Use case:
// Mutexes are commonly used in multi-threaded applications to protect shared resources
// from concurrent access, ensuring data integrity and preventing race conditions
// Examples include:
// 1. Database access: When multiple threads need to read/write to a database,
//    a mutex can ensure that only one thread accesses the database at a time.
// 2. File operations: When multiple threads need to read/write to a file,
//    a mutex can prevent simultaneous access that could corrupt the file.
// 3. Shared data structures: When multiple threads need to modify shared data structures
//    like linked lists, hash tables, etc., a mutex can ensure that only one thread
//    modifies the structure at a time, preventing inconsistencies.