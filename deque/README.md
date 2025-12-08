# Deque — Double-Ended Queue (Linked List)

The deque module implements a generic double-ended queue in C using a doubly-linked list.

It provides:
* O(1) push/pop at both front and back
* Arbitrary element types via deque_attr_t
* Custom copy/free/print/stream handling (through univ_attr_t)
* Safe cloning and cleanup
* Simple, pointer-based API for front/back access

It behaves similarly to `std::deque` in usage, but with a linked-list backing (better for frequent inserts/removals at both ends, not random access).

## 🧱 Data Structure Layout
```C
typedef univ_attr_t deque_attr_t;

typedef struct deque_node_t {
    void *data;                /*!< data inside the deque node */
    struct deque_node_t *next; /*!< next element in the deque */
    struct deque_node_t *prev; /*!< previous element in the deque */
} deque_node_t;

typedef struct deque {
    int size;            /*!< current size of the deque */
    deque_node_t *front; /*!< first element in the deque */
    deque_node_t *back;  /*!< last element in the deque */
    deque_attr_t attr;   /*!< attributes for the elements inside the deque */
} deque;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `deque_init`      | Initialize a map with an attribute descriptor for both key and value |
| `deque_clone`  | Set all attributes at once |
| `deque_set_attr`  | Set custom free function |
| `deque_set_free` | Set custom print function |
| `deque_set_print`  | Set custom deep-copy function |
| `deque_set_copy`  | Set custom comparator |
| `deque_set_stream`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `deque_push_back` |  |
| `deque_push_front` | Append element to end |
| `deque_swap` | Append element to end |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `deque_front`   | Return pointer to element at index |
| `deque_back`   | Return pointer to element at index |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `deque_pop_back` | Remove element at index (shift left) |
| `deque_pop_front` | Remove all elements and free their memory |
| `deque_clear`  | Free vector + internal memory |
| `deque_free`  | Free vector + internal memory |

### 🧹 Utility
| **Function**   | **Description** |
|----------------|-----------------|
| `deque_print` | Remove element at index (shift left) |