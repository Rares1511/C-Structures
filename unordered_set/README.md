# **Unordered Set**

A hash-table-backed container for unique elements with $O(1)$ average-case operations.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/unordered_set.h>

size_t int_hash(const void *key) {
    return (size_t)(*(int*)key);
}

int int_comp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int main() {
    unordered_set uset;
    unordered_set_attr_t attr = { .size = sizeof(int), .comp = int_comp };
    
    unordered_set_init(&uset, attr, int_hash, 16);
    
    int values[] = {10, 20, 30, 40};
    for (int i = 0; i < 4; i++) {
        unordered_set_insert(&uset, &values[i]);
    }
    
    int key = 20;
    void *found = unordered_set_find(uset, &key);  // O(1) lookup
    
    unordered_set_free(&uset);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `unordered_set_init(unordered_set *uset, unordered_set_attr_t attr, hash_func_t hash_func, int initial_capacity)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Initializes with custom hash function and initial bucket count. |
| `unordered_set_insert(unordered_set *uset, const void *key)` | `CS_SUCCESS` on success, `CS_ELEM` if element exists, `CS_MEM` on failure | Inserts unique element. Triggers rehash if load factor exceeds threshold. |
| `unordered_set_erase(unordered_set *uset, const void *key)` | `CS_SUCCESS` on success, `CS_ELEM` if not found | Removes element from set. |
| `unordered_set_find(unordered_set uset, const void *key)` | `void*` pointer to element if found, `NULL` otherwise | $O(1)$ average-case lookup. |
| `unordered_set_count(unordered_set uset, const void *key)` | `1` if present, `0` otherwise | Checks existence (always 0 or 1 for sets). |
| `unordered_set_size(unordered_set uset)` | `int` number of elements | Returns element count. |
| `unordered_set_empty(unordered_set uset)` | `1` if empty, `0` otherwise | Checks if set has no elements. |
| `unordered_set_clear(unordered_set *uset)` | `void` | Removes all elements. Retains bucket array. |
| `unordered_set_free(void *v_uset)` | `void` | Frees all memory including hash table. |
| `unordered_set_print(FILE *stream, void *v_uset)` | `void` | Prints all elements (unordered). |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct unordered_set {
    hash_table *ht;  // Pointer to hash table
} unordered_set;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `unordered_set` struct | 8 | Single pointer |
| Hash table header | ~56 | Metadata, function pointers |
| Bucket array | $8 \times capacity$ | Pointer per bucket |
| Entry overhead | ~24 | Per element (next, hash, data pointer) |

### Memory Growth Formula

Total memory $M$ for $n$ elements with capacity $c$:

$$M(n, c) = S_{header} + (c \times 8) + n \times (S_{entry} + S_e)$$

Where:
- $S_{header} \approx 64$ bytes
- $S_{entry} \approx 24$ bytes per element

### Scaling Behavior

- **Space Complexity**: $O(n + c)$ where $c$ is bucket count
- **Time Complexity**: $O(1)$ average, $O(n)$ worst-case (hash collisions)
- **Rehashing**: Automatic when load factor > 0.75, doubles capacity

> [!NOTE]
> Choose a good hash function to minimize collisions. Poor hash distribution degrades to $O(n)$ linear search.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lunordered_set -lhash_table -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> Both `hash_func` and `comp` callbacks are required. The hash function determines bucket placement; the comparator resolves collisions.
