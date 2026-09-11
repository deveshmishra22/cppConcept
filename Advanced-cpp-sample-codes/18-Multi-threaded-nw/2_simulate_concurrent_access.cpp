#include <iostream>
#include <thread> // for std::thread
#include <vector> // for std::vector
#include <mutex> // for std::mutex, std::lock_guard
#include <chrono> // for sleep

std::mutex rr_mtx; // protects std::cout. Explanation: multiple threads writing to cout can mix up output.
// what is rr_mtx here?
// rr_mtx is a mutex (mutual exclusion) object that is used to synchronize access
// to the standard output stream (std::cout) in a multi-threaded environment.
// It ensures that only one thread can write to std::cout at a time, preventing interleaved or corrupted output.
// Why do we need mutex for cout?
// In a multi-threaded program, multiple threads may attempt to write to std::cout simultaneously
// without synchronization. This can lead to mixed-up or garbled output, making it difficult
// to read and understand the printed messages. By using a mutex to protect std::cout, we
// ensure that only one thread can write to it at a time. This prevents interleaved output and maintains the integrity
// of the printed messages.
// --- Added explanation: handle_request(id) ---------------------------------
// Generic syntax:  void handle_request( int id )
//   id - parameter #1 (only parameter): an identifying number for this
//        simulated request (1..8, passed from the for-loop in main()
//        below), used only in the printed "[Req id] started/finished"
//        messages and to vary how long this call sleeps.
void handle_request(int id) {

    {

        // --- Added explanation: lock_guard<mutex> lk(rr_mtx) -----------------
        // Generic syntax:  std::lock_guard<MutexType> variableName( mutexObj )
        //   MutexType - template parameter: the type of mutex wrapped
        //               (mutex, here).
        //   mutexObj  - constructor parameter #1: the specific mutex instance
        //               (rr_mtx) to lock RIGHT NOW, at construction. Its
        //               destructor calls rr_mtx.unlock() automatically when
        //               `lk` goes out of scope (end of this `{ }` block,
        //               right after the cout line below), so cout writes
        //               from different threads never interleave mid-line.
        std::lock_guard<std::mutex> lk(rr_mtx);
        std::cout << "[Req " << id << "] started\n";

    }
 
    // simulate work (I/O, DB, compute)

    // --- Added explanation: sleep_for(milliseconds(200 + (id % 5) * 100)) ----
    // Generic syntax:  std::this_thread::sleep_for( duration )
    //   duration - parameter #1: how long THIS thread pauses here, standing
    //              in for real I/O/DB/compute work. The value varies per
    //              request: 200 + (id % 5) * 100 ms, e.g. id=1 sleeps 300ms,
    //              id=5 sleeps 200ms, id=8 sleeps 500ms - so requests finish
    //              in a different order than they started.
    std::this_thread::sleep_for(std::chrono::milliseconds(200 + (id % 5) * 100));
 
    {

        // --- Added explanation: same as lock_guard<mutex> lk(rr_mtx) explained
        // above; locks rr_mtx for this cout line and auto-unlocks at the end
        // of this `{ }` block.
        std::lock_guard<std::mutex> lk(rr_mtx);
        std::cout << "[Req " << id << "] finished\n";

    }

}
 
int main() {

    const int concurrent = 8; // number of concurrent requests to simulate
    std::vector<std::thread> threads;
    // --- Added explanation: threads.reserve(concurrent) -----------------------
    // Generic syntax:  vectorObj.reserve( size_t newCapacity )
    //   newCapacity - parameter #1 (only parameter): how many elements of
    //                 storage to pre-allocate (concurrent, i.e. 8 here). This
    //                 is purely a performance optimization - it avoids the
    //                 vector reallocating (and moving already-created
    //                 std::thread objects) as emplace_back is called 8 times
    //                 below; it does not change any behavior.
    threads.reserve(concurrent);
    std::cout << "Simulating " << concurrent << " concurrent requests...\n";
    for (int i = 1; i <= concurrent; ++i) {
        // --- Added explanation: threads.emplace_back(handle_request, i) -----
        // Generic syntax:  vectorObj.emplace_back( callableFunction, arg1 )
        //   callableFunction - parameter #1: the function the new thread runs
        //                      (handle_request, here).
        //   arg1 (i)          - parameter #2: forwarded into handle_request's
        //                      `id` parameter. emplace_back constructs a NEW
        //                      std::thread directly inside the `threads`
        //                      vector, and that thread starts running
        //                      handle_request(i) immediately, concurrently
        //                      with every other thread already created by
        //                      earlier loop iterations.
        threads.emplace_back(handle_request, i);
    }
    // --- Added explanation: t.joinable() / t.join() ---------------------------
    // Generic syntax:  threadObj.joinable()   and   threadObj.join()
    //   (no parameters, either call) - joinable() returns true if `t`
    //   represents a real running/finished thread that hasn't been joined or
    //   detached yet (always true here, since none of these threads are ever
    //   detached); join() then blocks the calling (main) thread until `t`
    //   has fully finished running handle_request. Looping this over every
    //   entry in `threads` waits for ALL 8 requests to finish, one at a time
    //   in vector order, before main can print the "All simulated..." line.
    for (auto &t : threads) if (t.joinable()) t.join();
    std::cout << "All simulated requests completed.\n";
    return 0;

}

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() prints "Simulating 8 concurrent requests..." then enters the
//     for-loop (i = 1..8), calling threads.emplace_back(handle_request, i)
//     each time. Each call starts a NEW thread immediately running
//     handle_request(i) - so by the time the loop finishes, all 8 threads
//     are already running concurrently with main and with each other.
// #2  Inside each running handle_request(id):
//       #2a  locks rr_mtx, prints "[Req id] started", unlocks (lock_guard's
//            destructor releases it at the end of that `{ }` block).
//       #2b  sleeps for 200 + (id % 5) * 100 milliseconds (a stand-in for
//            real work) - NOT holding rr_mtx during the sleep, so other
//            threads are free to print their own "started"/"finished" lines
//            while this one sleeps.
//       #2c  locks rr_mtx again, prints "[Req id] finished", unlocks.
// #3  Because all 8 threads are started back-to-back with no synchronization
//     between their start times, and each sleeps a DIFFERENT duration based
//     on `id`, the "finished" messages print in whatever order each
//     thread's sleep happens to end - NOT necessarily 1,2,3...8. For example
//     id=5 sleeps 200ms while id=1 sleeps 300ms, so id=5 finishes before
//     id=1 even though id=1's thread was created (and started) first.
//     rr_mtx only guarantees
//     each individual "[Req id] started"/"finished" line prints as a whole,
//     un-interleaved line - it does NOT enforce any ordering between threads.
// #4  Back in main(), the final for-loop calls t.joinable() then t.join()
//     for each thread in `threads`, IN VECTOR ORDER (i.e. thread for id=1
//     first, then id=2, etc.) - regardless of the order they actually
//     finished in. Since t.join() blocks until that specific thread ends,
//     and most threads likely finished already during the earlier sleeps,
//     these joins usually return almost immediately; but if any thread is
//     still sleeping, main blocks right there until it's done before moving
//     on to join the next one.
// #5  Once all 8 joins return, main prints "All simulated requests
//     completed." and returns 0. By this point all 16 "started"/"finished"
//     messages (8 requests x 2 each) have already been printed, in an order
//     that depends on the sleep durations from step #3, not a fixed order.
// ============================================================================
 