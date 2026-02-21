# List — Circular Doubly Linked List

A fully generic circular doubly linked list in C with O(1) front/back operations and bidirectional traversal support.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Initialization & Attributes](#initialization--attributes)
  - [Modification Operations](#modification-operations)
  - [Access Operations](#access-operations)
  - [Removal & Cleanup](#removal--cleanup)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/list.h>

int main(void) {
    list l;
    list_attr_t attr = {.size = sizeof(int)};
    list_init(&l, attr);

    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        list_push_back(&l, &values[i]);
    }

    // Access front and back
    int *front = (int *)list_front(l);
    int *back = (int *)list_back(l);
    printf("Front: %d, Back: %d\n", *front, *back);  // Output: Front: 10, Back: 30

    list_free(&l);
    return 0;
}
```

---

## API Reference

### Data Structures

```c
typedef univ_attr_t list_attr_t;

typedef struct list_node {
    void *data;             /* Information held inside the node */
    struct list_node *next; /* Next node in the list */
    struct list_node *prev; /* Previous node in the list */
} list_node;

typedef struct list {
    int size;         /* Number of elements in the list */
    list_node *front; /* Pointer to front element */
    list_attr_t attr; /* Attributes for the datatype inside the list */
} list;
```

### Initialization & Attributes

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `list_init` | `cs_codes list_init(list *l, list_attr_t attr)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Initializes an empty list with given attributes |
| `list_set_attr` | `void list_set_attr(list *l, list_attr_t attr)` | void | Replaces all attribute callbacks; validates `attr.size` |
| `list_set_free` | `void list_set_free(list *l, freer fr)` | void | Sets custom destructor for elements |
| `list_set_print` | `void list_set_print(list *l, printer print)` | void | Sets custom print function for elements |
| `list_set_copy` | `void list_set_copy(list *l, deepcopy copy)` | void | Sets deep-copy function for element duplication |
| `list_set_comp` | `void list_set_comp(list *l, comparer comp)` | void | Sets comparator for sorting operations |

### Modification Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `list_push_front` | `cs_codes list_push_front(list *l, const void *el)` | `CS_SUCCESS` or `CS_MEM` | Inserts element at the front of the list |
| `list_push_back` | `cs_codes list_push_back(list *l, const void *el)` | `CS_SUCCESS` or `CS_MEM` | Appends element to the back of the list |
| `list_sort` | `void list_sort(list *l)` | void | Sorts list in-place; requires `comp` to be set. Returns `CS_FUNC` if no comparator |
| `list_swap` | `void list_swap(list *l1, list *l2)` | void | Swaps contents of two lists in O(1) |

### Access Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `list_front` | `void *list_front(list l)` | Pointer to front element data, or `NULL` | Returns pointer to data of front element |
| `list_back` | `void *list_back(list l)` | Pointer to back element data, or `NULL` | Returns pointer to data of back element |
| `list_empty` | `int list_empty(list l)` | 1 if empty, 0 otherwise | Inline check for `size == 0` |
| `list_size` | `int list_size(list l)` | Number of elements | Returns current element count |

### Removal & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `list_pop_front` | `cs_codes list_pop_front(list *l)` | `CS_SUCCESS` or `CS_SIZE` | Removes front element; calls `freer` if set |
| `list_pop_back` | `cs_codes list_pop_back(list *l)` | `CS_SUCCESS` or `CS_SIZE` | Removes back element; calls `freer` if set |
| `list_erase` | `cs_codes list_erase(list *l, int pos)` | `CS_SUCCESS` or `CS_SIZE` | Removes element at position; O(n) traversal |
| `list_clear` | `void list_clear(list *l)` | void | Removes all elements; calls `freer` on each if set |
| `list_free` | `void list_free(void *l_p)` | void | Frees all memory including nodes |
| `list_print` | `void list_print(FILE *stream, void *l_p)` | void | Prints all elements using `printer` callback |

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `size` | `int` | 4 bytes | Current element count |
| *(padding)* | — | 4 bytes | Alignment padding |
| `front` | `list_node *` | 8 bytes | Pointer to front node |
| `attr` | `univ_attr_t` | ~48 bytes | Attribute callbacks |

**Total**: `sizeof(list)` ≈ 64 bytes

### Node Layout

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `data` | `void *` | 8 bytes | Pointer to element data |
| `next` | `list_node *` | 8 bytes | Pointer to next node |
| `prev` | `list_node *` | 8 bytes | Pointer to previous node |

**Per-node overhead**: 24 bytes + `attr.size` bytes for data

### Memory Growth Model

$$M(n) = S_{header} + n \times (S_{node} + S_{element})$$

Where:
- $S_{header}$ = `sizeof(list)` ≈ 64 bytes
- $S_{node}$ = 24 bytes (node overhead)
- $S_{element}$ = `attr.size` bytes per element

**Complexity**:
- Space: $O(n)$ with constant per-element overhead
- All push/pop operations: $O(1)$
- Erase at position: $O(n)$ due to traversal

> [!NOTE]
> List is optimal for frequent insertions/deletions at both ends. For random access patterns, consider `vector` or `deque`.

> [!WARNING]
> `list_sort` temporarily converts the list to an array for sorting. Memory allocation may fail for very large lists.

---

## Build & Usage

### Compilation

```bash
# Build the library
make libs

# Link against liblist
gcc -o myprogram myprogram.c -Iinclude -Llib -llist
```

### Runtime (shared library)

```bash
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./myprogram
```

### Example with Bidirectional Traversal

```c
#include <cs/list.h>

void int_print(FILE *stream, const void *p) {
    fprintf(stream, "%d", *(int *)p);
}

int main(void) {
    list l;
    list_attr_t attr = {.size = sizeof(int)};
    list_init(&l, attr);
    list_set_print(&l, int_print);

    for (int i = 1; i <= 5; i++) {
        list_push_back(&l, &i);
    }

    list_print(stdout, &l);  // Output: 1 2 3 4 5

    // Remove from both ends
    list_pop_front(&l);
    list_pop_back(&l);

    list_print(stdout, &l);  // Output: 2 3 4

    list_free(&l);
    return 0;
}
```
