# Enterprise C++ Sample Code Sequence

> **Filename convention:** each C++ sample is now prefixed with its teaching
> order, for example `1_Program1.cpp` and `2_namespaceprog1.cpp`. The concept
> names below retain their original basenames for readability; compile the
> corresponding prefixed file.

## One-Time Setup

Open the repository in VS Code:

```powershell
cd Advanced-cpp-sample-codes
code .
g++ --version
```

VS Code is the editor. g++ is the compiler. Recompile after every source change.

Standard build pattern:

```powershell
g++ -std=c++17 path\to\file.cpp -o program
.\program.exe
```

Use ./program on Linux/macOS. Prefer a fresh output name for each demo instead
of trusting a previously committed executable.

## Classroom Sequence

### 00. Toolchain and C++ version

Recap the source -> compiler -> linker -> executable -> operating-system loop.

From the repository root:

```powershell
g++ -std=c++17 cppversioncheck.cpp -o cppversioncheck
.\cppversioncheck.exe
```

Discuss main, includes, the selected C++ standard, and why a changed source file
does not change the executable until a successful build.

Root references:

- cpp_mind_map_key_values.txt: vocabulary map.
- cppMemoryTypes.cpp: memory preview.
- lambda_examples.cpp: later preview only; teach it in the Module 7 sequence.

### 01. Foundation: structs

Recap data fields, functions, value types, and C-to-C++ syntax.

Open 01-Structs and start with Program1.cpp:

```powershell
cd 01-Structs
g++ -std=c++17 Program1.cpp -o structs_demo
.\structs_demo.exe
```

Discuss struct, member access with ., input/output, and the difference between a
data record and the functions that operate on it.

### 02. Functions and namespaces

Recap declarations, definitions, parameters, return values, and std qualification.

Open 02-FunctionsLearning and run func1.cpp, then namespaceprog1.cpp:

```powershell
cd ..\02-FunctionsLearning
g++ -std=c++17 func1.cpp -o functions_demo
.\functions_demo.exe
g++ -std=c++17 namespaceprog1.cpp -o namespace_demo
.\namespace_demo.exe
```

Discuss how namespaces prevent collisions and relate to packages/modules in C#,
Java, Python, and JavaScript.

### 03. Types and modifiers

Recap primitive types, const, type deduction, references, and structured bindings.

Open 03-Datatype-modifiers in this order:

1. modifiersint.cpp
2. modifiersdouble.cpp
3. modifierschar.cpp
4. modifierconst.cpp
5. dectTypeTool.cpp
6. Contexpression.cpp
7. strucBinding.cpp

Compile one file at a time using the standard build pattern.

### 04. Conditions and iteration

Recap Boolean expressions, branches, loops, break, continue, and ranges.

Open 04-Conditional-Iterators in this order:

1. ifcondition.cpp
2. switchcondition.cpp
3. forloop.cpp
4. whileloop.cpp
5. dowhileloop.cpp
6. for-jump-continue-break.cpp
7. for-and-forrange.cpp
8. ternaryoperator.cpp

Example:

```powershell
cd ..\04-Conditional-Iterators
g++ -std=c++17 ifcondition.cpp -o condition_demo
.\condition_demo.exe
```

### 05. Arrays and 06. Strings

Recap contiguous storage, indexes, loops, character data, and string input.

Open 05-Array in this order: arr1.cpp, arr2.cpp, twodarray.cpp, jaggedarray.cpp.

Then open 06-Strings in this order: s1.cpp, stringinput.cpp, stringscompare.cpp,
stringCapacity.cpp, stringbufferarray.cpp, stringStream.cpp,
manipulatestring.cpp, cstdlibrary.cpp, regx.cpp.

Pause before regular expressions and explain why std::string is easier to use
than manually managing a character array.

### 07. Pointers and 08. Classes

Recap address, pointer, dereference, lifetime, stack versus heap, and object
invariants.

Open 07-Pointers: Checkmemoryaddress.cpp, pointer2var.cpp, pointer2pointer.cpp.

Open 08-ClassesandObjects in this order:

1. c1.cpp
2. c2withconstructor.cpp
3. destructorprog.cpp
4. staticmemandfunc.cpp
5. friendFunction.cpp
6. friendclass.cpp
7. inheritancecls.cpp
8. polymorphismcls.cpp
9. purevirtual-interfaces.cpp
10. virtualdestructor.cpp
11. vtablemechanism.cpp

```powershell
cd ..\08-ClassesandObjects
g++ -std=c++17 c1.cpp -o class_demo
.\class_demo.exe
```

The first six files support Modules 1 and 2. The last five support the planned
inheritance and runtime-polymorphism lessons.

### 09. Operators

Recap arithmetic, assignment, comparison, logical, increment/decrement, and
bitwise operators.

Open 09-Operators in this order:

1. Arthmeticoperator.cpp
2. Assignmentoperator.cpp
3. Relationaloperator.cpp
4. Logicaloperator.cpp
5. IncrementDecrementoperator.cpp
6. Bitwiseoperator.cpp

Use this before operator overloading on custom types.

### 10. Exceptions and 11. File streams

Recap invalid states, return codes, failure paths, and resource lifetime.

Open 10-ExceptionHandling in this order: GetStartedCode.cpp, Prog1.cpp,
customexception.cpp, customexceptionProgram.cpp, fileexception.cpp,
filehandlingexception.cpp.

Then open 11-FStream: filehandlingmodes.cpp, followed by raii.cpp.

```powershell
cd ..\10-ExceptionHandling
g++ -std=c++17 GetStartedCode.cpp -o exceptions_demo
.\exceptions_demo.exe
```

Discuss why RAII is the C++ answer to cleanup that might otherwise be forgotten
on an error path.

### 12. Containers, 13. Iterators, and 14. Algorithms

Recap choosing a data structure, traversing a range, and separating an algorithm
from the container.

Run 12-DataStrucCollections in this order:

1. vectorprog.cpp
2. listsprog.cpp
3. stack.cpp
4. queues.cpp
5. set-program.cpp
6. map-program.cpp

Then run 13-Iterators\iterators.cpp, followed by
14-Alogrithms\foreachalog.cpp, find.cpp, count.cpp, accumulate.cpp, and
sorting.cpp.

This is the Module 6 path for the student-records case study.

### 15. Smart pointers, 16. Move semantics, and 17. Threading

Recap ownership, lifetime, copying, moving, and shared mutable state.

Run:

- 15-Smart_Pointers\cppMemoryTypes.cpp
- 15-Smart_Pointers\memoryprog2.cpp
- 16-MoveSemantics\movesemantics1.cpp
- 17-Threading\thread1.cpp
- 17-Threading\thread-mutex.cpp
- 17-Threading\deadlockProg.cpp
- 17-Threading\threadpool.cpp

For thread examples on Linux, add -pthread:

```powershell
g++ -std=c++17 thread1.cpp -pthread -o thread_demo
.\thread_demo.exe
```

### 18. Networking and 19. Server/client

Recap processes, sockets, client/server roles, blocking versus non-blocking work,
and why concurrency appears in network programs.

Open 18-Multi-threaded-nw for local concurrency and socket examples. Then open
19-Server-Client in this order:

1. simple_echo_server.cpp
2. simple_server.cpp
3. server.cpp
4. client.cpp
5. threaded_server_win.cpp and threaded_client_win.cpp
6. poll_echo_server_win.cpp and poll_client_win.cpp

Run a server and client in separate terminals. Several examples are Windows
specific; explain the platform dependency before attempting to port them.

### 20. Pipes and 21-22. Linking

Open 20-File-descriptor-pipe-stream for pipes and file descriptors.

Then open 21-StaticvsDynamicLinking and 22-LinkingMulModule to explain how
multiple source files and libraries become one application. Build from source
instead of trusting committed exe, o, a, or dll files.

### 23-25. Extensions and exercises

Use 23-ReadingsystemInfo as a systems extension. Use 24-Exercises after
Modules 0-3 for learner practice. Finish with 25-demo only after the class
understands separate compilation and linking.

## Curriculum Gaps

The sample repository does not currently contain dedicated folders for:

- 05-Templates: planned function/class templates and Stack<T> examples.
- 09-Qt-QML: planned Qt widgets, QML, signals/slots, and Qt networking.

Do not present those examples as available until the folders and code exist.

## Teaching Rhythm

For every file:

1. Ask learners to predict the output.
2. Compile it in front of them.
3. Run it and compare the result.
4. Change one line and rebuild.
5. Explain the changed behavior.
6. Connect the result to the next handbook concept.

The numbered folders are a delivery aid. Keep the original source filenames and
teach one concept at a time.
