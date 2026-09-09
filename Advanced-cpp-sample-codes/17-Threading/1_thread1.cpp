// What is threading in C++?
// Threading in C++ refers to the ability of a program to execute multiple sequences of instructions
// concurrently. This is achieved using threads, which are lightweight processes that can run in parallel
// within a single program. C++11 introduced a standard threading library that provides support for
// creating and managing threads, allowing developers to write multithreaded applications more easily.

// How it is implemented in C++?
// C++ provides the <thread> header that includes the std::thread class for creating and
// managing threads. You can create a thread by instantiating a std::thread object and passing
// a function or callable object to it. The thread starts executing the function in parallel
// with the main thread. You can also use synchronization mechanisms like mutexes and condition
// variables from the <mutex> and <condition_variable> headers to manage access to shared resources
// and coordinate between threads.

#include <iostream>
#include <thread>
using namespace std;

void SayHello() {
    cout << "Hello from Raushan's first c++ thread!" << endl;
}

int main() {
    // Creating a thread that runs the SayHello function
    thread t(SayHello);

    // Wait for the thread to finish execution
    t.join();
    // Is this mandatory to use join() here?
    // Yes, it is generally mandatory to use join() on a thread before the main thread
    // exits. If you don't join a thread, the program may terminate while the thread
    // is still running, leading to undefined behavior. Joining a thread ensures that
    // the main thread waits for the created thread to finish its execution.

    cout << "Main thread finished execution." << endl;
    return 0;
}