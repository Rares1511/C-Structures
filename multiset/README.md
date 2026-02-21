# **Multiset**

A sorted container that allows duplicate elements, backed by a Red-Black Tree with occurrence counting.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/multiset.h>

int int_comp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    multiset ms;
    multiset_attr_t attr = { .size = sizeof(int), .comp = int_comp, .print = int_print };
    
    multiset_init(&ms, attr);
    
    int val = 5;
    multiset_insert(&ms, &val);  // Add 5
    multiset_insert(&ms, &val);  // Add 5 again
    multiset_insert(&ms, &val);  // Add 5 again
    
    int count = multiset_count(&ms, &val);  // Returns 3
    
    multiset_delete(&ms, &val);  // Decrements count to 2
    
    multiset_free(&ms);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `multiset_init(multiset *ms, multiset_attr_t attr)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Initializes multiset with specified attributes. Requires `comp` callback. |
| `multiset_insert(multiset *ms, const void *elem)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Inserts element. If element exists, increments occurrence count. |
| `multiset_delete(multiset *ms, const void *elem)` | `CS_SUCCESS` on success, `CS_ELEM` if element not found | Decrements occurrence count. Removes node when count reaches zero. |
| `multiset_count(multiset *ms, const void *elem)` | `int` count of occurrences, `0` if not found | Returns how many times element has been inserted. |
| `multiset_size(multiset ms)` | `int` number of unique elements | Returns count of distinct elements (not total occurrences). |
| `multiset_empty(multiset ms)` | `1` if empty, `0` otherwise | Checks if multiset has no elements. |
| `multiset_swap(multiset *ms1, multiset *ms2)` | `void` | Swaps contents of two multisets. |
| `multiset_clear(multiset *ms)` | `void` | Removes all elements and resets counts. |
| `multiset_free(void *v_ms)` | `void` | Frees all memory including RBT nodes. |
| `multiset_print(FILE *stream, void *v_ms)` | `void` | Prints elements with their counts. |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct multiset {
    rbt *t;                   // Red-Black Tree
    multiset_attr_t* el_attr; // Element attributes
    multiset_attr_t* count_attr; // Count attributes
    int size;                 // Unique element count
} multiset;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `multiset` struct | 32 | Header with pointers and size |
| RBT node | 40 | Per unique element |
| Element data | varies | User-defined element size |
| Count storage | 4 | Integer count per unique element |

### Memory Growth Formula

Total memory $M$ for $u$ unique elements:

$$M(u) = S_{header} + u \times (S_{node} + S_e + S_{count})$$

Where:
- $S_{header} = 32$ bytes
- $S_{node} = 40$ bytes
- $S_{count} = 4$ bytes

### Scaling Behavior

- **Space Complexity**: $O(u)$ where $u$ is unique element count
- **Time Complexity**: $O(\log u)$ for all operations
- **Memory Efficiency**: Duplicate insertions only increment counter, no additional nodes

> [!NOTE]
> Unlike std::multiset, this implementation stores a count per unique element rather than duplicating nodes, making it more memory-efficient for high-frequency duplicates.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lmultiset -lrbt -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> The `comp` callback is required. Without it, element comparison will fail.
