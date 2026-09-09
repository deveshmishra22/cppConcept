
#pragma GCC diagnostic warning "-Wsign-conversion"
#pragma GCC diagnostic warning "-Wsign-compare"

# Directive Breakdown
* 1. #pragma GCC: Tells the preprocessor this rule is specifically for the GCC/Clang compiler family.
* 2. diagnostic: Specifies that you are modifying compiler diagnostic behavior (warnings and errors).
* 3. warning: Sets the target behavior level (can also be error to halt builds, or ignored to suppress warnings).

"-Wsign-conversion": The exact flag name in double quotes.
