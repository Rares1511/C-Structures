# **Unordered Map**

A hash-table-backed key-value container with $O(1)$ average-case operations.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/unordered_map.h>

size_t str_hash(const void *key) {
    const char *str = *(const char**)key;
    size_t hash = 5381;
    while (*str) hash = ((hash << 5) + hash) + *str++;
    return hash;
}

int str_comp(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

int main() {
    unordered_map umap;
    unordered_map_attr_t key_attr = { .size = sizeof(char*), .comp = str_comp };
    unordered_map_attr_t val_attr = { .size = sizeof(int) };
    
    unordered_map_init(&umap, key_attr, val_attr, str_hash, 16);
    
    char *key = "Alice";
    int age = 25;
    unordered_map_add_entry(&umap, &key, &age);
    
    int *found = (int*)unordered_map_get_entry(umap, &key);  // Returns 25
    
    unordered_map_free(&umap);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `unordered_map_init(unordered_map *umap, unordered_map_attr_t key_attr, unordered_map_attr_t value_attr, hash_func_t hash_func, int initial_capacity)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes map with key/value attributes and hash function. |
| `unordered_map_add_entry(unordered_map *umap, const void *key, const void *value)` | `CS_SUCCESS` on success, `CS_ELEM` if key exists, `CS_MEM` on failure | Inserts key-value pair. Rejects duplicate keys. |
| `unordered_map_remove_entry(unordered_map *umap, const void *key)` | `CS_SUCCESS` on success, `CS_ELEM` if key not found | Removes entry by key. |
| `unordered_map_get_entry(unordered_map umap, const void *key)` | `void*` pointer to value if found, `NULL` otherwise | $O(1)$ average-case lookup by key. |
| `unordered_map_count(unordered_map umap, const void *key)` | `1` if key exists, `0` otherwise | Checks key existence. |
| `unordered_map_size(unordered_map umap)` | `int` number of entries | Returns key-value pair count. |
| `unordered_map_empty(unordered_map umap)` | `1` if empty, `0` otherwise | Checks if map has no entries. |
| `unordered_map_swap(unordered_map *umap1, unordered_map *umap2)` | `void` | Swaps contents of two maps. O(1) pointer swap. |
| `unordered_map_clear(unordered_map *umap)` | `void` | Removes all entries. Retains bucket array. |
| `unordered_map_free(void *v_umap)` | `void` | Frees all memory. |
| `unordered_map_print(FILE *stream, void *v_umap)` | `void` | Prints all key-value pairs. |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct unordered_map {
    hash_table *ht;
    hash_func_t hash_func;
    unordered_map_attr_t key_attr;
    unordered_map_attr_t value_attr;
} unordered_map;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `unordered_map` struct | ~96 | Header with function pointers and attrs |
| Hash table | ~56 | Bucket management |
| Bucket array | $8 \times capacity$ | Pointer per bucket |
| Entry overhead | ~32 | Per entry (next, hash, key ptr, value ptr) |

### Memory Growth Formula

Total memory $M$ for $n$ entries with capacity $c$:

$$M(n, c) = S_{header} + (c \times 8) + n \times (S_{entry} + S_k + S_v)$$

Where:
- $S_{header} \approx 150$ bytes
- $S_{entry} \approx 32$ bytes
- $S_k$, $S_v$ = key and value sizes

### Scaling Behavior

- **Space Complexity**: $O(n + c)$
- **Time Complexity**: $O(1)$ average, $O(n)$ worst-case
- **Load Factor**: Rehashes at 75% occupancy, doubling capacity

> [!NOTE]
> For string keys, use a proper hash function like djb2 or FNV-1a. The hash function significantly impacts performance.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lunordered_map -lhash_table -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> The `hash_func` must be consistent: equal keys must produce equal hashes. The `comp` callback in `key_attr` is required for collision resolution.
