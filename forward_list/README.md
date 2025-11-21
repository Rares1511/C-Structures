# Forward List — Single Linked List

This is roughly analogous to `std::forward_list` in C++.

## 🧱 Data Structure Layout
```C
typedef xuniv_attr_t forward_list_attr_t;

typedef struct forward_list_node{
    void* data;
    struct forward_list_node* next;
} forward_list_node;

typedef struct forward_list{
    forward_list_node* head;
    forward_list_attr_t attr;
    int size;
} forward_list;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `forward_list_init`      | Initialize a map with an attribute descriptor for both key and value |
| `forward_list_set_attr`  | Set all attributes at once |
| `forward_list_set_size`  | Set custom free function |
| `forward_list_set_free` | Set custom print function |
| `forward_list_set_copy`  | Set custom deep-copy function |
| `forward_list_set_print`  | Set custom comparator |
| `forward_list_set_compare`  | Set custom comparator |
| `forward_list_set_stream`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `forward_list_push_front` |  |
| `forward_list_swap` | Append element to end |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `forward_list_front`   | Return pointer to element at index |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `forward_list_pop_front` | Remove element at index (shift left) |
| `forward_list_clear` | Remove all elements and free their memory |
| `forward_list_free`  | Free vector + internal memory |

### 🧹 Utility
| **Function**   | **Description** |
|----------------|-----------------|
| `forward_list_empty` | Remove element at index (shift left) |
| `forward_list_print` | Remove element at index (shift left) |