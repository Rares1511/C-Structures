# Vector — Dynamic Array Implementation

## The vector module provides a fully generic dynamic array in C, supporting:
* Automatic resizing
* Arbitrary element types
* Custom copy/free/compare/print functions via vector_attr_t
* Random access in O(1)
* Insert/erase/replace operations
* Optional sorting
* Memory-safe cleanup

It is designed as a lightweight and reusable equivalent of std::vector in C.

## 🧱 Data Structure Layout
```C
typedef struct vector {
    void *vec;          // raw buffer for elements
    int cap;            // allocated capacity
    int size;           // number of stored elements
    vector_attr_t attr; // element attribute handler
} vector;

typedef xuniv_attr_t vector_attr_t;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `vector_init`      | Initialize a vector with an attribute descriptor |
| `vector_set_attr`  | Set all attributes at once |
| `vector_set_free`  | Set custom free function |
| `vector_set_print` | Set custom print function |
| `vector_set_copy`  | Set custom deep-copy function |
| `vector_set_comp`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `vector_insert_at` | Insert element at index (shift right) |
| `vector_push_back` | Append element to end |
| `vector_replace`   | Replace element at index |
| `vector_sort`      | Sort vector (required compare) |
| `vector_swap`      | Swap contents of two vectors |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `vector_at`   | Return pointer to element at index |
| `vector_find` | Find index of element (requires compare) |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `vector_erase` | Remove element at index (shift left) |
| `vector_clear` | Remove all elements and free their memory |
| `vector_free`  | Free vector + internal memory |
| `vector_print` | Print elements using print callback |