# Map — Generic Associative Container (Red-Black Tree)

A fully generic key-value associative container in C backed by a Red-Black Tree, providing O(log n) operations with customizable key and value handling.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Initialization & Attributes](#initialization--attributes)
  - [Key Attribute Setters](#key-attribute-setters)
  - [Value Attribute Setters](#value-attribute-setters)
  - [Modification Operations](#modification-operations)
  - [Access & Search](#access--search)
  - [Removal & Cleanup](#removal--cleanup)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/map.h>
#include <string.h>

int str_cmp(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

int main(void) {
    map m;
    map_attr_t key_attr = {.size = sizeof(char *), .comp = str_cmp};
    map_attr_t val_attr = {.size = sizeof(int)};
    map_init(&m, key_attr, val_attr);

    char *key = "hello";
    int value = 42;
    map_insert(&m, &key, &value);

    int *found = (int *)map_find(m, &key);
    if (found) {
        printf("Value: %d\n", *found);  // Output: Value: 42
    }

    map_free(&m);
    return 0;
}
```

---

## API Reference

### Data Structures

```c
typedef univ_attr_t map_attr_t;
typedef struct rbt rbt;

typedef struct map {
    rbt *t;               /* Pointer to underlying Red-Black Tree */
    map_attr_t *key_attr; /* Attributes for key datatype */
    map_attr_t *val_attr; /* Attributes for value datatype */
} map;
```

### Initialization & Attributes

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `map_init` | `cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr)` | `CS_SUCCESS` or `CS_MEM` | Initializes map with separate key and value attributes |
| `map_set_attr` | `void map_set_attr(map *m, map_attr_t key_attr, map_attr_t val_attr)` | void | Sets both key and value attributes at once |
| `map_set_free` | `void map_set_free(map *m, freer key_fr, freer val_fr)` | void | Sets freer functions for both key and value |
| `map_set_print` | `void map_set_print(map *m, printer key_pr, printer val_pr)` | void | Sets printer functions for both key and value |
| `map_set_comp` | `void map_set_comp(map *m, comparer key_comp, comparer val_comp)` | void | Sets comparators for both key and value |
| `map_set_copy` | `void map_set_copy(map *m, deepcopy key_copy, deepcopy val_copy)` | void | Sets deep-copy functions for both key and value |
| `map_set_stream` | `void map_set_stream(map *m, FILE *key_stream, FILE *val_stream)` | void | Sets output streams for both key and value |

### Key Attribute Setters

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `map_key_set_attr` | `void map_key_set_attr(map *m, map_attr_t key_attr)` | void | Sets key attributes only |
| `map_key_set_free` | `void map_key_set_free(map *m, freer key_fr)` | void | Sets key freer function |
| `map_key_set_comp` | `void map_key_set_comp(map *m, comparer key_comp)` | void | Sets key comparator (required for tree operations) |
| `map_key_set_print` | `void map_key_set_print(map *m, printer key_pr)` | void | Sets key printer function |
| `map_key_set_copy` | `void map_key_set_copy(map *m, deepcopy key_copy)` | void | Sets key deep-copy function |
| `map_key_set_stream` | `void map_key_set_stream(map *m, FILE *key_stream)` | void | Sets key output stream |

### Value Attribute Setters

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `map_val_set_attr` | `void map_val_set_attr(map *m, map_attr_t val_attr)` | void | Sets value attributes only |
| `map_val_set_free` | `void map_val_set_free(map *m, freer val_fr)` | void | Sets value freer function |
| `map_val_set_comp` | `void map_val_set_comp(map *m, comparer val_comp)` | void | Sets value comparator |
| `map_val_set_print` | `void map_val_set_print(map *m, printer val_pr)` | void | Sets value printer function |
| `map_val_set_copy` | `void map_val_set_copy(map *m, deepcopy val_copy)` | void | Sets value deep-copy function |
| `map_val_set_stream` | `void map_val_set_stream(map *m, FILE *val_stream)` | void | Sets value output stream |

### Modification Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `map_insert` | `cs_codes map_insert(map *m, void *key, void *val)` | `CS_SUCCESS`, `CS_ELEM` (key exists), or `CS_MEM` | Inserts key-value pair; fails if key already exists |
| `map_swap` | `void map_swap(map *m1, map *m2)` | void | Swaps contents of two maps in O(1) |

### Access & Search

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `map_find` | `void *map_find(map m, void *key)` | Pointer to value, or `NULL` if not found | Searches for key and returns pointer to associated value |
| `map_empty` | `int map_empty(map m)` | 1 if empty, 0 otherwise | Checks if map contains no elements |
| `map_size` | `int map_size(map m)` | Number of key-value pairs | Returns current element count |

### Removal & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `map_delete` | `cs_codes map_delete(map *m, void *key)` | `CS_SUCCESS` or `CS_ELEM` | Removes key-value pair; calls freers if set |
| `map_clear` | `void map_clear(map *m)` | void | Removes all elements; calls freers on each if set |
| `map_free` | `void map_free(void *v_m)` | void | Frees all memory including RBT structure |
| `map_print` | `void map_print(FILE *stream, void *v_m)` | void | Prints all key-value pairs in-order |

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `t` | `rbt *` | 8 bytes | Pointer to Red-Black Tree |
| `key_attr` | `map_attr_t *` | 8 bytes | Pointer to key attributes |
| `val_attr` | `map_attr_t *` | 8 bytes | Pointer to value attributes |

**Total**: `sizeof(map)` = 24 bytes

### RBT Node Layout (Internal)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `color` | `char` | 1 byte | Node color (RED/BLACK) |
| *(padding)* | — | 7 bytes | Alignment padding |
| `key` | `void *` | 8 bytes | Pointer to key data |
| `val` | `void *` | 8 bytes | Pointer to value data |
| `left_child` | `rbt_node *` | 8 bytes | Left child pointer |
| `right_child` | `rbt_node *` | 8 bytes | Right child pointer |
| `father` | `rbt_node *` | 8 bytes | Parent pointer |

**Per-node overhead**: ~48 bytes + key_size + value_size

### Memory Growth Model

$$M(n) = S_{header} + 2 \times S_{attr} + n \times (S_{node} + S_{key} + S_{value})$$

Where:
- $S_{header}$ = 24 bytes
- $S_{attr}$ = ~48 bytes per attribute structure
- $S_{node}$ = ~48 bytes (RBT node overhead)
- $S_{key}$ = `key_attr.size` bytes
- $S_{value}$ = `val_attr.size` bytes

**Complexity**:
- Space: $O(n)$
- Insert/Delete/Find: $O(\log n)$ guaranteed (balanced tree)

> [!NOTE]
> The map requires a comparator function for the key type. Set it via `map_key_set_comp` or in the initial `key_attr`.

> [!WARNING]
> Inserting a duplicate key returns `CS_ELEM` and does not update the value. Delete first if you need to update.

---

## Build & Usage

### Compilation

```bash
# Build the library
make libs

# Link against libmap and librbt
gcc -o myprogram myprogram.c -Iinclude -Llib -lmap -lrbt
```

### Runtime (shared library)

```bash
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./myprogram
```

### Example with Integer Keys

```c
#include <cs/map.h>

int int_cmp(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void int_print(FILE *stream, const void *p) {
    fprintf(stream, "%d", *(int *)p);
}

int main(void) {
    map m;
    map_attr_t key_attr = {.size = sizeof(int), .comp = int_cmp};
    map_attr_t val_attr = {.size = sizeof(double)};
    map_init(&m, key_attr, val_attr);
    map_key_set_print(&m, int_print);

    int keys[] = {3, 1, 4, 1, 5};
    double vals[] = {3.14, 1.0, 4.0, 1.5, 5.0};

    for (int i = 0; i < 5; i++) {
        map_insert(&m, &keys[i], &vals[i]);  // Duplicate key 1 will fail
    }

    printf("Map size: %d\n", map_size(m));  // Output: 4 (one duplicate)

    int search = 4;
    double *result = (double *)map_find(m, &search);
    if (result) {
        printf("Found: %f\n", *result);  // Output: 4.000000
    }

    map_free(&m);
    return 0;
}
```
