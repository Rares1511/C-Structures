# Queue — FIFO Linked Queue (Generic)

The queue module implements a fully generic FIFO queue in C, using a singly-linked list for O(1) insertions and removals at opposite ends.

It provides:
* O(1) push at the back
* O(1) pop from the front
* Arbitrary element types (queue_attr_t)
* Configurable copy/free/print/stream behavior
* Clean, lightweight API
* No capacity limits (linked-list based)

It behaves similarly to `std::queue` in usage.

## 🧱 Data Structure Layout
```C
typedef univ_attr_t queue_attr_t;

typedef struct queue_node {
    void *data;              /*!< data of the queue node */
    struct queue_node *next; /*!< next element in the queue */
} queue_node;

typedef struct queue {
    queue_node *start; /*!< start node of the queue */
    queue_node *end;   /*!< end node of the queue */
    int size;          /*!< current size of the queue */
    queue_attr_t attr; /*!< attributes for the elements inside the queue */
} queue;
```

## Api Overview
### 🔧 Initialization & Attribute Control
| **Function**       | **Description** |
|--------------------|-----------------|
| `queue_init`      | Initialize a map with an attribute descriptor for both key and value |
| `queue_set_attr`  | Set all attributes at once |
| `queue_set_free`  | Set custom free function |
| `queue_set_print` | Set custom print function |
| `queue_set_copy`  | Set custom deep-copy function |
| `queue_set_stream`  | Set custom comparator |

### ➕ Modification Operations
| **Function**       | **Description** |
|--------------------|-----------------|
| `queue_push` |  |
| `queue_swap` | Append element to end |

### 🔍 Access & Search
| **Function**  | **Description** |
|---------------|-----------------|
| `queue_front`   | Return pointer to element at index |
| `queue_back`   | Return pointer to element at index |

### ❌ Removal & Cleanup
| **Function**   | **Description** |
|----------------|-----------------|
| `queue_pop` | Remove element at index (shift left) |
| `queue_clear` | Remove all elements and free their memory |

### 🧹 Utility
| **Function**   | **Description** |
|----------------|-----------------|
| `queue_empty` | Remove element at index (shift left) |
| `queue_print` | Remove element at index (shift left) |