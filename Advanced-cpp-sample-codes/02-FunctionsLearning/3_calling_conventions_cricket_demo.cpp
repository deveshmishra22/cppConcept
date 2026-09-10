// Calling conventions, explained through cricket.
//
// The point of this file is NOT "here are four ways to pass a parameter,
// pick your favorite." Each one below has exactly one concrete situation
// where it is the ONLY correct tool -- the other options either don't
// compile, silently do the wrong thing, or are outright undefined
// behavior. Read each PART as "why can't I use anything else here?"
//
// PART 1: call by value      -- function parameters
// PART 2: call by reference  -- function parameters
// PART 3: call by pointer    -- function parameters
// PART 4: smart pointers     -- NOT a calling convention (theory only)
// PART 5: lambda capture by value vs by reference -- a DIFFERENT axis,
//         nothing to do with how you pass arguments to a normal function
//
// Build: g++ -std=c++17 3_calling_conventions_cricket_demo.cpp -o cricket
// Run:   ./cricket

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <algorithm>
using namespace std;

struct Player {
    string name;
    int runs;
};

void printPlayer(const Player& p) {
    cout << "  " << p.name << ": " << p.runs << " runs\n";
}

// ============================================================
// PART 1: Call by value
// Concrete requirement: I need a working copy I can scribble on
// freely, and the CALLER's original must be 100% untouched -- not
// "untouched by convention," untouched because it is structurally
// impossible for this function to reach the original at all.
// ============================================================
int previewWithBonus(int runs) { // <-- runs is a fresh copy, local to this call
    runs += 6; // freely mutated -- this is scratch space, not the caller's data
    return runs;
}
// Why not by reference? int& runs would let += 6 leak straight into the
// caller's real variable -- exactly what we're trying to avoid.
// Why not by const reference? const int& runs can't be mutated at all,
// so runs += 6 wouldn't even compile.
// Call by value is the only option that is BOTH freely mutable AND
// guaranteed side-effect-free for the caller.

void part1_callByValue() {
    cout << "==================== PART 1: Call by value ====================\n";
    Player virat{"Virat", 74};
    cout << "Before preview:\n";
    printPlayer(virat);

    int preview = previewWithBonus(virat.runs);
    cout << "Previewed score with a bonus six: " << preview << "\n";
    cout << "After preview (must be unchanged):\n";
    printPlayer(virat);
}

// ============================================================
// PART 2: Call by reference
// Concrete requirement: there is exactly ONE real object involved,
// it is guaranteed to exist (never "no scoreboard"), and I need to
// change THAT object itself, not a copy of it.
// ============================================================
void addRuns(int& totalScore, int runsThisBall) { // totalScore IS the caller's variable
    totalScore += runsThisBall;
}

void swapBatsmen(Player& striker, Player& nonStriker) { // must be the real two objects
    Player temp = striker;
    striker = nonStriker;
    nonStriker = temp;
}
// Why not by value? addRuns(int totalScore, ...) would update a copy that
// is thrown away the moment the function returns -- the real scoreboard
// in main() would never move.
// Why not by pointer? A pointer could be null (no scoreboard at all) or
// reseated mid-function to point somewhere else -- neither is a real
// possibility here, so a reference's "always valid, never reseated"
// guarantee is a better fit, and the call site stays clean:
// addRuns(matchScore, 4) instead of addRuns(&matchScore, 4).

void part2_callByReference() {
    cout << "\n==================== PART 2: Call by reference ====================\n";
    int matchScore = 0;
    addRuns(matchScore, 4);
    addRuns(matchScore, 1); // odd run -> batsmen cross ends
    addRuns(matchScore, 6);
    cout << "Live match score after 3 balls: " << matchScore << "\n";

    Player striker{"Rohit", 40};
    Player nonStriker{"Rahul", 25};
    cout << "Before swap -> striker: " << striker.name << ", non-striker: " << nonStriker.name << "\n";
    swapBatsmen(striker, nonStriker);
    cout << "After swap  -> striker: " << striker.name << ", non-striker: " << nonStriker.name << "\n";
}

// ============================================================
// PART 3: Call by pointer
// Concrete requirement #1: the answer might legitimately be "nobody" --
// a reference can NEVER be null, so if "not found" is a real outcome,
// only a pointer (or std::optional) can represent it.
// Concrete requirement #2: I need to point at different objects over
// time -- a reference is bound for life the moment it's created; a
// pointer can be reseated.
// ============================================================
Player* findPlayerByName(vector<Player>& team, const string& name) {
    for (auto& p : team) {
        if (p.name == name) return &p; // found -> address of the real player
    }
    return nullptr; // not found -- this line is the entire reason to use a pointer here
}

void part3_callByPointer() {
    cout << "\n==================== PART 3: Call by pointer ====================\n";
    vector<Player> team{{"Bumrah", 8}, {"Kohli", 74}, {"Jadeja", 31}};

    Player* found = findPlayerByName(team, "Kohli");
    if (found) { cout << "Found: "; printPlayer(*found); }
    else cout << "Not found.\n";

    Player* missing = findPlayerByName(team, "Sachin");
    if (missing) { cout << "Found: "; printPlayer(*missing); }
    else cout << "Searched for Sachin -> nullptr (he isn't in this team). A reference could not express this.\n";

    // Requirement #2: reseating. currentBatsman points at different real
    // Player objects as overs go by -- a reference could not do this,
    // it would have stayed bound to team[0] forever.
    Player* currentBatsman = &team[0];
    cout << "Over 1, on strike: " << currentBatsman->name << "\n";
    currentBatsman = &team[1]; // reseated -- now points at a DIFFERENT object
    cout << "Over 2, on strike: " << currentBatsman->name << "\n";
}

// ============================================================
// PART 4 (THEORY ONLY): Are smart pointers a calling convention?
// No. Smart pointers answer a completely different question: "who OWNS
// this heap object, and when should it be destroyed?" That's orthogonal
// to "how do I hand an argument to a function." Once you already have a
// shared_ptr<Player> or unique_ptr<Player>, you still pick one of the
// THREE mechanisms above to pass it:
//
//   void useOnly(const shared_ptr<Player>& p)   // by reference (const)
//       -- most common. Callee only USES the player during the call and
//       does not need to be an owner. Does not touch the refcount at all.
//
//   void becomeCoOwner(shared_ptr<Player> p)    // by value
//       -- callee is meant to keep a copy of ownership around after the
//       call returns (e.g. storing it in a member or a container). This
//       bumps the reference count on entry, which has a real cost.
//
//   void takeOwnershipAway(unique_ptr<Player> p) // by value, forces move()
//       -- caller permanently gives up the player; only makes sense for
//       unique_ptr, since copying it is disabled by design.
//
//   void justPeek(Player* p)                     // raw pointer/reference
//   void justPeek(Player& p)                     // to the MANAGED OBJECT
//       -- the overwhelmingly common real-world case. The callee wants to
//       read or modify the Player, has zero interest in who owns it or
//       when it gets destroyed. Get this raw view with smartPtr.get()
//       or *smartPtr, never by taking the smart pointer as a parameter.
//
// Rule of thumb: if a function does not need to change WHO OWNS the
// object or HOW LONG it lives, it should never take a smart pointer
// parameter at all -- take a plain reference or pointer to the object
// it manages.
void justPeek(const Player& p) { // callee only looks -- no ownership involved
    cout << "  Peeking (no ownership change): " << p.name << " has " << p.runs << " runs\n";
}

void part4_smartPointerTheory() {
    cout << "\n==================== PART 4: Smart pointers (theory, not a 4th mechanism) ====================\n";
    shared_ptr<Player> managed = make_shared<Player>(Player{"Dhoni", 50});
    cout << "use_count before calling justPeek: " << managed.use_count() << "\n";
    justPeek(*managed); // pass the MANAGED OBJECT by reference -- refcount untouched
    cout << "use_count after calling justPeek:  " << managed.use_count() << "\n";
    cout << "The count never moved -- justPeek never had, and never needed, any ownership stake.\n";
}

// ============================================================
// PART 5: Lambda capture by value vs capture by reference
// A DIFFERENT axis entirely -- this is not about passing arguments to
// a function, it's about what a lambda REMEMBERS from its surrounding
// scope, and for how long.
// ============================================================
void part5_lambdaCaptures() {
    cout << "\n==================== PART 5: Lambda capture by value vs by reference ====================\n";

    cout << "-- 5a. Capture by VALUE: freeze a snapshot for later --\n";
    vector<function<void()>> replays;
    for (int over = 1; over <= 3; ++over) {
        int scoreAtThisMoment = over * 6; // a new local variable every loop iteration
        // [scoreAtThisMoment, over] copies BOTH into the lambda right now.
        replays.push_back([scoreAtThisMoment, over]() {
            cout << "  Replay: after over " << over << ", score was " << scoreAtThisMoment << "\n";
        });
        // scoreAtThisMoment and over go out of scope at the end of this
        // iteration. If we had captured by REFERENCE instead, every
        // stored lambda would be holding a reference to memory that no
        // longer exists by the time we call it below -- a dangling
        // reference, not just "wrong output." Capture by value is the
        // only correct choice here.
    }
    cout << "Calling the stored replays after the loop has finished:\n";
    for (auto& replay : replays) replay();

    cout << "\n-- 5b. Capture by REFERENCE: write back into the real outer variable --\n";
    vector<int> ballsThisOver = {1, 4, 6, 0, 6, 2};

    int totalSixesWrong = 0;
    for_each(ballsThisOver.begin(), ballsThisOver.end(), [totalSixesWrong](int runs) mutable {
        if (runs == 6) totalSixesWrong++; // only mutates the lambda's OWN private copy
    });
    cout << "Captured by VALUE     -> totalSixesWrong stays: " << totalSixesWrong
         << " (the real variable was never touched)\n";

    int totalSixesRight = 0;
    for_each(ballsThisOver.begin(), ballsThisOver.end(), [&totalSixesRight](int runs) {
        if (runs == 6) totalSixesRight++; // mutates the REAL outer variable
    });
    cout << "Captured by REFERENCE -> totalSixesRight is:  " << totalSixesRight
         << " (correctly accumulated back into main's variable)\n";
}

int main() {
    part1_callByValue();
    part2_callByReference();
    part3_callByPointer();
    part4_smartPointerTheory();
    part5_lambdaCaptures();

    cout << "\n==================== Summary ====================\n";
    cout << "Call by value      -> freely-mutable local scratch copy, zero effect on the caller.\n";
    cout << "Call by reference  -> exactly one real object, always valid, mutate it directly.\n";
    cout << "Call by pointer    -> the answer might be nobody (nullptr), or you must reseat it.\n";
    cout << "Smart pointer      -> not a calling convention -- it's who OWNS the object; pass the\n";
    cout << "                      managed object by reference/pointer unless ownership must change.\n";
    cout << "Capture by value   -> freeze a snapshot now, safe to use long after the original is gone.\n";
    cout << "Capture by reference -> read or write the live, current value of the real outer variable.\n";
    return 0;
}
