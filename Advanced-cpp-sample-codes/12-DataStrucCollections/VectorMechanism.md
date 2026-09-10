# 1. C++ std::vector Mechanics & Memory Layout
Memory Footprint on Instantiation
Instantiating std::vector<T> v; allocates zero bytes of dynamic heap memory.
The object itself resides on the stack, consuming a fixed size (12 bytes on 32-bit ARM Cortex-R5; 24 bytes on 64-bit systems) to store three raw management pointers:
* _M_start: Points to the first allocated element.
* _M_finish: Points past the last active initialized element (size = _M_finish - _M_start).
* _M_end_of_storage: Points past the allocated capacity boundary (capacity = _M_end_of_storage - _M_start).

Stack Memory (12 bytes on 32-bit ARM):
[ _M_start ]  [ _M_finish ]  [ _M_end_of_storage ]
      |             |                  |
      v             v                  v
Heap: [ Element 0 ] [ Element 1 ] ... [ Capacity Limit ]

## Contiguous Memory & Fragmentation Risks
Vectors require single contiguous memory blocks. In embedded systems, this introduces two major hazards:Heap Fragmentation: Free memory may exist in total, but if no single unbroken block matches the requested size, allocation fails (std::bad_alloc).Reallocation Peak Memory Trap: During capacity doubling ($2\times$), the vector allocates the new buffer before destroying the old one. Expanding from $N$ to $2N$ capacity temporarily requires $3N$ total capacity in RAM.

# 2. AddressSanitizer (ASan) Internals
* Shadow Memory & Memory Poisoning
ASan maps 1/8th of virtual memory space into Shadow Memory to track byte validity. Memory marked as poisoned triggers an immediate execution halt if read or written to by CPU instructions.

* Vector Annotations (GROW / GREW)
When a vector allocates spare capacity, the OS views the entire block as valid RAM, but the vector only considers elements up to _M_finish as valid. To catch out-of-bounds pointer reads/writes inside allocated capacity, standard libraries use ASan annotations:

```c++
_GLIBCXX_ASAN_ANNOTATE_GROW(1); // Unpoisons 1 slot at _M_finish in shadow memory
_Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
++this->_M_impl._M_finish;
_GLIBCXX_ASAN_ANNOTATE_GREW(1); // Finalizes slot unpoisoning
```

Why Vector Pointers Blindside Hardware
Internal vector bounds checks (_M_finish) only protect class methods like .at(). Once raw pointers decay (T* ptr = v.data()), compiler instructions access RAM directly via load/store instructions (LDR/STR), bypassing vector variables. ASan shadow memory intercepts these raw instruction accesses at runtime.

Debug Safety Macro Pattern
Handles array access safety with hardware breakpoint triggers (BKPT #0) in debug mode, collapsing to raw indexing in release builds:
```c
#ifdef DEBUG
    #define HARDWARE_BREAK() __asm__ volatile("BKPT #0")

    #define ARRAY_WRITE(arr, idx, val) do { \
        if ((idx) >= (sizeof(arr) / sizeof((arr)[0]))) { \
            HARDWARE_BREAK(); \
        } \
        (arr)[idx] = (val); \
    } while(0)

    #define ARRAY_READ(arr, idx) ({ \
        if ((idx) >= (sizeof(arr) / sizeof((arr)[0]))) { \
            HARDWARE_BREAK(); \
        } \
        (arr)[idx]; \
    })
#else
    #define ARRAY_WRITE(arr, idx, val) ((arr)[idx] = (val))
    #define ARRAY_READ(arr, idx)       ((arr)[idx])
#endif
```

# Decayed Pointer Protection (Slice Pattern)
When raw arrays decay to pointers, sizeof evaluates to pointer width (4 bytes). Pass bounds context explicitly using a slice container:
```c
typedef struct {
    uint8_t *data;
    size_t   len;
} BufferSlice;

#define MAKE_SLICE(array) ((BufferSlice){ .data = (array), .len = sizeof(array) })

#ifdef DEBUG
    #define SLICE_READ(slice, idx) ({ \
        if ((idx) >= (slice).len) { __asm__ volatile("BKPT #0"); } \
        (slice).data[idx]; \
    })
#else
    #define SLICE_READ(slice, idx) ((slice).data[idx])
#endif
```
