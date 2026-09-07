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
void handle_request(int id) {

    {

        std::lock_guard<std::mutex> lk(rr_mtx);
        std::cout << "[Req " << id << "] started\n";

    }
 
    // simulate work (I/O, DB, compute)

    std::this_thread::sleep_for(std::chrono::milliseconds(200 + (id % 5) * 100));
 
    {

        std::lock_guard<std::mutex> lk(rr_mtx);
        std::cout << "[Req " << id << "] finished\n";

    }

}
 
int main() {

    const int concurrent = 8; // number of concurrent requests to simulate
    std::vector<std::thread> threads;
    threads.reserve(concurrent);
    std::cout << "Simulating " << concurrent << " concurrent requests...\n";
    for (int i = 1; i <= concurrent; ++i) {
        threads.emplace_back(handle_request, i);
    }
    for (auto &t : threads) if (t.joinable()) t.join();
    std::cout << "All simulated requests completed.\n";
    return 0;

}
 