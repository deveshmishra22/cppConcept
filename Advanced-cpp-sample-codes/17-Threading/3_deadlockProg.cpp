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

// --- Added explanation: thread1Func() --------------------------------------
// Generic syntax:  thread1Func()
//   (no parameters) - this is the callable that gets handed to `thread t1`
//   in main(). It locks mtx1 FIRST, then mtx2 SECOND.
void thread1Func() {
    cout << "Thread 1 attempting to lock mtx1 and mtx2" << endl;

    // Thread 1 locks mtx1 then mtx2
    // --- Added explanation: mtx1.lock() ---------------------------------------
    // Generic syntax:  mtx1.lock()
    //   (no parameters) - blocks this thread until it alone holds mtx1.
    mtx1.lock();
    // --- Added explanation: this_thread::sleep_for(duration) ------------------
    // Generic syntax:  std::this_thread::sleep_for( <chrono duration> )
    //   duration - parameter #1: a std::chrono duration; here
    //              chrono::seconds(5) means "pause THIS thread for 5
    //              seconds" while still holding mtx1 - this is what widens
    //              the window for the deadlock below to actually occur.
    this_thread::sleep_for(chrono::seconds(5)); // Simulate some work
    // --- Added explanation: mtx2.lock() ---------------------------------------
    // Generic syntax:  mtx2.lock()
    //   (no parameters) - blocks this thread until it alone holds mtx2. If
    //   thread2Func has already locked mtx2 and is itself waiting on mtx1,
    //   this call waits forever - this is the deadlock.
    mtx2.lock();    
    //this_thread::sleep_for(chrono::seconds(5)); // Simulate some work


    cout << "Thread 1 acquired both locks." << endl;

    // --- Added explanation: mtx2.unlock() / mtx1.unlock() ---------------------
    // Generic syntax:  mtxObj.unlock()
    //   (no parameters) - releases the lock this thread holds on mtxObj, in
    //   reverse order of acquisition (mtx2 first, then mtx1), letting any
    //   other thread waiting on that mutex proceed.
    mtx2.unlock();
    mtx1.unlock();
    cout << "Thread 1 released both locks." << endl;
}

// --- Added explanation: thread2Func() --------------------------------------
// Generic syntax:  thread2Func()
//   (no parameters) - the callable handed to `thread t2` in main(). Notice
//   it locks mtx2 FIRST, then mtx1 SECOND - the OPPOSITE order from
//   thread1Func. This mismatched (non-uniform) lock ordering between the two
//   functions is exactly what causes the deadlock (see the workflow at the
//   bottom of this file, and "Lock Ordering" in the notes at the top).
void thread2Func() {
    cout << "Thread 2 attempting to lock mtx2 and mtx1" << endl;

    // Thread 2 locks mtx2 then mtx1
    // Same generic syntax as thread1Func's mtx1.lock()/sleep_for/mtx2.lock()
    // explained above, just with mtx1 and mtx2 swapped.
    mtx2.lock();
    this_thread::sleep_for(chrono::seconds(5)); // Simulate some work
    mtx1.lock();
    //this_thread::sleep_for(chrono::seconds(5)); // Simulate some work

    cout << "Thread 2 acquired both locks." << endl;

    mtx1.unlock();
    mtx2.unlock();
    cout << "Thread 2 released both locks." << endl;
}

// --- Added explanation: longTask() -----------------------------------------
// Generic syntax:  int longTask()
//   (no parameters) - returns an int (11) after simulating 10 seconds of
//   work via sleep_for. Only used by the commented-out std::async example in
//   main(), so with the code as it stands today this function is never
//   actually called.
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

    // --- Added explanation: thread t1(thread1Func) / thread t2(thread2Func) --
    // Generic syntax:  std::thread variableName( callableFunction )
    //   callableFunction - parameter #1 (only parameter here, since neither
    //                      thread1Func nor thread2Func takes arguments): the
    //                      function this new thread should start running
    //                      immediately upon construction.
    // Constructing t1 starts thread1Func running concurrently; constructing
    // t2 starts thread2Func running concurrently with BOTH t1 and main.
    thread t1(thread1Func);
    thread t2(thread2Func);

    // --- Added explanation: t1.join() / t2.join() ------------------------------
    // Generic syntax:  threadObj.join()
    //   (no parameters) - blocks the calling thread (main) until threadObj
    //   has completely finished. main will wait here on t1 first; if t1 and
    //   t2 have deadlocked each other (see workflow below), NEITHER join()
    //   call ever returns and the whole program hangs forever.
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

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() constructs t1 (running thread1Func) and t2 (running
//     thread2Func). Both start running concurrently, immediately, alongside
//     the main thread (which moves straight on to call t1.join()).
// #2  Thread 1 (thread1Func) - roughly in parallel with step #3 below:
//       #2a  prints "Thread 1 attempting to lock mtx1 and mtx2".
//       #2b  mtx1.lock() succeeds immediately (nobody else holds mtx1 yet).
//            Thread 1 now HOLDS mtx1.
//       #2c  sleep_for(5s): Thread 1 pauses for 5 seconds, STILL HOLDING
//            mtx1 the entire time.
//       #2d  after waking up, Thread 1 calls mtx2.lock() - it wants mtx2
//            NEXT, while still holding mtx1.
// #3  Thread 2 (thread2Func) - running at the same time as step #2:
//       #3a  prints "Thread 2 attempting to lock mtx2 and mtx1".
//       #3b  mtx2.lock() succeeds immediately (nobody else holds mtx2 yet,
//            and this typically happens before Thread 1 gets to step #2d
//            because Thread 1 is busy sleeping in #2c). Thread 2 now HOLDS
//            mtx2.
//       #3c  sleep_for(5s): Thread 2 pauses for 5 seconds, STILL HOLDING
//            mtx2 the entire time.
//       #3d  after waking up, Thread 2 calls mtx1.lock() - it wants mtx1
//            NEXT, while still holding mtx2.
// #4  THE DEADLOCK: by the time both threads reach #2d/#3d (their sleeps
//     were both ~5 seconds and started at nearly the same time, so they
//     wake at nearly the same time), the situation is:
//       - Thread 1 HOLDS mtx1, and is blocked in mtx2.lock() waiting for
//         mtx2, which Thread 2 is holding.
//       - Thread 2 HOLDS mtx2, and is blocked in mtx1.lock() waiting for
//         mtx1, which Thread 1 is holding.
//     Neither thread can ever release the mutex it holds, because releasing
//     happens AFTER the second lock() call succeeds - and neither second
//     lock() call can ever succeed. This circular "I'm waiting for what you
//     have, and you're waiting for what I have" is a classic deadlock.
// #5  Because of #4, thread1Func and thread2Func never reach their
//     "acquired both locks" / unlock() / "released both locks" lines.
// #6  Back in main(), t1.join() therefore blocks forever (Thread 1 never
//     finishes), so t2.join() is never even reached, and
//     "Main thread finished execution." is never printed. The program hangs
//     indefinitely and must be killed manually - this IS the deadlock the
//     comments at the top of this file describe.
// #7  How this file's own comments say to prevent it (see "Deadlock
//     prevention strategies" above): the simplest fix is Lock Ordering -
//     make BOTH functions lock mtx1 before mtx2 (i.e. change thread2Func to
//     lock mtx1 first, mtx2 second, matching thread1Func) so a circular
//     wait can never form.
// ============================================================================