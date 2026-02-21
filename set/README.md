# **Set**

A sorted unique-element container backed by a Red-Black Tree, providing $O(\log n)$ insertion, deletion, and lookup.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/set.h>

int int_comp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

void int_print(FILE *stream, const void *el) {
    fprintf(stream, "%d", *(int*)el);
}

int main() {
    set s;
    set_attr_t attr = { .size = sizeof(int), .comp = int_comp, .print = int_print };
    
    set_init(&s, attr);
    
    int values[] = {5, 3, 8, 1};
    for (int i = 0; i < 4; i++) {
        set_insert(&s, &values[i]);
    }
    
    int key = 3;
    void *found = set_find(s, &key);  // Returns pointer to 3
    
    set_free(&s);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `set_init(set *s, set_attr_t attr)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Initializes set with specified attributes. Requires `comp` callback for ordering. |
| `set_insert(set *s, void *data)` | `CS_SUCCESS` on success, `CS_ELEM` if element exists, `CS_MEM` on allocation failure | Inserts element maintaining sorted order. Duplicate elements are rejected. |
| `set_delete(set *s, void *data)` | `CS_SUCCESS` on success, `CS_ELEM` if element not found | Removes element from set. Rebalances tree after deletion. |
| `set_find(set s, void *data)` | `void*` pointer to element if found, `NULL` otherwise | Searches for element using binary search. $O(\log n)$ complexity. |
| `set_size(set s)` | `int` number of unique elements | Returns count of elements in set. |
| `set_empty(set s)` | `1` if empty, `0` otherwise | Checks if set has no elements. |
| `set_swap(set *s1, set *s2)` | `void` | Swaps contents of two sets. O(1) pointer swap. |
| `set_clear(set *s)` | `void` | Removes all elements. Calls `fr` callback on each if set. |
| `set_free(void *v_s)` | `void` | Frees all memory including RBT nodes. |
| `set_print(FILE *stream, void *v_s)` | `void` | Prints elements in sorted order using `print` callback. |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct set {
    rbt *t;  // Pointer to Red-Black Tree
} set;
```

Each RBT node contains:
```c
typedef struct rbt_node {
    void *data;              // Element data
    struct rbt_node *left;   // Left child
    struct rbt_node *right;  // Right child
    struct rbt_node *parent; // Parent node
    int color;               // RED or BLACK
} rbt_node;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `set` struct | 8 | Single pointer to RBT |
| RBT header | ~40 | Tree metadata |
| Node overhead | 40 | Per-element (4 pointers + color + padding) |

### Memory Growth Formula

Total memory $M$ for $n$ elements with element size $S_e$:

$$M(n) = S_{set} + S_{rbt} + n \times (S_{node} + S_e)$$

Where:
- $S_{set} = 8$ bytes
- $S_{rbt} \approx 40$ bytes
- $S_{node} = 40$ bytes per element

### Scaling Behavior

- **Space Complexity**: $O(n)$ with 40-byte overhead per element
- **Time Complexity**: All operations are $O(\log n)$ due to balanced tree
- **Rebalancing**: Automatic after insert/delete to maintain $O(\log n)$ height

> [!NOTE]
> The `comp` callback is **required** for set operations. Without it, the set cannot maintain ordering.

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lset -lrbt -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

### System-wide Installation

```bash
sudo make install
gcc myprogram.c -lset -lrbt -o myprogram
```

> [!WARNING]
> Always provide a `comp` callback. Operations will fail or produce undefined behavior without one.
