# **Queue**

A FIFO (First-In-First-Out) container adapter with configurable underlying storage (deque or list).

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/queue.h>

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    queue q;
    queue_attr attr = { .size = sizeof(int), .print = int_print };
    
    queue_init(&q, attr, CS_QUEUE_DEFAULT);  // Uses deque backend
    
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        queue_push(&q, &values[i]);
    }
    
    int *front = (int*)queue_front(&q);  // Returns 10 (first pushed)
    int *back = (int*)queue_back(&q);    // Returns 30 (last pushed)
    
    queue_pop(&q);  // Removes 10
    
    queue_free(&q);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `queue_init(queue *q, queue_attr attr, queue_type type)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes with backend: `CS_QUEUE_DEQUE` or `CS_QUEUE_LIST`. |
| `queue_push(queue *q, void *data)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Adds element to back of queue. |
| `queue_pop(queue *q)` | `CS_SUCCESS` on success, `CS_EMPTY` if empty | Removes element from front. |
| `queue_front(queue *q)` | `void*` pointer to front element, `NULL` if empty | Returns reference to front element. |
| `queue_back(queue *q)` | `void*` pointer to back element, `NULL` if empty | Returns reference to back element. |
| `queue_size(queue q)` | `int` element count | Returns number of elements. |
| `queue_empty(queue q)` | `1` if empty, `0` otherwise | Checks if queue has no elements. |
| `queue_swap(queue *q1, queue *q2)` | `void` | Swaps contents of two queues. |
| `queue_clear(queue *q)` | `void` | Removes all elements. |
| `queue_free(void *v_q)` | `void` | Frees queue and underlying container. |
| `queue_print(FILE *stream, void *v_q)` | `void` | Prints all elements (front to back). |

### Backend Types

| Type | Constant | Characteristics |
|------|----------|-----------------|
| **Deque** | `CS_QUEUE_DEQUE` | O(1) push/pop both ends, default choice |
| **List** | `CS_QUEUE_LIST` | O(1) operations, pointer overhead |

> [!NOTE]
> Vector is not available as a queue backend because `pop_front` would be $O(n)$.

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct queue {
    void *container;  // Pointer to underlying container
    queue_type type;  // Backend type enum
} queue;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `queue` struct | 16 | Pointer + type enum with padding |
| Container overhead | varies | Depends on backend choice |

### Per-Backend Memory

| Backend | Header Size | Per-Element Overhead |
|---------|-------------|---------------------|
| Deque | ~80 bytes | ~0 (blocked storage) |
| List | ~24 bytes | 24 bytes (prev/next pointers) |

### Scaling Behavior

- **Space Complexity**: $O(n)$ for both backends
- **Time Complexity**: $O(1)$ for all operations
- **Memory Layout**: Deque more cache-friendly; List avoids reallocation

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link required backends
gcc myprogram.c -Iinclude -Llib -lqueue -ldeque -llist -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> Both deque and list libraries must be linked due to the wrapper architecture, even if you only use one backend type.
