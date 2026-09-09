# Move Semantics 
Before C++11, passing or returning large objects forced the compiler to duplicate heap memory even when the original object was a temporary about to 
be destroyed. Move semantics eliminates this redundant overhead by allowing a new object to "steal" pointers directly from an expiring source object.
## The Core Intuition: Copying vs. Moving
* Copying: You buy a brand new physical binder, print copies of all 500 pages from a colleague's binder, and put them in yours. 
(Expensive, slow heap allocations).
* Moving: Your colleague hands you their entire binder directly. They now hold an empty hand (nullptr), you hold all 500 pages,
and zero pages were printed.

Lets first understand what is LValue and RValue
Concept,Definition,Example,Can you take its memory address?
Lvalue,Persistent object with a named location in memory.,int x = 10;,✅ Yes (&x)
Lvalue,Persistent object with a named location in memory.,"std::string s = ""text"";",✅ Yes (&s)
Rvalue,Temporary value/literal expiring at the end of the line.,42,❌ No (&42 is invalid)
Rvalue,Temporary value/literal expiring at the end of the line.,x + 5,❌ No (&(x + 5) is invalid)
Rvalue,Temporary value/literal expiring at the end of the line.,"std::string(""temp"")","❌ No (&std::string(""temp"") is invalid)"
