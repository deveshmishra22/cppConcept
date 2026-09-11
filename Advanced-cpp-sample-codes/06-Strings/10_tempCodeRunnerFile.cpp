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