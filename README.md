# Advanced Enterprise C++ Handbook

**Prepared by Raushan Ranjan**

The Advanced Enterprise C++ Handbook is a practical set of personal training
notes designed to make professional C++ easier to learn, practise, and explain.
It starts with the foundations and gradually connects language features to
real-world software design, runnable examples, and small enterprise-style
use cases.

## What You Will Learn

The handbook covers:

- The transition from C to C++ and the native C++ build and execution model
- Classes, objects, constructors, access control, and object-oriented design
- Inheritance, runtime polymorphism, compile-time polymorphism, and operator design
- Exceptions, RAII, templates, and reusable generic code
- STL containers, iterators, algorithms, smart pointers, and modern C++ practices
- Files, threads, synchronization, networking, linking, and performance thinking
- Qt/QML integration, GUI examples, network applications, and final projects

The topics are not presented as a reference dump. Each lesson is written to
help you understand why a feature exists, how its syntax works, and where it
can be useful in a maintainable application.

## Learn By Doing

Each available module follows a simple learning loop:

1. Read the plain-language explanation.
2. Open the matching sample-code folder.
3. Copy a small example into VS Code.
4. Compile it from the terminal.
5. Run it and compare the output.
6. Change one small thing and run it again.
7. Build the complete use-case program.
8. Extend it through the homework task.

The notes include compiler commands, expected output, VS Code guidance, and
hints rather than complete homework solutions. This keeps the focus on
understanding and building confidence through repetition.

## Start Here

Open the handbook from the course home page:

**[Open the Enterprise C++ Handbook](https://raushan1107.github.io/Advanced-cpp-handbook/)**

If GitHub Pages has not been enabled for the repository yet, you can also open
the handbook locally by opening index.html in a browser.

Useful pages:

- [Course home](index.html)
- [How to use the repository and sample codes](repo-guide.html)
- [Student study-flow guide](module-authoring-context.html)
- [Module 0: C++ foundation](module0.html)
- [Module 1: C++ basics](module1.html)
- [Module 2: Inheritance and polymorphism](module2.html)
- [Module 3: Compile-time polymorphism](module3.html)

## Sample-Code Repository

The examples are included in the Advanced-cpp-sample-codes folder. The
numbered folders provide a classroom sequence, beginning with:

~~~text
01-Structs
02-FunctionsLearning
03-Datatype-modifiers
04-Conditional-Iterators
05-Array
06-Strings
07-Pointers
08-ClassesandObjects
09-Operators
10-ExceptionHandling
~~~

Continue with the later folders for STL, algorithms, smart pointers, move
semantics, threading, client-server programs, file descriptors, linking, and
exercises.

For the full run order and folder-by-folder classroom flow, open
[How to use the repository](repo-guide.html).

## Basic Terminal Commands

From a folder containing a C++ source file:

~~~text
g++ -std=c++17 program.cpp -o program
~~~

Run it on Windows:

~~~text
program.exe
~~~

Run it on Linux or macOS:

~~~text
./program
~~~

VS Code is the editor used for the lessons. GCC/G++, Clang, or Microsoft
Visual C++ is the compiler that turns the source code into an executable.

## About These Notes

This handbook is prepared as personal notes by **Raushan Ranjan** for learners
who already have some programming experience and want to understand C++ more
deeply. Familiar ideas from C#, Java, Python, and other languages are used as
analogies when they make a C++ concept easier to relate to, while the native
C++ rules and execution model are always explained separately.

The goal is simple: make the language less intimidating, make the terminal
workflow visible, and help every learner move from reading code to building
and explaining working programs.
