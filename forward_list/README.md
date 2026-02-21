# Forward List — Singly Linked List

A lightweight singly linked list in C with O(1) front operations and minimal memory overhead per node.

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
#include <cs/forward_list.h>

int main(void) {
    forward_list fl;
    forward_list_attr_t attr = {.size = sizeof(int)};
    forward_list_init(&fl, attr);

    int values[] = {30, 20, 10};
    for (int i = 0; i < 3; i++) {
        forward_list_push_front(&fl, &values[i]);  // Insert at front
    }

    // Access front element
    int *front = (int *)forward_list_front(fl);
    printf("Front: %d\n", *front);  // Output: Front: 10

    forward_list_free(&fl);
    return 0;
}
```

---

## API Reference

### Data Structures

```c
typedef univ_attr_t forward_list_attr_t;

typedef struct forward_list_node {
    void *data;                      /* Pointer to element data */
    struct forward_list_node *next;  /* Pointer to next node */
} forward_list_node;

typedef struct forward_list {
    forward_list_node *head;    /* Pointer to first node */
    forward_list_attr_t attr;   /* Attributes for elements */
    int size;                   /* Number of elements */
} forward_list;
```

### Initialization & Attributes

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `forward_list_init` | `cs_codes forward_list_init(forward_list *list, forward_list_attr_t attr)` | `CS_SUCCESS` or error code | Initializes an empty forward list with given attributes |
| `forward_list_set_attr` | `void forward_list_set_attr(forward_list *list, forward_list_attr_t attr)` | void | Replaces all attribute callbacks; validates `attr.size` |
| `forward_list_set_size` | `void forward_list_set_size(forward_list *list, int size)` | void | Sets the element size in attributes |
| `forward_list_set_free` | `void forward_list_set_free(forward_list *list, freer fr)` | void | Sets custom destructor for elements |
| `forward_list_set_copy` | `void forward_list_set_copy(forward_list *list, deepcopy cp)` | void | Sets deep-copy function for element duplication |
| `forward_list_set_print` | `void forward_list_set_print(forward_list *list, printer pr)` | void | Sets custom print function for elements |
| `forward_list_set_compare` | `void forward_list_set_compare(forward_list *list, comparer cmp)` | void | Sets comparator for search/sort operations |

### Modification Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `forward_list_push_front` | `cs_codes forward_list_push_front(forward_list *list, const void *data)` | `CS_SUCCESS` or error code | Inserts element at the front in O(1) |
| `forward_list_swap` | `void forward_list_swap(forward_list *list1, forward_list *list2)` | void | Swaps contents of two forward lists in O(1) |

### Access Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `forward_list_front` | `void *forward_list_front(forward_list list)` | Pointer to front element data, or `NULL` | Returns pointer to data of first element |
| `forward_list_empty` | `int forward_list_empty(forward_list list)` | 1 if empty, 0 otherwise | Inline check for `size == 0` |
| `forward_list_size` | `int forward_list_size(forward_list list)` | Number of elements | Returns current element count |

### Removal & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `forward_list_pop_front` | `cs_codes forward_list_pop_front(forward_list *list)` | `CS_SUCCESS` or error code | Removes front element in O(1); calls `freer` if set |
| `forward_list_clear` | `void forward_list_clear(forward_list *list)` | void | Removes all elements; calls `freer` on each if set |
| `forward_list_free` | `void forward_list_free(void *v_l)` | void | Frees all memory including nodes |
| `forward_list_print` | `void forward_list_print(FILE *stream, const void *v_l)` | void | Prints all elements using `printer` callback |

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `head` | `forward_list_node *` | 8 bytes | Pointer to first node |
| `attr` | `univ_attr_t` | ~48 bytes | Attribute callbacks |
| `size` | `int` | 4 bytes | Current element count |
| *(padding)* | — | 4 bytes | Alignment padding |

**Total**: `sizeof(forward_list)` ≈ 64 bytes

### Node Layout

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `data` | `void *` | 8 bytes | Pointer to element data |
| `next` | `forward_list_node *` | 8 bytes | Pointer to next node |

**Per-node overhead**: 16 bytes + `attr.size` bytes for data

### Memory Growth Model

$$M(n) = S_{header} + n \times (S_{node} + S_{element})$$

Where:
- $S_{header}$ = `sizeof(forward_list)` ≈ 64 bytes
- $S_{node}$ = 16 bytes (node overhead)
- $S_{element}$ = `attr.size` bytes per element

**Complexity**:
- Space: $O(n)$ with 16-byte overhead per element
- `push_front` / `pop_front`: $O(1)$
- Access by index: $O(n)$ (requires traversal)

> [!NOTE]
> Forward list has 8 bytes less overhead per node compared to doubly linked `list`. Use it when you only need forward traversal and front operations.

> [!WARNING]
> There is no `push_back` operation. All insertions are at the front, resulting in reverse order if elements are pushed sequentially.

---

## Build & Usage

### Compilation

```bash
# Build the library
make libs

# Link against libforward_list
gcc -o myprogram myprogram.c -Iinclude -Llib -lforward_list
```

### Runtime (shared library)

```bash
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./myprogram
```

### Example: Stack-Like Usage

```c
#include <cs/forward_list.h>

void int_print(FILE *stream, const void *p) {
    fprintf(stream, "%d ", *(int *)p);
}

int main(void) {
    forward_list fl;
    forward_list_attr_t attr = {.size = sizeof(int)};
    forward_list_init(&fl, attr);
    forward_list_set_print(&fl, int_print);

    // Push elements (LIFO order)
    for (int i = 1; i <= 5; i++) {
        forward_list_push_front(&fl, &i);
    }

    forward_list_print(stdout, &fl);  // Output: 5 4 3 2 1

    // Pop until empty
    while (!forward_list_empty(fl)) {
        int *top = (int *)forward_list_front(fl);
        printf("Popping: %d\n", *top);
        forward_list_pop_front(&fl);
    }

    forward_list_free(&fl);
    return 0;
}
```
