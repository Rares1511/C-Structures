# Heap — Heap — Binary Heap (Priority Queue)

The heap module implements a generic binary heap in C, supporting customizable ordering via user-provided compare functions.

This module provides:
* O(log n) push and pop operations
* O(1) access to the top element
* Configurable min-heap or max-heap behavior
* Attribute-driven memory handling (copy/free/print/stream)
* Dynamic resizing (backed by a generic vector buffer)

It behaves similarly to `std::priority_queue` from C++, but fully generic and implemented in pure C.

## 🧱 Data Structure Layout
```C
typedef xuniv_attr_t heap_attr_t;

typedef struct heap {
    void *vec;        /*!< vector holing the elements of the heap */
    int size;         /*!< current size of the heap */
    int cap;          /*!< current capacity of the heap */
    heap_attr_t attr; /*!< attributes for the datatype inside the heap */
} heap;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `heap_init`      | Initialize a map with an attribute descriptor for both key and value |
| `heap_set_attr`  | Set all attributes at once |
| `heap_set_free`  | Set custom free function |
| `heap_set_print` | Set custom print function |
| `heap_set_copy`  | Set custom deep-copy function |
| `heap_set_comp`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `heap_push` |  |
| `heap_swap` | Append element to end |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `heap_top`   | Return pointer to element at index |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `heap_pop` | Remove element at index (shift left) |
| `heap_clear` | Remove all elements and free their memory |
| `heap_free`  | Free vector + internal memory |

### 🧹 Utility
| **Function**   | **Description** |
|----------------|-----------------|
| `heap_empty` | Remove element at index (shift left) |
| `heap_print` | Remove element at index (shift left) |