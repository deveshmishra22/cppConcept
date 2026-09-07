# Enterprise C++ Handbook: Module Authoring Context

This file is the source of truth for writing or rewriting modules in this handbook.
Future AI-assisted edits should read this file before changing lesson content.

## Teaching Goal

Teach enterprise C++ by moving from a plain-language concept to a small runnable
experiment, then to one complete use-case program. Learners should be able to
copy, compile, run, inspect the output, change one thing, and explain what changed.

Do not assume that a learner already understands C++ syntax, the compiler, the
standard library, namespaces, or the terminal.

## Required Module Shape

Every module must follow this order:

1. Module purpose and learning outcomes.
2. Generic concept explanation: why the concept exists and what problem it solves.
3. Syntax vocabulary: explain every new keyword, punctuation pattern, type, and
   library name before using it in a larger example.
4. Progressive Try Now ladder: several small code blocks, each focused on one
   idea.
5. One command and one expected output for every Try Now block.
6. A separate complete `program.cpp` that combines the module ideas in one small,
   realistic use case.
7. A complete-program run guide for VS Code.
8. Homework or assessment that extends the complete program.
9. Hints only. Do not provide the homework solution or full homework code.
10. Submission instructions asking the learner to email `program.cpp` for review.

## Progressive Try Now Rules

Each step should be independently copyable and runnable. Prefer three to five
steps, increasing complexity one small change at a time.

Every step must show:

- a unique filename such as `step1_hello.cpp`;
- the complete code for that step;
- the compiler command;
- the run command for Windows and Linux/macOS when they differ;
- a short expected output or interaction example;
- one sentence explaining what changed from the previous step.

Use this command pattern unless the module requires another standard:

```text
g++ -std=c++17 step.cpp -o step
```

Windows run command:

```text
step.exe
```

Linux/macOS run command:

```text
./step
```

## Complete Use-Case Rules

The final exercise must be a new, separately named `program.cpp`, not merely the
last progressive snippet copied unchanged. It should:

- use the concepts taught in the module;
- solve one understandable use case;
- contain comments only where they clarify a non-obvious decision;
- compile with the command shown on the page;
- show expected output;
- invite learners to change input or one rule and run it again.

The homework must continue the same use case. It should request one or more
meaningful features, but must not include the completed implementation.

## Beginner Explanation Rules

Explain new names at first use. For example, before using `std::cout`, explain
that:

- `#include <iostream>` makes standard input/output declarations available;
- `std` is the namespace owned by the C++ standard library;
- `::` qualifies a name from a namespace;
- `cout` writes to standard output, `cin` reads standard input, and `endl` writes
  a newline and flushes the output buffer;
- VS Code is an editor, while GCC/g++, Clang/clang++, or MSVC/cl is the compiler;
- compiling and linking produce the executable that the operating system runs.

When useful, relate a C++ idea to a familiar equivalent in Python, Java,
JavaScript, or another language, but clearly explain that the syntax and runtime
model are not identical.

For audiences with C# experience, explicitly compare the execution models:

- C# source is compiled to CIL/MSIL, then loaded and JIT-compiled by the CLR;
- C++ source is preprocessed, compiled to object code, linked with libraries,
  and loaded as a native executable by the operating system;
- both still use the same practical loop: edit, build, run, inspect, change, and
  build again.

Show the terminal workflow early. Learners should see how to create a folder,
open it in VS Code, compile from the terminal, run the executable, and rebuild
after a change. Explain that VS Code is an editor, not the compiler.

## Promise Audit

Before publishing a module, search its text for every claim such as "covered in
Module N", "we will introduce", or "the next module". Each claim must be one of:

- implemented in the current page;
- clearly marked as planned or coming soon;
- removed because it is no longer part of the curriculum.

Do not present a future module's case study, tool, or assessment as already
available. The index and module notes must use the same status language.

## Homework Submission Pattern

Use this wording unless the trainer supplies a specific email address:

"Send your finished `program.cpp` to the trainer's cohort email. Use your name in
both places."

Subject format:

```text
Module N Task - Your Name - program.cpp
```

Body format:

```text
Your Name | Module N | compiler used | result you observed
```

Ask learners to attach the exact file they compiled and briefly describe any
different output or failed test.

## Hint Rules

Hints should point toward a design or API without writing the answer. Good hints
mention relevant keywords, function signatures, data flow, validation rules, or
the order of operations. Do not paste a complete homework function or solution.

## Existing Implementation Reference

- `module0.html` demonstrates the foundation-first format, including toolchain
  setup, syntax, namespaces, cross-language analogies, incremental exercises,
  and a procedural learner-progress use case.
- `module1.html` demonstrates the class-building format, including incremental
  bank-account steps, a complete class-based use case, and transfer homework.
- `module2.html` and `module3.html` demonstrate the Day 2 format: topic
  sections are controlled from the left navigation, while the main content
  remains full-width and includes progressive compile/run practice.
- `style.css` contains the shared visual styles for `try-now`, `build-ladder`,
  `build-step`, `task-card`, and `submission-card`.

Modules 4 through 10 are still listed on the index as coming soon. When their
lesson pages are created, use this exact structure and preserve the handbook's
WHY / WHAT / HOW explanation rhythm.
