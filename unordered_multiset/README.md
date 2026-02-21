# **Unordered Multiset**

A hash-table-backed container that allows duplicate elements with $O(1)$ average-case operations.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/unordered_multiset.h>

size_t int_hash(const void *key) {
    return (size_t)(*(int*)key);
}

int int_comp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int main() {
    unordered_multiset umset;
    unordered_multiset_attr_t attr = { .size = sizeof(int), .comp = int_comp };
    
    unordered_multiset_init(&umset, attr, int_hash, 16);
    
    int val = 42;
    unordered_multiset_insert(&umset, &val);  // Count: 1
    unordered_multiset_insert(&umset, &val);  // Count: 2
    unordered_multiset_insert(&umset, &val);  // Count: 3
    
    int count = unordered_multiset_count(umset, &val);  // Returns 3
    
    unordered_multiset_free(&umset);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `unordered_multiset_init(unordered_multiset *umset, unordered_multiset_attr_t attr, hash_func_t hash_func, int initial_capacity)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes with hash function and initial capacity. |
| `unordered_multiset_insert(unordered_multiset *umset, const void *key)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Inserts element. Increments count if element exists. |
| `unordered_multiset_erase(unordered_multiset *umset, const void *key)` | `CS_SUCCESS` on success, `CS_ELEM` if not found | Removes all occurrences of element. |
| `unordered_multiset_find(unordered_multiset umset, const void *key)` | `void*` pointer if found, `NULL` otherwise | $O(1)$ average-case lookup. |
| `unordered_multiset_count(unordered_multiset umset, const void *key)` | `int` occurrence count | Returns how many times element was inserted. |
| `unordered_multiset_size(unordered_multiset umset)` | `int` total elements | Returns sum of all occurrence counts. |
| `unordered_multiset_empty(unordered_multiset umset)` | `1` if empty, `0` otherwise | Checks if multiset has no elements. |
| `unordered_multiset_clear(unordered_multiset *umset)` | `void` | Removes all elements. |
| `unordered_multiset_free(void *v_umset)` | `void` | Frees all memory. |
| `unordered_multiset_print(FILE *stream, void *v_umset)` | `void` | Prints elements with counts. |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct unordered_multiset {
    hash_table *ht;  // Hash table with count tracking
} unordered_multiset;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `unordered_multiset` struct | 8 | Single pointer |
| Hash table | ~56 | Metadata |
| Bucket array | $8 \times capacity$ | Pointer per bucket |
| Entry overhead | ~28 | Per unique element (includes count) |

### Memory Growth Formula

Total memory $M$ for $u$ unique elements with capacity $c$:

$$M(u, c) = S_{header} + (c \times 8) + u \times (S_{entry} + S_e)$$

### Scaling Behavior

- **Space Complexity**: $O(u + c)$ where $u$ is unique element count
- **Time Complexity**: $O(1)$ average for all operations
- **Memory Efficiency**: Duplicates increment counter, no additional entries

> [!NOTE]
> More memory-efficient than storing duplicate entries separately. High-multiplicity elements have constant overhead regardless of count.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lunordered_multiset -lhash_table -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> `erase()` removes ALL occurrences of an element. To decrement count, use repeated insert/erase patterns or implement a wrapper.
