# Deque — Double-Ended Queue (Block-Based)

A generic double-ended queue in C using a block-based architecture for efficient O(1) operations at both ends with O(1) random access.

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
#include <cs/deque.h>

int main(void) {
    deque dq;
    deque_attr_t attr = {.size = sizeof(int)};
    deque_init(&dq, attr);

    int a = 10, b = 20, c = 30;
    deque_push_back(&dq, &a);
    deque_push_front(&dq, &b);
    deque_push_back(&dq, &c);

    // Deque now contains: [20, 10, 30]
    int *front = (int *)deque_front(dq);
    int *back = (int *)deque_back(dq);
    printf("Front: %d, Back: %d\n", *front, *back);  // Output: Front: 20, Back: 30

    // Random access
    int *mid = (int *)deque_at(dq, 1);
    printf("Middle: %d\n", *mid);  // Output: Middle: 10

    deque_free(&dq);
    return 0;
}
```

---

## API Reference

### Data Structures

```c
typedef univ_attr_t deque_attr_t;

typedef struct deque_block_t {
    void *data;  /* Pointer to block's data buffer */
    int front;   /* Index of first element in block */
    int back;    /* Index of next insertion position */
} deque_block_t;

typedef struct deque {
    deque_block_t *blocks; /* Array of blocks */
    deque_attr_t attr;     /* Attributes for elements */
    int size;              /* Total number of elements */
    int block_size;        /* Elements per block (default: 64) */
    int block_cap;         /* Number of allocated blocks */
    int front;             /* Index of front block */
    int back;              /* Index of back block */
} deque;
```

### Initialization & Attributes

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `deque_init` | `cs_codes deque_init(deque *dq, deque_attr_t attr)` | `CS_SUCCESS` or error code | Initializes deque with given attributes; allocates initial block array |
| `deque_set_attr` | `void deque_set_attr(deque *dq, deque_attr_t attr)` | void | Replaces all attribute callbacks |
| `deque_set_free` | `void deque_set_free(deque *dq, freer fr)` | void | Sets custom destructor for elements |
| `deque_set_print` | `void deque_set_print(deque *dq, printer print)` | void | Sets custom print function for elements |
| `deque_set_copy` | `void deque_set_copy(deque *dq, deepcopy cp)` | void | Sets deep-copy function for element duplication |
| `deque_set_stream` | `void deque_set_stream(deque *dq, FILE *stream)` | void | Sets output stream for printing |

### Modification Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `deque_push_back` | `cs_codes deque_push_back(deque *dq, const void *el)` | `CS_SUCCESS` or error code | Appends element to back in amortized O(1) |
| `deque_push_front` | `cs_codes deque_push_front(deque *dq, const void *el)` | `CS_SUCCESS` or error code | Inserts element at front in amortized O(1) |
| `deque_insert_at` | `cs_codes deque_insert_at(deque *dq, const void *el, int index)` | `CS_SUCCESS` or error code | Inserts element at index; shifts elements |
| `deque_swap` | `void deque_swap(deque *dq1, deque *dq2)` | void | Swaps contents of two deques in O(1) |

### Access Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `deque_front` | `void *deque_front(deque dq)` | Pointer to front element, or `NULL` | Returns pointer to first element |
| `deque_back` | `void *deque_back(deque dq)` | Pointer to back element, or `NULL` | Returns pointer to last element |
| `deque_at` | `void *deque_at(deque dq, int index)` | Pointer to element, or `NULL` | Random access in O(1); returns `NULL` if out of bounds |
| `deque_empty` | `int deque_empty(deque dq)` | 1 if empty, 0 otherwise | Inline check for `size == 0` |
| `deque_size` | `int deque_size(deque dq)` | Number of elements | Returns current element count |

### Removal & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `deque_pop_back` | `cs_codes deque_pop_back(deque *dq)` | `CS_SUCCESS` or error code | Removes last element; calls `freer` if set |
| `deque_pop_front` | `cs_codes deque_pop_front(deque *dq)` | `CS_SUCCESS` or error code | Removes first element; calls `freer` if set |
| `deque_erase` | `cs_codes deque_erase(deque *dq, int index)` | `CS_SUCCESS` or error code | Removes element at index; shifts elements |
| `deque_clear` | `void deque_clear(deque *dq)` | void | Removes all elements; calls `freer` on each if set |
| `deque_free` | `void deque_free(void *v_dq)` | void | Frees all memory including blocks |
| `deque_print` | `void deque_print(FILE *stream, const void *v_dq)` | void | Prints all elements using `printer` callback |

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `blocks` | `deque_block_t *` | 8 bytes | Pointer to block array |
| `attr` | `univ_attr_t` | ~48 bytes | Attribute callbacks |
| `size` | `int` | 4 bytes | Total element count |
| `block_size` | `int` | 4 bytes | Elements per block |
| `block_cap` | `int` | 4 bytes | Allocated block count |
| `front` | `int` | 4 bytes | Front block index |
| `back` | `int` | 4 bytes | Back block index |

**Total**: `sizeof(deque)` ≈ 80 bytes

### Block Layout

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `data` | `void *` | 8 bytes | Pointer to element buffer |
| `front` | `int` | 4 bytes | First element index in block |
| `back` | `int` | 4 bytes | Next insertion index in block |

**Per-block overhead**: 16 bytes + (`block_size` × `element_size`)

### Memory Growth Model

$$M(n) = S_{header} + B \times S_{block\_meta} + B \times (block\_size \times S_{element})$$

Where:
- $S_{header}$ = `sizeof(deque)` ≈ 80 bytes
- $B$ = number of allocated blocks
- $S_{block\_meta}$ = 16 bytes per block metadata
- $block\_size$ = 64 elements per block (default)
- $S_{element}$ = `attr.size` bytes per element

**Growth behavior**:
- Initial allocation: `DEQUE_INIT_BLOCKS` (64) blocks
- Block array doubles when all blocks are full

**Complexity**:
- Space: $O(n)$ with block-level granularity
- `push_front` / `push_back`: Amortized $O(1)$
- Random access `deque_at`: $O(1)$
- `insert_at` / `erase`: $O(n)$ due to shifting

> [!NOTE]
> The block-based design provides O(1) random access unlike linked-list implementations. Each block holds 64 elements by default (`DEQUE_BLOCK_SIZE`).

> [!WARNING]
> `insert_at` and `erase` at arbitrary positions require element shifting and are O(n). Prefer front/back operations when possible.

---

## Build & Usage

### Compilation

```bash
# Build the library
make libs

# Link against libdeque
gcc -o myprogram myprogram.c -Iinclude -Llib -ldeque
```

### Runtime (shared library)

```bash
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./myprogram
```

### Example: Sliding Window

```c
#include <cs/deque.h>

void int_print(FILE *stream, const void *p) {
    fprintf(stream, "%d ", *(int *)p);
}

int main(void) {
    deque dq;
    deque_attr_t attr = {.size = sizeof(int)};
    deque_init(&dq, attr);
    deque_set_print(&dq, int_print);

    // Simulate sliding window of size 3
    int window_size = 3;
    int stream[] = {1, 2, 3, 4, 5, 6, 7};

    for (int i = 0; i < 7; i++) {
        deque_push_back(&dq, &stream[i]);

        if (deque_size(dq) > window_size) {
            deque_pop_front(&dq);
        }

        printf("Window: ");
        deque_print(stdout, &dq);
        printf("\n");
    }

    deque_free(&dq);
    return 0;
}
```

**Output**:
```
Window: 1 
Window: 1 2 
Window: 1 2 3 
Window: 2 3 4 
Window: 3 4 5 
Window: 4 5 6 
Window: 5 6 7 
```
