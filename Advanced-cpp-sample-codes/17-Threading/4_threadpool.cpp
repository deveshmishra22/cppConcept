#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

public:
    // --- Added explanation: ThreadPool(threads) constructor ------------------
    // Generic syntax:  ThreadPool poolName( size_t threads )
    //   threads - parameter #1 (only parameter): how many worker OS threads
    //             to create and keep alive for the lifetime of this pool
    //             (2, when called as `ThreadPool pool(2)` in main() below).
    ThreadPool(size_t threads) : stop(false) {
        // --- Added explanation: workers.emplace_back(lambda) --------------------
        // Generic syntax:  workers.emplace_back( callable )
        //   callable - parameter #1: a lambda `[this, i] { ... }` that
        //              becomes this new thread's entire body. `this` is
        //              captured so the lambda can reach the pool's members
        //              (tasks, queue_mutex, condition, stop); `i` is captured
        //              (though unused in the body) as the worker's index.
        //              emplace_back constructs a NEW std::thread in-place
        //              inside the `workers` vector, and that thread starts
        //              running the lambda immediately.
        for(size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this, i] {
                // Each worker thread loops forever, waiting for and running
                // tasks, until the pool is destroyed (see ~ThreadPool below).
                for(;;) {
                    std::function<void()> task;
                    {
                        // --- Added explanation: unique_lock<mutex> lock(queue_mutex)
                        // Generic syntax:  std::unique_lock<MutexType> name( mutexObj )
                        //   mutexObj - parameter #1: the mutex to lock right
                        //              away (queue_mutex). Unlike lock_guard,
                        //              unique_lock CAN be temporarily released
                        //              and re-acquired, which condition.wait
                        //              (right below) needs to do internally.
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        // --- Added explanation: condition.wait(lock, predicate) --
                        // Generic syntax:  condVar.wait( lock, predicateLambda )
                        //   lock            - parameter #1: the unique_lock this
                        //                     thread currently holds; wait()
                        //                     releases it WHILE SLEEPING and
                        //                     re-acquires it before returning.
                        //   predicateLambda - parameter #2: a callable returning
                        //                     bool. wait() sleeps until
                        //                     notify_one()/notify_all() wakes it
                        //                     up AND this predicate is true;
                        //                     here that means "either the pool
                        //                     is stopping, or there is a task
                        //                     waiting in the queue."
                        this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        // Take ownership of the front task and remove it from
                        // the shared queue while still holding queue_mutex.
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    // Lock is released here (end of the block above) before
                    // running the task, so other workers can pull the NEXT
                    // task while this one is busy running its own.
                    task();
                }
            });
        }
    }

    // --- Added explanation: enqueue(f, args...) -------------------------------
    // Generic syntax:  pool.enqueue( callableF, arg1, arg2, ..., argN )
    //   callableF    - parameter #1: any callable (plain function, lambda,
    //                  functor) to run on one of the pool's worker threads.
    //   arg1..argN   - parameters #2+: however many arguments callableF
    //                  needs; each is forwarded straight through to
    //                  callableF when a worker eventually invokes it (e.g.
    //                  `pool.enqueue(myFunc, 10, 20)` calls myFunc(10, 20)).
    //   returns        a std::future<ReturnType> that will hold callableF's
    //                  return value once some worker thread finishes running
    //                  it; call .get() on that future to block until the
    //                  result is ready and retrieve it.
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        
        // Wrap the callable + its bound arguments into a packaged_task,
        // which pairs the callable with a future for its eventual result.
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        // Grab the future BEFORE handing the task off to the queue, since
        // once it's queued a worker could start (and finish) it at any time.
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if(stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task](){ (*task)(); });
        }
        // --- Added explanation: condition.notify_one() ----------------------------
        // Generic syntax:  condVar.notify_one()
        //   (no parameters) - wakes up exactly ONE thread that is currently
        //   sleeping in condition.wait() (one idle worker), so it can check
        //   its predicate again, see the newly queued task, and pick it up.
        condition.notify_one();
        return res;
    }

    // --- Added explanation: ~ThreadPool() destructor --------------------------
    // Generic syntax:  (called automatically when a ThreadPool variable goes
    //   out of scope, e.g. `pool` in main() below, at the end of main())
    //   (no parameters) - signals every worker to stop and waits for them.
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        // --- Added explanation: condition.notify_all() ----------------------------
        // Generic syntax:  condVar.notify_all()
        //   (no parameters) - wakes up EVERY thread currently sleeping in
        //   condition.wait() (all idle workers at once), so each re-checks
        //   its predicate, sees `stop` is now true, and exits its for(;;) loop.
        condition.notify_all();
        // --- Added explanation: worker.join() --------------------------------------
        // Generic syntax:  threadObj.join()
        //   (no parameters) - blocks THIS (destructor-running) thread until
        //   `worker` has fully exited its loop and returned. Looping this
        //   over every entry in `workers` waits for all of them before the
        //   destructor - and therefore the whole program shutdown - completes.
        for(std::thread &worker: workers)
            worker.join();
    }
};

int main() {
    // --- Added explanation: ThreadPool pool(2) -----------------------------
    // Generic syntax:  ThreadPool variableName( size_t threads )
    //   threads (2) - parameter #1: number of worker threads to spin up now.
    ThreadPool pool(2); // Create a thread pool with 2 threads

    // Push a lambda task to the pool
    // --- Added explanation: pool.enqueue(lambda, arg1, arg2) ----------------
    // Generic syntax:  pool.enqueue( callableF, arg1, arg2 )
    //   callableF - parameter #1: the lambda `[](int a, int b) {...}`, which
    //               itself takes two int parameters `a` and `b`.
    //   arg1 (10) - parameter #2: forwarded into the lambda's `a`.
    //   arg2 (20) - parameter #3: forwarded into the lambda's `b`.
    // This QUEUES the task and returns right away with a std::future<int>
    // (f1) - it does NOT run the lambda on the calling (main) thread, nor
    // does it block waiting for it to finish.
    std::future<int> f1 = pool.enqueue([](int a, int b) {
        std::cout << "Task 1 running on thread " << std::this_thread::get_id() << std::endl;
        return a + b;
    }, 10, 20);

    // Push another lambda task
    // --- Added explanation: pool.enqueue(lambda, arg1) ----------------------
    // Generic syntax:  pool.enqueue( callableF, arg1 )
    //   callableF        - parameter #1: the lambda `[](const string& msg) {...}`,
    //                       which takes one parameter `msg`.
    //   arg1 ("ThreadPool") - parameter #2: forwarded into the lambda's `msg`.
    // Also queues and returns immediately, this time with a
    // std::future<std::string> (f2).
    std::future<std::string> f2 = pool.enqueue([](const std::string& msg) {
        std::cout << "Task 2 running on thread " << std::this_thread::get_id() << std::endl;
        return "Hello, " + msg;
    }, "ThreadPool");

    // Get results from the futures
    // --- Added explanation: f1.get() / f2.get() -----------------------------
    // Generic syntax:  futureObj.get()
    //   (no parameters) - blocks the calling (main) thread until whichever
    //   worker thread is running that task finishes, then returns the task's
    //   return value (an int for f1, a std::string for f2). Calling .get()
    //   on the same future object a second time is undefined behavior - it
    //   is meant to be called once per future.
    std::cout << "Result of Task 1: " << f1.get() << std::endl;
    std::cout << "Result of Task 2: " << f2.get() << std::endl;

    return 0;
}


// or
// #include <ctpl.h> // Include the CTPL header
// #include <iostream>
// #include <string>

// int main() {
//     ctpl::thread_pool p(2); // Create a thread pool with 2 threads

//     // Push a lambda task to the pool
//     std::future<int> f1 = p.push([](int id, int a, int b) {
//         std::cout << "Task 1 running on thread " << id << std::endl;
//         return a + b;
//     }, 10, 20);

//     // Push another lambda task
//     std::future<std::string> f2 = p.push([](int id, const std::string& msg) {
//         std::cout << "Task 2 running on thread " << id << std::endl;
//         return "Hello, " + msg;
//     }, "CTPL");

//     // Get results from the futures
//     std::cout << "Result of Task 1: " << f1.get() << std::endl;
//     std::cout << "Result of Task 2: " << f2.get() << std::endl;

//     return 0;
// }

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() constructs `ThreadPool pool(2)`. Inside the constructor, 2
//     worker threads are created and immediately start running their
//     for(;;) loop. Both call condition.wait(...) right away and go to
//     sleep, because `tasks` is empty and `stop` is false (predicate is
//     false, so wait() keeps sleeping).
// #2  main() calls pool.enqueue(lambda1, 10, 20):
//       #2a  lambda1 + its bound args (10, 20) are wrapped into a
//            packaged_task and pushed onto the shared `tasks` queue (while
//            holding queue_mutex).
//       #2b  f1 (the future tied to that packaged_task) is handed back to
//            main immediately - main does NOT wait for lambda1 to run.
//       #2c  condition.notify_one() wakes exactly ONE sleeping worker.
// #3  The woken worker re-checks its predicate, sees `tasks` is non-empty,
//     pops lambda1's packaged_task off the queue, releases queue_mutex, and
//     calls task() - running lambda1(10, 20) on that worker thread. It
//     prints "Task 1 running on thread <id>" and computes 10 + 20 = 30,
//     which is stored inside the packaged_task and becomes visible through
//     f1 once someone calls f1.get().
// #4  Back in main(), pool.enqueue(lambda2, "ThreadPool") repeats the same
//     pattern (#2a-#2c) for a second task, waking the SECOND worker thread
//     (or the first one again if it already finished task 1), which runs
//     lambda2("ThreadPool") and produces the string "Hello, ThreadPool",
//     stored for retrieval through f2.
// #5  main() calls f1.get(): this blocks main until worker running lambda1
//     has finished and stored its result, then returns 30, and
//     "Result of Task 1: 30" is printed.
// #6  main() calls f2.get(): likely already finished by this point (it was
//     enqueued and probably ran concurrently with lambda1), so this returns
//     almost immediately with "Hello, ThreadPool", and
//     "Result of Task 2: Hello, ThreadPool" is printed.
// #7  main() returns 0. `pool` (a local variable) goes out of scope, so its
//     destructor ~ThreadPool() runs automatically:
//       #7a  sets stop = true (under queue_mutex).
//       #7b  condition.notify_all() wakes BOTH worker threads.
//       #7c  each worker re-checks its predicate, sees stop == true AND
//            tasks.empty() == true, so it returns from its for(;;) loop and
//            the worker thread function ends.
//       #7d  the destructor's `for(std::thread &worker: workers) worker.join();`
//            waits for both worker threads to fully finish before the
//            destructor - and the whole program - completes.
// ============================================================================