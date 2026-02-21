# **Priority Queue**

A max-heap container adapter providing $O(\log n)$ insertion and $O(1)$ access to the largest element.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/priority_queue.h>

int int_comp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;  // Max-heap: larger values have higher priority
}

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    priority_queue pq;
    priority_queue_attr_t attr = { .size = sizeof(int), .comp = int_comp, .print = int_print };
    
    priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_DEFAULT);  // Uses vector backend
    
    int values[] = {30, 10, 50, 20, 40};
    for (int i = 0; i < 5; i++) {
        priority_queue_push(&pq, &values[i]);
    }
    
    int *top = (int*)priority_queue_top(pq);  // Returns 50 (maximum)
    priority_queue_pop(&pq);                   // Removes 50
    top = (int*)priority_queue_top(pq);        // Returns 40 (new maximum)
    
    priority_queue_free(&pq);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `priority_queue_init(priority_queue *pq, priority_queue_attr_t attr, priority_queue_type type)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes with backend: `CS_PRIORITY_QUEUE_VECTOR` or `CS_PRIORITY_QUEUE_DEQUE`. |
| `priority_queue_push(priority_queue *pq, void *data)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Inserts element and maintains heap property. $O(\log n)$ complexity. |
| `priority_queue_pop(priority_queue *pq)` | `CS_SUCCESS` on success, `CS_EMPTY` if empty | Removes top (maximum) element. $O(\log n)$ complexity. |
| `priority_queue_top(priority_queue pq)` | `void*` pointer to top element, `NULL` if empty | Returns reference to maximum element. $O(1)$ complexity. |
| `priority_queue_size(priority_queue pq)` | `int` element count | Returns number of elements. |
| `priority_queue_empty(priority_queue pq)` | `1` if empty, `0` otherwise | Checks if priority queue has no elements. |
| `priority_queue_swap(priority_queue *pq1, priority_queue *pq2)` | `void` | Swaps contents of two priority queues. |
| `priority_queue_clear(priority_queue *pq)` | `void` | Removes all elements. |
| `priority_queue_free(void *v_pq)` | `void` | Frees priority queue and underlying container. |
| `priority_queue_print(FILE *stream, void *v_pq)` | `void` | Prints elements in heap order (not sorted). |

### Backend Types

| Type | Constant | Characteristics |
|------|----------|-----------------|
| **Vector** | `CS_PRIORITY_QUEUE_VECTOR` | Cache-friendly, good for most cases (default) |
| **Deque** | `CS_PRIORITY_QUEUE_DEQUE` | Better for very large heaps with frequent insertions |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct priority_queue {
    void *container;         // Underlying array storage
    priority_queue_type type; // Backend type
} priority_queue;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `priority_queue` struct | 16 | Pointer + type enum |
| Container overhead | 64 (vector) | Backend header |

### Heap Layout

Elements are stored in array form using implicit binary heap indexing:
- Parent of index $i$: $\lfloor(i-1)/2\rfloor$
- Left child of index $i$: $2i + 1$
- Right child of index $i$: $2i + 2$

### Memory Growth Formula

Total memory $M$ for $n$ elements:

$$M(n) = S_{header} + capacity \times S_e$$

Where capacity follows vector growth rules (doubles when full).

### Scaling Behavior

- **Space Complexity**: $O(n)$
- **Push/Pop Complexity**: $O(\log n)$ for heapify operations
- **Top Complexity**: $O(1)$
- **No pointer overhead**: Binary heap stored implicitly in array

> [!NOTE]
> For min-heap behavior, reverse the comparison function: return `*(int*)b - *(int*)a`.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link required backends
gcc myprogram.c -Iinclude -Llib -lpriority_queue -lvector -ldeque -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> The `comp` callback is **required**. The heap property depends on consistent comparison. Without it, behavior is undefined.
