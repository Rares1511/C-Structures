# Vector — Dynamic Array Implementation

A fully generic dynamic array in C with automatic resizing, O(1) random access, and customizable element handling through attribute callbacks.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Initialization & Attributes](#initialization--attributes)
  - [Modification Operations](#modification-operations)
  - [Access & Search](#access--search)
  - [Removal & Cleanup](#removal--cleanup)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/vector.h>

int main(void) {
    vector v;
    vector_attr_t attr = {.size = sizeof(int)};
    vector_init(&v, attr);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        vector_push_back(&v, &values[i]);
    }

    // Access element at index 2
    int *elem = (int *)vector_at(v, 2);
    printf("Element at index 2: %d\n", *elem);  // Output: 30

    vector_free(&v);
    return 0;
}
```

---

## API Reference

### Data Structures

```c
typedef univ_attr_t vector_attr_t;

typedef struct vector {
    void *vec;            /* Contiguous buffer for elements */
    int cap;              /* Allocated capacity */
    int size;             /* Number of stored elements */
    int shrink_factor;    /* Threshold for automatic shrinking */
    vector_attr_t attr;   /* Element attribute callbacks */
} vector;
```

### Initialization & Attributes

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `vector_init` | `cs_codes vector_init(vector *v, vector_attr_t attr)` | `CS_SUCCESS` on success, `CS_MEM` on allocation failure | Initializes vector with given attributes; allocates initial buffer of `VECTOR_INIT_CAPACITY` (1024) elements |
| `vector_set_attr` | `void vector_set_attr(vector *vec, vector_attr_t attr)` | void | Replaces all attribute callbacks at once |
| `vector_set_free` | `void vector_set_free(vector *vec, freer fr)` | void | Sets custom destructor for elements |
| `vector_set_print` | `void vector_set_print(vector *vec, printer print)` | void | Sets custom print function for elements |
| `vector_set_copy` | `void vector_set_copy(vector *vec, deepcopy cp)` | void | Sets deep-copy function for element duplication |
| `vector_set_comp` | `void vector_set_comp(vector *vec, comparer comp)` | void | Sets comparator for `vector_find`, `vector_sort`, `vector_count` |

### Modification Operations

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `vector_insert_at` | `cs_codes vector_insert_at(vector *vec, const void *el, int pos)` | `CS_SUCCESS`, `CS_MEM`, or `CS_POS` | Inserts element at position; shifts subsequent elements right. Grows buffer if needed |
| `vector_push_back` | `cs_codes vector_push_back(vector *vec, const void *el)` | `CS_SUCCESS` or `CS_MEM` | Appends element to end (inline wrapper around `vector_insert_at`) |
| `vector_replace` | `cs_codes vector_replace(vector *vec, const void *el, int pos)` | `CS_SUCCESS`, `CS_EMPTY`, or `CS_POS` | Replaces element at position; frees old element if `freer` is set |
| `vector_sort` | `void vector_sort(vector *vec)` | void | Sorts vector in-place using comparator (requires `comp` to be set) |
| `vector_swap` | `void vector_swap(vector *vec1, vector *vec2)` | void | Swaps contents of two vectors in O(1) |

### Access & Search

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `vector_at` | `void *vector_at(vector vec, int pos)` | Pointer to element, or `NULL` | Returns pointer to element at index; no bounds checking returns garbage if out of range |
| `vector_find` | `int vector_find(vector vec, const void *el)` | Index ≥ 0, `CS_COMP` if no comparator, `CS_ELEM` if not found | Linear search for element using comparator |
| `vector_count` | `int vector_count(vector vec, const void *el)` | Count ≥ 0, or `CS_COMP` if no comparator | Returns number of occurrences of element |
| `vector_empty` | `int vector_empty(vector vec)` | 1 if empty, 0 otherwise | Inline check for `size == 0` |
| `vector_size` | `int vector_size(vector vec)` | Number of elements | Returns current element count |

### Removal & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `vector_erase` | `cs_codes vector_erase(vector *vec, int pos)` | `CS_SUCCESS`, `CS_EMPTY`, or `CS_POS` | Removes element at position; shifts subsequent elements left. May shrink buffer |
| `vector_pop_back` | `cs_codes vector_pop_back(vector *vec)` | `CS_SUCCESS` or `CS_EMPTY` | Removes last element (inline wrapper around `vector_erase`) |
| `vector_clear` | `void vector_clear(vector *vec)` | void | Removes all elements; calls `freer` on each if set. Resets size to 0 |
| `vector_free` | `void vector_free(void *v_vec)` | void | Frees all memory including buffer; calls `freer` on each element if set |
| `vector_print` | `void vector_print(FILE *stream, const void *v_vec)` | void | Prints all elements using `printer` callback |

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `vec` | `void *` | 8 bytes | Pointer to contiguous element buffer |
| `cap` | `int` | 4 bytes | Allocated capacity |
| `size` | `int` | 4 bytes | Current element count |
| `shrink_factor` | `int` | 4 bytes | Shrink threshold divisor |
| `attr` | `univ_attr_t` | ~48 bytes | Attribute callbacks |

**Total**: `sizeof(vector)` ≈ 72 bytes (with padding)

### Memory Growth Model

The vector uses a **doubling strategy** for growth:

$$M(n) = S_{header} + (capacity \times S_{element})$$

Where:
- $S_{header}$ = `sizeof(vector)` ≈ 72 bytes
- $capacity$ = allocated slots (initially 1024)
- $S_{element}$ = `attr.size` bytes per element

**Growth behavior**:
- Capacity doubles when `size == cap`
- Shrinks by half when `size < cap / VECTOR_SHRINK_FACTOR` (default: 4)

**Complexity**:
- Space: $O(n)$ with at most 4× overhead due to doubling
- Amortized push_back: $O(1)$

> [!NOTE]
> Initial capacity is 1024 elements (`VECTOR_INIT_CAPACITY`). For small datasets, consider using a smaller initial allocation or `list` for memory efficiency.

---

## Build & Usage

### Compilation

```bash
# Build the library
make libs

# Link against libvector
gcc -o myprogram myprogram.c -Iinclude -Llib -lvector
```

### Runtime (shared library)

```bash
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./myprogram
```

### Example with Custom Type

```c
#include <cs/vector.h>
#include <string.h>

typedef struct {
    char name[32];
    int age;
} person;

void person_print(FILE *stream, const void *p) {
    const person *per = p;
    fprintf(stream, "{name: %s, age: %d}", per->name, per->age);
}

int person_cmp(const void *a, const void *b) {
    return ((person *)a)->age - ((person *)b)->age;
}

int main(void) {
    vector v;
    vector_attr_t attr = {.size = sizeof(person)};
    vector_init(&v, attr);
    vector_set_print(&v, person_print);
    vector_set_comp(&v, person_cmp);

    person p1 = {"Alice", 30};
    person p2 = {"Bob", 25};
    vector_push_back(&v, &p1);
    vector_push_back(&v, &p2);

    vector_sort(&v);
    vector_print(stdout, &v);  // Bob (25), Alice (30)

    vector_free(&v);
    return 0;
}
```
