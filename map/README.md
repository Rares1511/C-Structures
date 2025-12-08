# Map — Generic Associative Container (Red-Black Tree)

The map module implements a fully generic key–value associative container in C, backed by a Red-Black Tree. It provides:
* O(log n) insertion, lookup, and deletion
* Fully customizable key & value handling via map_attr_t
* Clean API for attribute configuration (copy, free, compare, print, stream)
* Safe ownership of key/value memory

This is roughly analogous to `std::map` in C++, but implemented in pure C with explicit attribute-based control.

## 🧱 Data Structure Layout
```C
typedef xuniv_attr_t map_attr_t;

typedef struct map_node {
    char color;                   /*!< color of the node */
    void *key;                    /*!< key of the node */
    void *val;                    /*!< value of the node */
    struct map_node *left_child;  /*!< left child of the node */
    struct map_node *right_child; /*!< right child of the node */
    struct map_node *father;      /*!< father of the node */
} map_node;

typedef struct map {
    struct map_node *root; /*!< root of the map */
    int size;              /*!< number of elements in the map */
    map_attr_t key_attr;   /*!< attributes of the key datatype */
    map_attr_t val_attr;   /*!< attributes of the value datatype */
} map;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `map_init`      | Initialize a map with an attribute descriptor for both key and value |
| `map_set_attr`  | Set all attributes at once |
| `map_set_free`  | Set custom free function |
| `map_set_print` | Set custom print function |
| `map_set_comp`  | Set custom deep-copy function |
| `map_set_copy`  | Set custom comparator |
| `map_set_stream`  | Set custom comparator |
| `map_key_set_attr`  | Set custom comparator |
| `map_key_set_free`  | Set custom comparator |
| `map_key_set_comp`  | Set custom comparator |
| `map_key_set_print`  | Set custom comparator |
| `map_key_set_copy`  | Set custom comparator |
| `map_key_set_stream`  | Set custom comparator |
| `map_val_set_attr`  | Set custom comparator |
| `map_val_set_free`  | Set custom comparator |
| `map_val_set_comp`  | Set custom comparator |
| `map_val_set_print`  | Set custom comparator |
| `map_val_set_copy`  | Set custom comparator |
| `map_val_set_stream`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `map_insert` |  |
| `map_swap` | Append element to end |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `map_get`   | Return pointer to element at index |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `map_delete` | Remove element at index (shift left) |
| `map_clear` | Remove all elements and free their memory |
| `map_free`  | Free vector + internal memory |

### 🧹 Utility
| **Function**   | **Description** |
|----------------|-----------------|
| `map_print` | Remove element at index (shift left) |