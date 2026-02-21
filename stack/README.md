# **Stack**

A LIFO (Last-In-First-Out) container adapter with configurable underlying storage (vector, deque, or list).

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/stack.h>

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    stack s;
    stack_attr_t attr = { .size = sizeof(int), .print = int_print };
    
    stack_init(&s, CS_STACK_DEFAULT, attr);  // Uses deque backend
    
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        stack_push(&s, &values[i]);
    }
    
    int *top = (int*)stack_top(&s);  // Returns 30 (last pushed)
    stack_pop(&s);                    // Removes 30
    
    stack_free(&s);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `stack_init(stack *s, stack_type type, stack_attr_t attr)` | `CS_SUCCESS` on success, `CS_MEM` on failure, `CS_SIZE` if invalid size | Initializes with chosen backend: `CS_STACK_ARRAY`, `CS_STACK_DEQUE`, or `CS_STACK_LIST`. |
| `stack_push(stack *s, const void *data)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Pushes element onto top. Delegates to underlying container's push_back. |
| `stack_pop(stack *s)` | `CS_SUCCESS` on success, `CS_EMPTY` if empty | Removes top element. |
| `stack_top(const stack *s)` | `void*` pointer to top element, `NULL` if empty | Returns reference to top element without removing. |
| `stack_size(const stack *s)` | `int` element count, `-1` on error | Returns number of elements. |
| `stack_empty(const stack *s)` | `1` if empty, `0` otherwise, `-1` on error | Checks if stack has no elements. |
| `stack_clear(stack *s)` | `void` | Removes all elements. |
| `stack_free(void *v_s)` | `void` | Frees stack and underlying container. |
| `stack_print(FILE *stream, void *v_s)` | `void` | Prints all elements (bottom to top). |

### Backend Types

| Type | Constant | Characteristics |
|------|----------|-----------------|
| **Vector** | `CS_STACK_ARRAY` | O(1) amortized push, cache-friendly, may reallocate |
| **Deque** | `CS_STACK_DEQUE` | O(1) push, no reallocation, default choice |
| **List** | `CS_STACK_LIST` | O(1) push, no contiguous memory requirement |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct stack {
    void *container;  // Pointer to underlying container
    stack_type type;  // Backend type enum
} stack;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `stack` struct | 16 | Pointer + type enum with padding |
| Container overhead | varies | Depends on backend choice |

### Per-Backend Memory

| Backend | Header Size | Per-Element Overhead |
|---------|-------------|---------------------|
| Vector | 64 bytes | 0 (contiguous) |
| Deque | ~80 bytes | ~0 (blocked) |
| List | ~24 bytes | 24 bytes (prev/next pointers) |

### Scaling Behavior

- **Space Complexity**: $O(n)$ for all backends
- **Time Complexity**: $O(1)$ for push/pop (amortized for vector)
- **Memory Layout**: Vector/Deque are cache-friendly; List has pointer overhead

> [!NOTE]
> Use `CS_STACK_DEQUE` (default) for best general performance. Use `CS_STACK_LIST` when avoiding reallocation is critical.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link ALL potential backends
gcc myprogram.c -Iinclude -Llib -lstack -lvector -ldeque -llist -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> The stack wrapper includes headers for all backends. Link all container libraries even if using only one type at runtime.
