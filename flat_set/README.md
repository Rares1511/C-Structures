# **Flat Set**

A sorted unique-element container backed by a contiguous array, providing cache-friendly iteration and $O(\log n)$ lookup via binary search.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/flat_set.h>

int int_comp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    flat_set fs;
    flat_set_attr_t attr = { .size = sizeof(int), .comp = int_comp, .print = int_print };
    
    flat_set_init(&fs, attr, CS_FLAT_SET_DEFAULT);  // Uses vector backend
    
    int values[] = {30, 10, 50, 20};
    for (int i = 0; i < 4; i++) {
        flat_set_insert(&fs, &values[i]);
    }
    
    // Elements stored sorted: [10, 20, 30, 50]
    int *first = (int*)flat_set_at(fs, 0);  // Returns 10
    
    int key = 30;
    int idx = flat_set_find(fs, &key);  // Returns 2
    
    flat_set_free(&fs);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `flat_set_init(flat_set *fs, flat_set_attr_t attr, flat_set_type type)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes with backend: `CS_FLAT_SET_VECTOR` or `CS_FLAT_SET_DEQUE`. |
| `flat_set_insert(flat_set *fs, void *data)` | `CS_SUCCESS` on success, `CS_ELEM` if exists, `CS_MEM` on failure | Inserts element in sorted position. $O(n)$ due to element shifting. |
| `flat_set_erase(flat_set *fs, void *data)` | `CS_SUCCESS` on success, `CS_ELEM` if not found | Removes element. $O(n)$ due to element shifting. |
| `flat_set_find(flat_set fs, void *data)` | Index if found, negative value if not found | Binary search lookup. $O(\log n)$ complexity. |
| `flat_set_at(flat_set fs, int index)` | `void*` pointer to element, `NULL` if out of bounds | Direct index access. $O(1)$ complexity. |
| `flat_set_size(flat_set fs)` | `int` element count | Returns number of elements. |
| `flat_set_empty(flat_set fs)` | `1` if empty, `0` otherwise | Checks if set has no elements. |
| `flat_set_swap(flat_set *fs1, flat_set *fs2)` | `void` | Swaps contents of two flat sets. |
| `flat_set_clear(flat_set *fs)` | `void` | Removes all elements. |
| `flat_set_free(void *v_fs)` | `void` | Frees flat set and underlying container. |
| `flat_set_print(FILE *stream, void *v_fs)` | `void` | Prints elements in sorted order. |

### Backend Types

| Type | Constant | Characteristics |
|------|----------|-----------------|
| **Vector** | `CS_FLAT_SET_VECTOR` | Best cache locality (default) |
| **Deque** | `CS_FLAT_SET_DEQUE` | Better for frequent front insertions |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct flat_set {
    void *container;     // Underlying sorted array
    flat_set_type type;  // Backend type
} flat_set;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `flat_set` struct | 16 | Pointer + type enum |
| Container overhead | 64 (vector) | Backend header |
| Elements | contiguous | No per-element pointer overhead |

### Memory Growth Formula

Total memory $M$ for $n$ elements:

$$M(n) = S_{header} + capacity \times S_e$$

### Flat Set vs Tree-Based Set

| Property | Flat Set | Tree-Based Set |
|----------|----------|----------------|
| **Lookup** | $O(\log n)$ binary search | $O(\log n)$ tree traversal |
| **Insert** | $O(n)$ shift elements | $O(\log n)$ rebalance |
| **Iterate** | Cache-friendly sequential | Pointer chasing |
| **Memory** | ~0 overhead per element | ~40 bytes per element |

### Scaling Behavior

- **Space Complexity**: $O(n)$ with minimal overhead
- **Lookup**: $O(\log n)$ via binary search
- **Insert/Erase**: $O(n)$ due to array shifting
- **Iteration**: Extremely cache-friendly

> [!NOTE]
> Best for read-heavy workloads with infrequent modifications. For write-heavy workloads, use tree-based `set` instead.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link required backends
gcc myprogram.c -Iinclude -Llib -lflat_set -lvector -ldeque -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> The `comp` callback is required for maintaining sorted order and binary search. Insert operations are $O(n)$—avoid in hot paths.
