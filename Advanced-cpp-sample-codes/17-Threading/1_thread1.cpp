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
#include<map>
#include<chrono>
using namespace std;

void SayHello() {
    cout << "Hello from Raushan's first c++ thread!" << endl;
}

// --- Added explanation: RefreshForecast(forecast) -------------------------
// Generic syntax:  RefreshForecast( map<string,int> forecast )
//   forecast - parameter #1 (and only parameter). It is a COPY (passed by
//              value, not by reference) of a map where each key is a city
//              name (string) and each value is a temperature (int).
//              Because it is a copy, any change made to `forecast` inside
//              this function (see item.second++ below) only affects the
//              local copy, NOT the ForecastMap that main() created.
// This function never returns (see the `while (true)` below with no break),
// so whichever line of code calls it will never get past that call.
void RefreshForecast(map<string,int> forecast)
{
    while (true)
    {
        for(auto& item : forecast)
        {
            item.second++;
            cout << item.first << " - " << item.second << endl;
        }
        // --- Added explanation: std::this_thread::sleep_for(duration) ------
        // Generic syntax:  std::this_thread::sleep_for( <chrono duration> )
        //   duration - parameter #1 (only parameter). Expects a std::chrono
        //              duration value, e.g. std::chrono::milliseconds(2000),
        //              std::chrono::seconds(2), or (as used here) the literal
        //              suffix form 2000ms, which means "pause THIS thread for
        //              2000 milliseconds (2 seconds)" before continuing to the
        //              next line. It pauses whichever thread calls it - here
        //              that is whichever thread happens to be running
        //              RefreshForecast.
        std::this_thread::sleep_for(2000ms);
    }  
}

int main() {
    map<string, int> ForecastMap = {
        { "Delhi", 31},
        { "Mumbai", 21},
        { "UK", 11},

    };
    // --- Added explanation: RefreshForecast(ForecastMap) --------------------
    // Generic syntax:  RefreshForecast( ForecastMap )
    //   ForecastMap - argument passed into the `forecast` parameter defined
    //                 above. This is a DIRECT, ORDINARY function call on the
    //                 MAIN thread (no `thread` object is involved), and
    //                 RefreshForecast contains an infinite `while(true)` loop
    //                 with no `break`/`return`. So this call NEVER returns:
    //                 everything below it in main() (creating the thread,
    //                 join(), the final cout) is unreachable while this
    //                 program runs as written.
    RefreshForecast(ForecastMap);
    // Creating a thread that runs the SayHello function
    // --- Added explanation: thread t(SayHello) -------------------------------
    // Generic syntax:  std::thread t( callableOrFunction, arg1, arg2, ... )
    //   callableOrFunction - parameter #1: the function (or lambda/functor)
    //                        the new thread should run. Here it is SayHello,
    //                        which itself takes no parameters.
    //   arg1, arg2, ...    - optional extra parameters #2+: any arguments the
    //                        callable needs would be listed here, comma
    //                        separated, and std::thread forwards them to the
    //                        callable when it starts running (SayHello needs
    //                        none, so none are supplied).
    // Constructing `t` immediately starts SayHello running concurrently on a
    // new OS thread, in parallel with the main thread.
    thread t(SayHello);

    // Wait for the thread to finish execution
    // --- Added explanation: t.join() -----------------------------------------
    // Generic syntax:  t.join()
    //   (no parameters) - blocks/pauses the CALLING thread (main, here) until
    //   the thread object `t` has completely finished running SayHello.
    t.join();
    // Is this mandatory to use join() here?
    // Yes, it is generally mandatory to use join() on a thread before the main thread
    // exits. If you don't join a thread, the program may terminate while the thread
    // is still running, leading to undefined behavior. Joining a thread ensures that
    // the main thread waits for the created thread to finish its execution.

    cout << "Main thread finished execution." << endl;
    return 0;
}


// when we go out of scope, all threads that are still running will be terminated, 
// which can lead to resource leaks or other issues. 
// Therefore, it is important to manage thread lifetimes properly and 
// ensure that all threads are joined or detached before the program exits.

// Note: In this example, we are using the C++17 language standard version, 
// which provides improved support for threading and 
// other modern C++ features.

// ============================================================================
// Added: STEP-BY-STEP EXECUTION FLOW (workflow) for this file
// ============================================================================
// #1  main() starts on the single, original thread ("main thread").
// #2  ForecastMap is constructed with 3 entries: Delhi=31, Mumbai=21, UK=11.
// #3  RefreshForecast(ForecastMap) is called. Because parameter `forecast`
//     is taken BY VALUE, the ENTIRE map is copied at this point - the
//     function now owns and works on its own private copy, `forecast`,
//     completely separate from main()'s `ForecastMap`.
// #4  Inside RefreshForecast, control enters `while(true)`:
//       #4a  for each (key, value) pair in `forecast`: value is incremented
//            by 1 (item.second++) and "key - value" is printed. So the first
//            pass prints Delhi-32, Mumbai-22, UK-12 (map iteration order,
//            which for std::map is sorted by key, so actually Delhi, Mumbai,
//            UK alphabetically).
//       #4b  after the for-loop finishes one full pass over the map,
//            std::this_thread::sleep_for(2000ms) pauses for 2 seconds.
//       #4c  the while(true) loop then repeats from #4a - forever. There is
//            no condition anywhere that stops this loop.
// #5  Because step #4 never ends, execution never returns to main() after
//     line `RefreshForecast(ForecastMap);`. Every following statement -
//     creating `thread t(SayHello)`, calling `t.join()`, and the final
//     "Main thread finished execution." message - is written in the source
//     but is NEVER reached while the program runs, because the single
//     (main) thread is permanently stuck inside RefreshForecast's loop.
// #6  (What WOULD happen if RefreshForecast returned, e.g. if you commented
//     out its call or gave the loop an exit condition): `thread t(SayHello)`
//     would construct a new std::thread object, and the instant it is
//     constructed a second, independent thread starts executing SayHello()
//     concurrently with main. That new thread prints
//     "Hello from Raushan's first c++ thread!" and then finishes/returns.
//     Meanwhile `t.join()` on the main thread blocks main until that second
//     thread has fully finished, guaranteeing the SayHello output is printed
//     before "Main thread finished execution." is printed and the program
//     returns 0, ending the process.
// ============================================================================