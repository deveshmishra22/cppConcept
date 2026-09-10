# Zero-Allocation Embedded Template Queue Implementation
## C++ Style
This production-ready queue container provides vector-like safety guarantees without dynamic memory, heap allocation, or external sanitizer overhead. It enforces fixed capacity at compile time and leverages hardware breakpoints for bounds violations in debug builds.

```c++

#ifndef STATIC_QUEUE_HPP
#ifndef STATIC_QUEUE_HPP
#define STATIC_QUEUE_HPP

#include <cstddef>
#include <utility>

template <typename T, std::size_t N>
class StaticQueue {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;

private:
    T         data_[N];
    size_type head_  = 0;
    size_type tail_  = 0;
    size_type count_ = 0;

    inline void debug_trap() const {
#ifdef DEBUG
        __asm__ volatile("BKPT #0");
#endif
    }

public:
    constexpr StaticQueue() noexcept = default;

    [[nodiscard]] bool empty() const noexcept { return count_ == 0; }
    [[nodiscard]] bool full()  const noexcept { return count_ == N; }
    [[nodiscard]] size_type size() const noexcept { return count_; }
    [[nodiscard]] constexpr size_type capacity() const noexcept { return N; }

    bool push(const T& value) {
        if (full()) {
            debug_trap();
            return false;
        }
        data_[tail_] = value;
        tail_ = (tail_ + 1) % N;
        ++count_;
        return true;
    }

    bool push(T&& value) {
        if (full()) {
            debug_trap();
            return false;
        }
        data_[tail_] = std::move(value);
        tail_ = (tail_ + 1) % N;
        ++count_;
        return true;
    }

    bool pop() noexcept {
        if (empty()) {
            debug_trap();
            return false;
        }
        head_ = (head_ + 1) % N;
        --count_;
        return true;
    }

    reference front() {
        if (empty()) { debug_trap(); }
        return data_[head_];
    }

    const_reference front() const {
        if (empty()) { debug_trap(); }
        return data_[head_];
    }

    reference operator[](size_type index) {
        if (index >= count_) { debug_trap(); }
        return data_[(head_ + index) % N];
    }

    const_reference operator[](size_type index) const {
        if (index >= count_) { debug_trap(); }
        return data_[(head_ + index) % N];
    }
};

#endif // STATIC_QUEUE_HPP
```
## C Style
This header-only C implementation uses macro generation to achieve compile-time type safety, zero dynamic memory allocation, and ARM Cortex-R5 debug trapping (BKPT #0).

```c
#ifndef STATIC_QUEUE_H
#define STATIC_QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Debug trap trigger for ARM Cortex-R5 debugger
#ifdef DEBUG
    #define QUEUE_TRAP() __asm__ volatile("BKPT #0")
#else
    #define QUEUE_TRAP() ((void)0)
#endif

/**
 * Macro generator for a type-safe, zero-allocation static queue.
 *
 * @param TYPE      Data type stored in the queue (e.g., uint8_t, CommandPacket)
 * @param NAME      Prefix name for the generated struct and inline functions
 * @param CAPACITY  Fixed compile-time capacity limit
 */
#define DEFINE_STATIC_QUEUE(TYPE, NAME, CAPACITY)                             \
typedef struct {                                                              \
    TYPE buffer[CAPACITY];                                                    \
    size_t head;                                                              \
    size_t tail;                                                              \
    size_t count;                                                             \
} NAME;                                                                       \
                                                                              \
static inline void NAME##_init(NAME *q) {                                     \
    q->head = 0;                                                              \
    q->tail = 0;                                                              \
    q->count = 0;                                                             \
}                                                                             \
                                                                              \
static inline bool NAME##_empty(const NAME *q) {                              \
    return q->count == 0;                                                     \
}                                                                             \
                                                                              \
static inline bool NAME##_full(const NAME *q) {                               \
    return q->count == CAPACITY;                                              \
}                                                                             \
                                                                              \
static inline size_t NAME##_size(const NAME *q) {                             \
    return q->count;                                                          \
}                                                                             \
                                                                              \
static inline bool NAME##_push(NAME *q, TYPE item) {                          \
    if (NAME##_full(q)) {                                                     \
        QUEUE_TRAP();                                                         \
        return false;                                                         \
    }                                                                         \
    q->buffer[q->tail] = item;                                                \
    q->tail = (q->tail + 1) % CAPACITY;                                       \
    q->count++;                                                               \
    return true;                                                              \
}                                                                             \
                                                                              \
static inline bool NAME##_pop(NAME *q, TYPE *out_item) {                      \
    if (NAME##_empty(q)) {                                                    \
        QUEUE_TRAP();                                                         \
        return false;                                                         \
    }                                                                         \
    if (out_item != NULL) {                                                   \
        *out_item = q->buffer[q->head];                                       \
    }                                                                         \
    q->head = (q->head + 1) % CAPACITY;                                       \
    q->count--;                                                               \
    return true;                                                              \
}                                                                             \
                                                                              \
static inline TYPE* NAME##_peek(NAME *q) {                                    \
    if (NAME##_empty(q)) {                                                    \
        QUEUE_TRAP();                                                         \
        return NULL;                                                          \
    }                                                                         \
    return &q->buffer[q->head];                                               \
}

#endif // STATIC_QUEUE_H
```
## usage in C

```c
#include "static_queue.h"

typedef struct {
    uint8_t header[4];
    uint8_t payload[10];
    uint32_t crc;
} CommandPacket;

// Generates a type-safe PacketQueue struct and functions with 16-item capacity
DEFINE_STATIC_QUEUE(CommandPacket, PacketQueue, 16)

void process_queue(void) {
    PacketQueue q;
    PacketQueue_init(&q);

    CommandPacket tx_pkt = { .crc = 0x12345678 };

    // PUSH: Halts at BKPT #0 in Debug mode if queue overflows
    if (PacketQueue_push(&q, tx_pkt)) {
        // Enqueued successfully
    }

    // PEEK & POP
    CommandPacket rx_pkt;
    if (PacketQueue_pop(&q, &rx_pkt)) {
        // Dequeued safely without dynamic pointers
    }
}
```

## Key Implementation Features
* Zero Dynamic Allocation: Buffer resides inline inside the struct (allocated on Stack or BSS segment).
* Type-Safe in Pure C: Using DEFINE_STATIC_QUEUE prevents void* type casting bugs at compile time.
* Hardware Fault Integration: Underflow or overflow attempts trigger BKPT #0 in -DDEBUG builds, while returning false gracefully in release mode.

