# **Pair**

A generic two-element tuple for storing heterogeneous data, commonly used as map entries or function return values.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/pair.h>

void str_print(FILE *stream, const void *el) {
    fprintf(stream, "%s", *(char**)el);
}

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    pair p;
    pair_attr_t first_attr = { .size = sizeof(char*), .print = str_print };
    pair_attr_t second_attr = { .size = sizeof(int), .print = int_print };
    
    pair_init(&p, &first_attr, &second_attr);
    
    char *name = "Alice";
    int age = 25;
    pair_set(&p, &name, &age);
    
    char **retrieved_name = (char**)pair_first(p);   // "Alice"
    int *retrieved_age = (int*)pair_second(p);       // 25
    
    pair_free(&p);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `pair_init(pair *p, pair_attr_t *first_attr, pair_attr_t *second_attr)` | `CS_SUCCESS` on success, `CS_MEM` on failure | Initializes pair with separate attributes for each element. |
| `pair_set(pair *p, const void *first, const void *second)` | `CS_SUCCESS` on success, error code otherwise | Sets both elements. Uses `copy` callback if provided, otherwise memcpy. |
| `pair_first(pair p)` | `void*` pointer to first element | Returns reference to first element. |
| `pair_second(pair p)` | `void*` pointer to second element | Returns reference to second element. |
| `pair_print(FILE *stream, const void *v_p)` | `void` | Prints both elements using their respective `print` callbacks. |
| `pair_free(void *v_p)` | `void` | Frees pair and both elements (using `fr` callbacks if set). |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct pair {
    void* first;              // Pointer to first element
    void* second;             // Pointer to second element
    pair_attr_t* first_attr;  // First element attributes
    pair_attr_t* second_attr; // Second element attributes
} pair;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `pair` struct | 32 | 4 pointers |
| First element | varies | User-defined size |
| Second element | varies | User-defined size |
| Attribute structs | 80 | 2 × 40 bytes (if allocated) |

### Memory Growth Formula

Total memory $M$ for a pair:

$$M = S_{pair} + S_{first} + S_{second} + 2 \times S_{attr}$$

Where:
- $S_{pair} = 32$ bytes
- $S_{attr} = 40$ bytes per attribute struct

### Scaling Behavior

- **Space Complexity**: $O(1)$ constant per pair
- **Access Complexity**: $O(1)$ for both elements
- **No dynamic growth**: Fixed two-element structure

> [!NOTE]
> Pairs are typically used as building blocks for maps and other associative containers. The attribute pointers allow heterogeneous types within a single pair.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lpair -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

### Use with Maps

```c
// Pairs are used internally by map implementations
// You typically don't create pairs directly when using maps
map m;
map_init(&m, key_attr, val_attr);
map_insert(&m, &key, &value);  // Creates pair internally
```

> [!WARNING]
> If storing pointers to heap-allocated data, provide `fr` callbacks in attributes to prevent memory leaks when calling `pair_free()`.
