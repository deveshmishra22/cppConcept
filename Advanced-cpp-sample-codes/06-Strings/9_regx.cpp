#include<iostream>  
#include<regex>
#include<string>
using namespace std;

int main() {
    string s = "My phone: ab36789-456-7890";

    // regex(pattern): compiles `pattern` into a matcher object. `\\d` in a normal
    // (non-raw) C++ string literal is the escape for a single backslash + 'd',
    // which the regex engine then reads as \d = "one digit". {3}/{4} are EXACT
    // repeat counts (not "up to N"), and (...) is a capturing group, numbered
    // 1, 2, 3 left to right by opening parenthesis.
    // Pattern shape required, contiguously, somewhere in the string:
    //   3 digits - 3 digits - 4 digits   e.g. "356-456-7890"
    // NOTE: {3}/{4} are exact, not fuzzy. If the digit run around the hyphens
    // is shaped e.g. 2-3-4 instead of 3-3-4 (like "12-234-7890"), there is NO
    // starting position anywhere that satisfies all 5 pieces (3 digits, '-',
    // 3 digits, '-', 4 digits) at once, so the match fails everywhere and
    // regex_replace below returns the string completely UNCHANGED (not even
    // the digits get touched) — it's all-or-nothing per match, never partial.
    regex re("(\\d{3})-(\\d{3})-(\\d{4})");
// regx pattern to have strict 3 digit-3digit-4digit match else return same s
    regex re1("\\b(\\d{3})-(\\d{3})-(\\d{4})");


    // regex_replace(input, regex, format): scans `input` for every non-overlapping
    // match of `regex`. For each match found, it expands `format` — where $1, $2, $3
    // refer back to that match's capture groups 1/2/3 — and splices the expansion in
    // place of the matched text. Text that doesn't match (like "My phone: ab") is
    // copied through untouched. Returns a NEW string; `s` itself is not modified.
    // If zero matches are found anywhere, the returned string equals `s` exactly.
    string replaced = regex_replace(s, re1, "($1) ($2-$3)");
    cout << replaced << '\n'; // formats phone number

    string text = "The quick brown fox jumps over the lazy dog. The quick blue hare.";

    // R"(...)" is a raw string literal: backslashes are literal, so \w does not
    // need to be written \\w. Pattern: literal word "quick", a space, then group 1
    // = one-or-more word chars (\w+), a space, then group 2 = one-or-more word chars.
    // Define a regex pattern to find the word 'quick' followed by a color and an animal
    regex pattern(R"(quick (\w+) (\w+))");

    // smatch: a container type that holds the result of matching a regex against a
    // std::string — the full matched text plus every capture group. Declared empty
    // here; regex_search below fills it in.
    // Create a smatch object to hold the results
    smatch matches;

    // regex_search(input, matchResults, regex): searches `input` for the FIRST
    // occurrence of `regex` ANYWHERE in the string (unlike regex_match, it does not
    // need the whole string to match). Returns true/false; on success, fills
    // `matchResults` with matches[0] = full match, matches[1..N] = capture groups.
    // Search for the pattern in the text
    if (regex_search(text, matches, pattern)) {
        cout << "Full match: " << matches[0] << endl; // Full matched string
        cout << "Color: " << matches[1] << endl;      // First captured group (color)
        cout << "Animal: " << matches[2] << endl;     // Second captured group (animal)
    } else {
        cout << "No match found." << endl;
    }

    // regex("quick"): a plain literal pattern (no metacharacters) — matches the exact
    // text "quick". regex_replace here has no $N in its format string, so it just
    // substitutes "swift" for every occurrence found (global replace by default).
    // Replace 'quick' with 'swift' in the text
    string replacedText = regex_replace(text, regex("quick"), "swift");
    cout << "After replacement: " << replacedText << endl;

    // sregex_iterator(begin, end, regex): an iterator over ALL matches of `regex`
    // within [begin, end) — constructing it positions it at the first match.
    // Dereferencing it (*i) yields an smatch for the current match; ++i advances
    // to the next match in the string.
    // Find all occurrences of the pattern
    auto words_begin = sregex_iterator(text.begin(), text.end(), pattern);

    // sregex_iterator() with no arguments is the special "end" sentinel — it does
    // not point at a match, it just represents "no more matches", used to detect
    // when the loop below should stop.
    auto words_end = sregex_iterator();
    cout << "All matches found:" << endl;
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        cout << "Full match: " << match[0] << ", Color: " << match[1] << ", Animal: " << match[2] << endl;
    }

    // Validate an email address using regex
    string email = "raushan@rrskillverse.com";

    // Pattern (\w+)(\.?)(\w*)@(\w+)\.(\w+):
    //   group1 \w+  -> start of username (1+ word chars)
    //   group2 \.?  -> an optional literal dot (e.g. "first.last")
    //   group3 \w*  -> rest of username (0+ word chars)
    //   literal @
    //   group4 \w+  -> domain name
    //   literal \.  -> an escaped, literal dot
    //   group5 \w+  -> top-level domain (e.g. "com")
    regex emailPattern(R"((\w+)(\.?)(\w*)@(\w+)\.(\w+))");

    // regex_match(input, regex): unlike regex_search, this requires the ENTIRE
    // `input` string to match `regex` from start to end (anchored both ends
    // implicitly) — a partial match anywhere inside does not count as success.
    // Returns true/false only; no smatch is needed since we only want the verdict.
    if (regex_match(email, emailPattern)) {
        cout << email << " is a valid email address." << endl;
    } else {
        cout << email << " is not a valid email address." << endl;
    }

    return 0;
}


// Let's understand what is Regex in C++17 and how to use it with examples. 
// Regular expressions (regex) are a powerful tool for pattern matching and text manipulation. 
// In C++, the `<regex>` library provides support for regex operations, allowing you to search, match, 
// and replace patterns in strings.

// For example, in the code above, we demonstrate various regex operations:
// 1. We use `regex_replace` to format a phone number in a specific pattern
// 2. We use `regex_search` to find a specific pattern in a string and extract captured groups
// 3. We use `regex_replace` again to replace occurrences of a specific word in a string
// 4. We use `sregex_iterator` to find all occurrences of a pattern in a string
// 5. We use `regex_match` to validate an email address against a regex pattern

// How to write regex in C++17:
// 1. Include the `<regex>` header file
// 2. Create a `regex` object with the desired pattern
// 3. Use functions like `regex_search`, `regex_match`, and `regex_replace` to perform operations on strings

// How to write patterns in regex:
// - Use `\d` to match digits, `\w` to match word characters
// - Use quantifiers like `{n}` to specify the number of occurrences
// - Use parentheses `()` to create capturing groups
// - Use `|` for alternation (OR) and `^` and `$` for start and end of string anchors
// for example, the pattern `(\d{3})-(\d{3})-(\d{4})` matches a phone number format like "123-456-7890" and 
// captures the three groups of digits.
// for email validation, the pattern `(\w+)(\.?)(\w*)@(\w+)\.(\w+)` matches a typical email format and 
// captures the username, domain, and top-level domain.
// if domain is fixed to "rrskillverse.com", then the pattern can be simplified to `(\w+)(\.?)(\w*)@rrskillverse\.com` to 
// match only emails from that domain.