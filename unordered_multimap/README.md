# **Unordered Multimap**

A hash-table-backed key-value container allowing multiple values per key with $O(1)$ average-case operations.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/unordered_multimap.h>

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
    unordered_multimap ummap;
    unordered_multimap_attr_t key_attr = { .size = sizeof(char*), .comp = str_comp };
    unordered_multimap_attr_t val_attr = { .size = sizeof(int) };
    
    unordered_multimap_init(&ummap, key_attr, val_attr, str_hash, 16);
    
    char *key = "scores";
    int values[] = {95, 87, 92};
    for (int i = 0; i < 3; i++) {
        unordered_multimap_add_entry(&ummap, &key, &values[i]);
    }
    
    int count = unordered_multimap_count(ummap, &key);  // Returns 3
    
    unordered_multimap_free(&ummap);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `unordered_multimap_init(unordered_multimap *ummap, unordered_multimap_attr_t key_attr, unordered_multimap_attr_t value_attr, hash_func_t hash_func, int initial_capacity)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes with separate key/value attributes. |
| `unordered_multimap_add_entry(unordered_multimap *umap, const void *key, const void *value)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Adds key-value pair. Allows duplicate keys. |
| `unordered_multimap_remove_entry(unordered_multimap *umap, const void *key)` | `CS_SUCCESS` on success, `CS_ELEM` if not found | Removes all entries with specified key. |
| `unordered_multimap_get_entry(unordered_multimap umap, const void *key)` | `void*` pointer to first value, `NULL` if not found | Returns first value associated with key. |
| `unordered_multimap_count(unordered_multimap umap, const void *key)` | `int` count of entries | Returns number of values for given key. |
| `unordered_multimap_size(unordered_multimap umap)` | `int` total entries | Returns total key-value pair count. |
| `unordered_multimap_empty(unordered_multimap umap)` | `1` if empty, `0` otherwise | Checks if multimap has no entries. |
| `unordered_multimap_swap(unordered_multimap *umap1, unordered_multimap *umap2)` | `void` | Swaps contents of two multimaps. |
| `unordered_multimap_clear(unordered_multimap *umap)` | `void` | Removes all entries. |
| `unordered_multimap_free(void *v_umap)` | `void` | Frees all memory. |
| `unordered_multimap_print(FILE *stream, void *v_umap)` | `void` | Prints all key-value pairs. |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct unordered_multimap {
    hash_table *ht;
    hash_func_t hash_func;
    unordered_multimap_attr_t key_attr;
    unordered_multimap_attr_t value_attr;
} unordered_multimap;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `unordered_multimap` struct | ~96 | Header with attributes |
| Hash table | ~56 | Bucket management |
| Bucket array | $8 \times capacity$ | Pointer per bucket |
| Entry overhead | ~32 | Per key-value pair |

### Memory Growth Formula

Total memory $M$ for $n$ total entries with capacity $c$:

$$M(n, c) = S_{header} + (c \times 8) + n \times (S_{entry} + S_k + S_v)$$

### Scaling Behavior

- **Space Complexity**: $O(n + c)$ where $n$ is total entries
- **Time Complexity**: $O(1)$ average for add, $O(k)$ for remove (where $k$ is values per key)
- **Collision Handling**: Chaining allows multiple values per hash bucket

> [!NOTE]
> Unlike `multimap` (RBT-backed), operations here are $O(1)$ but keys are unordered. Use when order doesn't matter.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lunordered_multimap -lhash_table -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> `get_entry()` returns only the first value for a key. To iterate all values, use the underlying hash table iteration or count-based access.
