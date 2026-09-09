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
    ThreadPool(size_t threads) : stop(false) {
        for(size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this, i] {
                for(;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if(stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for(std::thread &worker: workers)
            worker.join();
    }
};

int main() {
    ThreadPool pool(2); // Create a thread pool with 2 threads

    // Push a lambda task to the pool
    std::future<int> f1 = pool.enqueue([](int a, int b) {
        std::cout << "Task 1 running on thread " << std::this_thread::get_id() << std::endl;
        return a + b;
    }, 10, 20);

    // Push another lambda task
    std::future<std::string> f2 = pool.enqueue([](const std::string& msg) {
        std::cout << "Task 2 running on thread " << std::this_thread::get_id() << std::endl;
        return "Hello, " + msg;
    }, "ThreadPool");

    // Get results from the futures
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