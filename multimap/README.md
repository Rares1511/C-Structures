# **Multimap**

A sorted key-value container that allows multiple values per key, backed by a Red-Black Tree with vector storage.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/multimap.h>

int str_comp(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

int main() {
    multimap mm;
    multimap_attr_t key_attr = { .size = sizeof(char*), .comp = str_comp };
    multimap_attr_t val_attr = { .size = sizeof(int) };
    
    multimap_init(&mm, key_attr, val_attr);
    
    char *key = "Alice";
    int scores[] = {95, 87, 92};
    
    // Insert multiple values for same key
    for (int i = 0; i < 3; i++) {
        multimap_insert(&mm, &key, &scores[i]);
    }
    
    // Get all values for key
    vector *values = multimap_get(&mm, &key);  // Vector of {95, 87, 92}
    
    multimap_free(&mm);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `multimap_init(multimap *mm, multimap_attr_t key_attr, multimap_attr_t value_attr)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Initializes multimap with key and value attributes. |
| `multimap_insert(multimap *mm, const void *key, const void *value)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Inserts key-value pair. If key exists, appends value to its vector. |
| `multimap_delete(multimap *mm, const void *key)` | `CS_SUCCESS` on success, `CS_ELEM` if key not found | Removes key and all associated values. |
| `multimap_get(multimap *mm, const void *key)` | `vector*` of values if found, `NULL` otherwise | Returns pointer to vector containing all values for key. |
| `multimap_size(multimap mm)` | `int` total key-value pairs | Returns total count of all values across all keys. |
| `multimap_empty(multimap mm)` | `1` if empty, `0` otherwise | Checks if multimap has no entries. |
| `multimap_swap(multimap *mm1, multimap *mm2)` | `void` | Swaps contents of two multimaps. |
| `multimap_clear(multimap *mm)` | `void` | Removes all key-value pairs. |
| `multimap_free(void *v_mm)` | `void` | Frees all memory including all value vectors. |
| `multimap_print(FILE *stream, void *v_mm)` | `void` | Prints all key-value associations. |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct multimap {
    rbt *t;                    // Red-Black Tree (keys)
    multimap_attr_t* key_attr; // Key attributes
    multimap_attr_t* value_attr; // Value attributes
    multimap_attr_t* vec_attr; // Vector attributes
    int size;                  // Total value count
} multimap;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `multimap` struct | 48 | Header with pointers |
| RBT node | 40 | Per unique key |
| Key data | varies | User-defined key size |
| Value vector | 64 | Per unique key (stores all values) |

### Memory Growth Formula

Total memory $M$ for $k$ unique keys with average $v$ values per key:

$$M(k, v) = S_{header} + k \times (S_{node} + S_{key} + S_{vec} + v \times S_{val})$$

Where:
- $S_{header} = 48$ bytes
- $S_{node} = 40$ bytes
- $S_{vec} = 64$ bytes (vector header)

### Scaling Behavior

- **Space Complexity**: $O(k + n)$ where $k$ = keys, $n$ = total values
- **Time Complexity**: $O(\log k)$ for key lookup, $O(1)$ amortized for value insertion
- **Value Storage**: Each key maps to a dynamic vector that grows as values are added

> [!NOTE]
> Values are stored in insertion order within each key's vector. Use vector operations to access individual values.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lmultimap -lrbt -lvector -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

> [!WARNING]
> The returned `vector*` from `multimap_get()` is owned by the multimap. Do not free it directly.
