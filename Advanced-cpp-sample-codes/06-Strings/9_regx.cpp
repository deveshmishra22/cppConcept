#include<iostream>  
#include<regex>
#include<string>
using namespace std;

int main() {
    string s = "My phone: ab356-456-7890";
    regex re("(\\d{3})-(\\d{3})-(\\d{4})");
    string replaced = regex_replace(s, re, "($1) ($2-$3)");
    cout << replaced << '\n'; // formats phone number

    string text = "The quick brown fox jumps over the lazy dog. The quick blue hare.";

    // Define a regex pattern to find the word 'quick' followed by a color and an animal
    regex pattern(R"(quick (\w+) (\w+))");

    // Create a smatch object to hold the results
    smatch matches;

    // Search for the pattern in the text
    if (regex_search(text, matches, pattern)) {
        cout << "Full match: " << matches[0] << endl; // Full matched string
        cout << "Color: " << matches[1] << endl;      // First captured group (color)
        cout << "Animal: " << matches[2] << endl;     // Second captured group (animal)
    } else {
        cout << "No match found." << endl;
    }

    // Replace 'quick' with 'swift' in the text
    string replacedText = regex_replace(text, regex("quick"), "swift");
    cout << "After replacement: " << replacedText << endl;

    // Find all occurrences of the pattern
    auto words_begin = sregex_iterator(text.begin(), text.end(), pattern);
    auto words_end = sregex_iterator();
    cout << "All matches found:" << endl;
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        cout << "Full match: " << match[0] << ", Color: " << match[1] << ", Animal: " << match[2] << endl;
    }

    // Validate an email address using regex
    string email = "raushan@rrskillverse.com";
    regex emailPattern(R"((\w+)(\.?)(\w*)@(\w+)\.(\w+))");
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