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
11. A Story Mode tab (see the dedicated section below) as the last tab in the
    module's tab bar. This is required once per module, not once per topic tab.

## Story Mode Tab (Required, One Per Module)

Every module's tab bar ends with one additional tab — Story Mode — that retells
the whole module as a single real-world narrative instead of another WHY/WHAT/HOW
topic. It exists so a trainer can teach the technical tabs first, then switch to
this tab and narrate: "here is the actual problem all of that just solved." Do
not add a Story Mode tab per lesson/topic — exactly one per module, always last.

### Placement and numbering

- Add it as the final button in both `.tabbar` (main content) and `.nav-tabs`
  (sidebar), using the next `tN` id after the module's last topic tab (module
  with topics `t1`-`t4` gets Story Mode as `t5`; module0's `t0`-`t5` gets `t6`).
- Every existing topic tab's trailing element (a `.topic-nav`, or the last block
  in the panel if no `.topic-nav` exists) should gain or keep a forward link/button
  to Story Mode: `<button class="btn btn-primary" onclick="document.querySelector('[data-tab=tN]').click()">Story Mode: see it all connected →</button>`.
  Reuse the file's existing tab-jump convention — every module page in this repo
  wires jump buttons the same way, by calling `.click()` on the real tab button
  via `document.querySelector('[data-tab=tX]')`. Do not invent a new `data-*`
  attribute or new JS handler; the existing `initTabs()` in each file already
  listens on `.tabbar button, .nav-tab` and needs nothing new to support this.
- Update the module header's `Topics:` count in `.module-meta` if that module
  displays one (add 1 for the new tab).
- Add one bullet to that module's card on `index.html`:
  `<li>🎬 Story Mode: {short hook}</li>` as the last `<li>` in its `.topic-list`.

### Required internal structure

```html
<div class="tabpanel" id="tN">   <!-- or <section class="tabpanel"> if the page uses <section> panels -->
  <span class="story-badge">🎬 Story Mode</span>
  <h2 style="margin:0 0 12px;">{Story title}</h2>
  <p class="story-lede">{2-4 sentence framing, addressed to the learner, present
     tense, "picture this" voice — states the real problem before any acts.}</p>

  <div class="story-timeline">
    <div class="story-act">
      <div class="story-act-head">
        <span class="story-act-num">ACT 1</span>
        <h3>{Act title}</h3>
        <button class="story-jump" type="button" onclick="document.querySelector('[data-tab=t1]').click()">↳ Revisit {Topic name}</button>
      </div>
      <p>{One paragraph narrating why this specific topic tab's concept had to
         exist, told through the module's story characters/company.}</p>
    </div>
    <!-- one .story-act per topic tab in the module, same order as the tab bar -->
  </div>

  <blockquote class="story-pullquote">{One memorable, quotable line that sums up
     the module's real lesson.}</blockquote>

  <div class="story-also">
    <p class="story-also-title">Also seen in the wild</p>
    <div class="story-also-grid">
      <div class="story-also-item"><span class="story-also-domain">{Domain}</span>{One-line secondary analogy.}</div>
      <!-- 3-4 items total -->
    </div>
  </div>

  <div class="callout"><strong>Bring it back:</strong> {One line tying the story
     back to this module's actual complete program/homework, and teasing what
     the next module's story will need.}</div>
</div>
```

All of `.story-hero`/`.story-badge`/`.story-lede`/`.story-timeline`/`.story-act`/
`.story-pullquote`/`.story-also` are already defined:

- In `style.css` (light theme) — used as-is by any new module built in that theme
  (this is the theme `module2.html` and `module3.html` use, and the one new
  modules should default to unless told otherwise).
- Duplicated inline in `module0.html`'s and `module1.html`'s own `<style>` block
  (dark "Blueprint" theme) — only relevant if a future module is built in that
  same dark, self-contained-`<style>` format. Copy that CSS block (search either
  file for `/* ---------- Story Mode ---------- */`) into the new page's own
  `<style>` rather than linking `style.css`, to match that theme's tokens.

### Choosing the story's theme

- Pick ONE primary real-world analogy per module, and it must match what that
  module's topic tabs actually teach and actually show in code — never invent a
  story whose entities don't correspond to the real example code on the page.
  (Module 2's Story Mode is a game engine's Shape renderer because Module 2's
  real code is a Shape/Circle/Rectangle hierarchy — it is not a banking story,
  even though Module 1 is, because Module 2's code isn't about banking.)
- Inside `.story-also`, add 2-4 *secondary* one-line analogies from other
  unrelated domains (finance, healthcare, GUI toolkits, plugin systems, maps,
  automotive, etc.) so the learner sees the pattern is general — these are brief
  "also seen in the wild" callouts, not full parallel narratives.
- This repo runs a light continuity device across modules — a recurring
  fictional company family named "Nimbus" (Nimbus Vending in Module 0, Nimbus
  Bank in Module 1, Nimbus Games in Modules 2 and 3). Continue "Nimbus" into a
  later module's story only where it's a natural fit for that module's real
  content; when it isn't a fit, use a fresh company name rather than forcing it.
  This is flavor, not a hard requirement.

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
  and a procedural learner-progress use case. Dark "Blueprint" theme, styled via
  its own inline `<style>` block (not `style.css`).
- `module1.html` demonstrates the class-building format, including incremental
  bank-account steps, a complete class-based use case, and transfer homework.
  Same dark inline-`<style>` theme as `module0.html`.
- `module2.html` and `module3.html` demonstrate the Day 2 format: topic
  sections are controlled from the left navigation, while the main content
  remains full-width and includes progressive compile/run practice. Light theme,
  styled via the shared `style.css`.
- `style.css` contains the shared visual styles for `try-now`, `build-ladder`,
  `build-step`, `task-card`, `submission-card`, and `story-*` (Story Mode).
- `module4.html` and `module5.html` demonstrate the Day 3 format: same
  light `style.css` theme as module2/module3. module4 covers exception handling
  (try/catch/throw, catch-by-type, custom exceptions, RAII); module5 covers
  templates and generics (function templates, class templates, STL as templates).
  Both end with a Story Mode tab and a try-now section with complete program.
- `module6.html` demonstrates the Day 4 STL module: sequence containers (vector,
  list, deque), associative containers (map, set, unordered_map), algorithms
  (sort, find_if, count_if, transform, accumulate), and iterators. Case study:
  student records analyzer. Story Mode: "Nimbus Academy — the records that
  couldn't be found."
- `module7.html` demonstrates the Day 4 Advanced C++ module: lambdas (captures,
  std::function), smart pointers (unique_ptr, shared_ptr, weak_ptr), file I/O
  (ofstream, ifstream, RAII file handling), and multithreading (std::thread,
  mutex, lock_guard, async, future). Case study: concurrent sensor log aggregator.
  Story Mode: "Nimbus Systems — the pipeline that kept crashing at midnight."
- `module8.html` demonstrates the Day 5 Systems Programming & Advanced C++
  Internals module — a full-day, six-topic module (largest in the handbook):
  networking & sockets (TCP vs UDP, Winsock client/server), move semantics
  (rvalue references, std::move), pipes & file descriptors (Windows
  CreatePipe/CreateProcess IPC), static vs dynamic linking (built three ways
  and verified on this repo's MinGW toolchain), reading system info (Win32
  SYSTEM_INFO/MEMORYSTATUSEX plus the portable std::thread::hardware_concurrency()),
  and an Additional Topics lesson on typeid/dynamic_cast/RTTI use cases/
  polymorphic type identification. Case study: fleet diagnostics relay agent
  (move semantics + RTTI + networking; pipes/linking/system info are taught as
  standalone systems-level exercises rather than forced into the same program —
  the try-now intro states this explicitly). Repo folder: `27-RTTI/` is new
  (created for this module — see the Curriculum Gaps section in
  `Advanced-cpp-sample-codes/CPP_CODE_SEQUENCE_CONTEXT.md`). Story Mode:
  "Nimbus Systems — the on-call page that came from the wrong data center."
- Every module currently live (`module0`–`module8`) already ends its tab bar
  with a Story Mode tab — see the dedicated section above before adding a new
  one. Read an existing Story Mode tab (e.g. `module5.html`'s `id="t5"`) as the
  concrete worked example before writing a new one from the template.

Module 8 is Day 5's only module — Day 5 was originally scoped as two modules
(Networking, and Qt/QML), but Qt/QML moved to Day 6 (alongside the Module 10
wrap-up/capstone) so that Day 5 could absorb every remaining non-Qt,
non-capstone topic into one comprehensive day. Do not reintroduce a "Module 9:
Qt/QML" card under Day 5 — Module 9 is now a Day 6 card.

Modules 9 and 10 are still listed on the index as coming soon (both under
Day 6 now). When their lesson pages are created, use this exact structure
(including the required Story Mode tab) and preserve the handbook's
WHY / WHAT / HOW explanation rhythm. Default new modules to the light
`style.css` theme used by `module2.html`/`module3.html` unless told to match
the dark theme instead.

## Boilerplate/TODO Capstone Pattern

`exercise.html` and `capstone-work1.html` are "build it from scratch" style —
a use case description and a homework/hints structure, no starter code with
gaps. `capstone-work2.html` and `capstone-work3.html` are a different style:
a single starter `.cpp` file that already compiles and runs, where each
unfinished feature's function/method body is literally
`throw logic_error("Feature N not implemented: functionName()");`, and
`main()` wraps each call site in `try { ... } catch (const logic_error& e) {
cout << "[TODO] " << e.what(); }` so the file always builds and runs, showing
one clear TODO message per unfinished feature. Use this style when asked for
a "boilerplate," "template with blanks," or "fill in the logic" exercise:

- GIVEN sections (classes/structs the learner doesn't need to write, or a
  skeleton with fields+constructor given but a method body as the TODO) model
  the exact shape every TODO should follow — write at least one GIVEN example
  per pattern the learner needs to repeat.
- Each feature is independent where possible (fixing one doesn't require
  fixing another), but a feature that *calls* another unfinished feature will
  legitimately surface that other feature's TODO message — this is fine and
  informative, not a bug to hide.
- If any requirement can't be expressed as "fill in this function body"
  (e.g. Capstone-Work3's linking requirement — restructuring which files
  things live in and how they're built), state it as a separate CONSTRAINT in
  the source comments and as its own "Constraints — read before you start"
  `task-card` on the page, not as a throwing function. Constraints belong
  before the features task-card, since they're things to know going in, not
  things to fix afterward.
- Page structure: `module-header` → `story-also-grid` mapping each feature to
  the module tab it draws from → WHY block (why this file is boilerplate, not
  finished) → WHAT block (the starter file, GIVEN sections explained, full
  code-card, "Output today" `output-box`, "Target output" `output-box`,
  `quickguide` for VS Code setup, a `callout` prompting the learner to explain
  something about the GIVEN code before starting) → HOW block (constraints
  `task-card`, then features `task-card` with hints, then `submission-card`)
  → `topic-nav` back to the parent module and the course map.
- Before publishing, actually compile and run BOTH the starter (with the
  `throw logic_error` stubs in place, to capture the real "Output today" text)
  and a private reference solution (to capture the real "Target output" text
  and confirm the task is actually solvable as specified) — do not hand-write
  either output box from assumption.

## Practice & Capstones Drawer

Every module page (`module0.html`–`module8.html`) ends with a collapsed
`<details class="practice-drawer">` (native disclosure element, no JS needed)
right after the closing `</div>` of `.tabs-wrap` and before `</main>` (in
`module3.html`'s differently-structured page, right before its
`<footer class="page-footer">` instead), linking to all four practice pages:
`exercise.html`, `capstone-work1.html`, `capstone-work2.html`,
`capstone-work3.html`. This exists because capstones were previously only
discoverable from `index.html`'s sidebar — every module page now surfaces
them too. When a new capstone page is added, add its `<a class="practice-link">`
to this drawer in all 9 module files, plus the sidebar-resources `<li>` in
`exercise.html`, `capstone-work1.html`, `capstone-work2.html`, and
`index.html` (the newest capstone page's own sidebar lists all the others,
bolding itself — copy `capstone-work2.html`'s sidebar as the template for a
new capstone page's sidebar). The drawer's CSS lives in `style.css` under
`/* PRACTICE & CAPSTONES DRAWER */` for the light theme; `module0.html` and
`module1.html` duplicate it inline with hardcoded hex colors (not `var()`
tokens) because their inline dark-theme `<style>` block loads *before*
`style.css` on those two pages, and since both blocks declare a `:root` with
several identically-named custom properties (`--bg`, `--border`, `--text`,
`--surface`, `--radius`, etc.), the later-loaded `style.css` `:root` wins the
cascade for those shared names — hardcoded colors sidestep that risk entirely
for any new component added to those two files. This collision is pre-existing
and out of scope to fix unless asked; be aware of it before adding any new
shared-name CSS variable usage to `module0.html`/`module1.html`.
