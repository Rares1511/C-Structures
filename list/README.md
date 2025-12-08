# List — Circular Double Linked List

This is roughly analogous to `std::list` in C++.

## 🧱 Data Structure Layout
```C
typedef xuniv_attr_t list_attr_t;

typedef struct list_node {
    void *data;             /*!< information held inside the node */
    struct list_node *next; /*!< next node in the list */
    struct list_node *prev; /*!< previous node in the list */
} list_node;

typedef struct list {
    list_node *front; /*!< front element of the list */
    int size;         /*!< current size of the list */
    list_attr_t attr; /*!< attributes for the datatype inside the list */
} list;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `list_init`      | Initialize a map with an attribute descriptor for both key and value |
| `list_set_attr`  | Set all attributes at once |
| `list_set_free`  | Set custom free function |
| `list_set_print` | Set custom print function |
| `list_set_copy`  | Set custom deep-copy function |
| `list_set_comp`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `list_push_front` |  |
| `list_push_back` | Append element to end |
| `list_sort` | Append element to end |
| `list_swap` | Append element to end |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `list_front`   | Return pointer to element at index |
| `list_back`   | Return pointer to element at index |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `list_pop_front` | Remove element at index (shift left) |
| `list_pop_back` | Remove all elements and free their memory |
| `list_erase`  | Free vector + internal memory |
| `list_clear` | Remove all elements and free their memory |
| `list_free`  | Free vector + internal memory |

### 🧹 Utility
| **Function**   | **Description** |
|----------------|-----------------|
| `list_empty` | Remove element at index (shift left) |
| `list_print` | Remove element at index (shift left) |