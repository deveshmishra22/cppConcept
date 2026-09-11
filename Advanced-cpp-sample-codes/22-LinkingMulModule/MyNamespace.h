// MyNamespace.h
// this is a header file defining a namespace
#include <iostream>
using namespace std;
// --- Added explanation: namespace MyNamespace { ... } ----------------------
// A namespace groups related names (here, just myFunction) under one
// qualifier so they don't collide with same-named things elsewhere in a
// program. Code outside this header would call this function either as
// `MyNamespace::myFunction();` or, after a `using namespace MyNamespace;`
// directive, simply as `myFunction();`.
namespace MyNamespace {
    // --- Added explanation: myFunction() ------------------------------------
    // Generic syntax:  MyNamespace::myFunction()
    //   (no parameters) - takes no input and returns nothing (void); it
    //   only prints a fixed message to demonstrate that the call reached
    //   this namespaced function.
    // Note: unlike header/rrmathlib.h in this same folder (which only
    // DECLARES its functions and lets a separate .cpp DEFINE them), this
    // header both declares AND defines myFunction() inline, right here in
    // the header. This file is not currently #included by any of the
    // main/src files in this folder - it is a standalone namespace example.
    void myFunction()
    {
        // function implementation
        cout << "Inside MyNamespace function" << endl;
    };
}
