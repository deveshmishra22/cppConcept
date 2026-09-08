** constexpr vs consteval vs #define **

| Feature | `#define` (Legacy C) | `constexpr` (C++11) | `consteval` (C++20) |
| :--- | :--- | :--- | :--- |
| **Type Safety** | None (raw text substitution) | Full C++ type safety | Full C++ type safety |
| **Execution Timing** | Preprocessor phase (before compilation) | Compile-time or Runtime | Compile-time ONLY |
| **Scope Rules** | Ignores namespaces and class scopes | Respects scope & access controls | Respects scope & access controls |
| **Debugging** | Difficult (expands before compile) | Easy (normal C++ symbol) | Easy (normal C++ symbol) |
| **Runtime Fallback** | N/A | Yes (if arguments are non-const) | No (compiler error if non-const) |
