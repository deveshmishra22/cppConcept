#include<iostream> 
#include<thread>
#include<mutex>
using namespace std;

mutex mtx; // Mutex for synchronizing access to shared resource
int sharedCounter = 0; // Shared resource

// --- Added explanation: incrementCounter(id, increments) ------------------
// Generic syntax:  incrementCounter( int id, int increments )
//   id         - parameter #1: an identifying number for whichever thread is
//                running this function, used only for the printed message
//                below (it has no effect on sharedCounter).
//   increments - parameter #2: how many times this call should add 1 to the
//                shared counter, i.e. how many loop iterations to run.
void incrementCounter(int id, int increments) {
    for (int i = 0; i < increments; ++i) {
        // Lock the mutex before accessing the shared resource
        // --- Added explanation: mtx.lock() -----------------------------------
        // Generic syntax:  mtx.lock()
        //   (no parameters) - blocks the calling thread until it is the only
        //   thread holding `mtx`. If another thread already holds the lock,
        //   this call waits here until that thread calls mtx.unlock().
        mtx.lock();
        ++sharedCounter; // Critical section
        // --- Added explanation: mtx.unlock() ---------------------------------
        // Generic syntax:  mtx.unlock()
        //   (no parameters) - releases the lock this thread is holding on
        //   `mtx`, allowing exactly one other waiting thread (if any) to
        //   acquire it next.
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

// --- Added explanation: incrementCounterwithLockGuard(id, increments) -----
// Generic syntax:  incrementCounterwithLockGuard( int id, int increments )
//   id         - parameter #1: identifying number for the printed message.
//   increments - parameter #2: how many times to add 1 to sharedCounter.
void incrementCounterwithLockGuard(int id, int increments) {
    for (int i = 0; i < increments; ++i) {
        // Using lock_guard to automatically manage mutex locking and unlocking
        // --- Added explanation: lock_guard<mutex> lock(mtx) -------------------
        // Generic syntax:  std::lock_guard<MutexType> variableName( mutexObj )
        //   MutexType - template parameter: the type of mutex being wrapped
        //               (mutex, here).
        //   mutexObj  - constructor parameter #1: the specific mutex instance
        //               (mtx) to lock RIGHT NOW, at the moment `lock` is
        //               constructed. There is no unlock() call anywhere
        //               because lock_guard's destructor calls mtx.unlock()
        //               automatically the instant `lock` goes out of scope
        //               (end of this loop iteration) - even if an exception
        //               were thrown, so the mutex can never be left locked.
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
    // --- Added explanation: thread(incrementCounter, i + 1, incrementsPerThread)
    // Generic syntax:  std::thread( callableFunction, arg1, arg2, ... )
    //   callableFunction - parameter #1: the function the new thread runs
    //                      (incrementCounter, here).
    //   arg1             - parameter #2: forwarded into incrementCounter's
    //                      first parameter `id` (i + 1, so threads are
    //                      numbered 1..numThreads instead of 0-based).
    //   arg2             - parameter #3: forwarded into incrementCounter's
    //                      second parameter `increments`
    //                      (incrementsPerThread, i.e. 100000).
    // Each assignment below starts a NEW thread immediately running
    // incrementCounter(i + 1, incrementsPerThread) concurrently with the
    // other threads already created in this loop.
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = thread(incrementCounter, i + 1, incrementsPerThread);
    }

    // Join all threads to the main thread
    // --- Added explanation: threads[i].join() --------------------------------
    // Generic syntax:  threads[i].join()
    //   (no parameters) - blocks the main thread until thread #i has fully
    //   finished. Looping this over every index waits for ALL 5 threads to
    //   finish before main() is allowed to read/print sharedCounter below.
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    cout << "Final value of sharedCounter: " << sharedCounter << endl;

    // Reset sharedCounter for lock_guard demonstration
    sharedCounter = 0;
    cout << "\nDemonstrating with lock_guard:\n";
    // Create multiple threads to increment the shared counter using lock_guard
    // --- Added explanation: same generic syntax as the thread(...) call above
    // (parameter #1 = function to run, parameter #2 = id, parameter #3 =
    // increments) - here it targets incrementCounterwithLockGuard instead.
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = thread(incrementCounterwithLockGuard, i + 1, incrementsPerThread);
    }
    // Join all threads to the main thread
    // --- Added explanation: same as threads[i].join() explained above; waits
    // for all 5 lock_guard-based threads to finish before printing the total.
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

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() starts. sharedCounter = 0. An array `threads` of 5 empty
//     std::thread objects is declared.
// #2  First for-loop: 5 NEW threads are created back-to-back, each
//     immediately running incrementCounter(id, 100000) concurrently. Thread
//     i gets id = i+1 (so ids 1..5).
// #3  Inside each running incrementCounter: the loop runs 100000 times; each
//     iteration calls mtx.lock(), then does ++sharedCounter (reads the
//     current value, adds 1, writes it back), then mtx.unlock(). Because
//     mtx is a single shared mutex object, at most one thread at a time can
//     be between lock() and unlock(), so the read-modify-write on
//     sharedCounter can never be interrupted by another thread - this is
//     what makes it "thread-safe."
// #4  Second for-loop (still in main): threads[i].join() is called for
//     i = 0..4, in order. main blocks on threads[0].join() until thread 0
//     finishes; once it does, main blocks on threads[1].join(); and so on.
//     By the time this loop completes, ALL 5 threads have finished all
//     100000 increments each.
// #5  cout prints "Final value of sharedCounter: 500000" (5 threads *
//     100000 increments each, and the mutex guaranteed no increments were
//     lost to a race condition).
// #6  sharedCounter is reset to 0 and the exact same pattern (#2-#5) repeats
//     using incrementCounterwithLockGuard instead of incrementCounter -
//     functionally identical locking behavior, just expressed with
//     lock_guard's automatic (RAII) unlock instead of manual
//     mtx.lock()/mtx.unlock() calls.
// #7  main() returns 0, ending the program after printing both final
//     totals.
// ============================================================================