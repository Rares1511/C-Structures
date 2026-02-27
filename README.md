<div align="center">

# 🏗️ C-Structures Library

**A comprehensive, efficient, and modular collection of data structures in pure C**

[![Language](https://img.shields.io/badge/Language-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Linux%20|%20macOS%20|%20Windows-lightgrey.svg)]()

---

*Clean APIs • Zero Dependencies • Memory-Safe • Valgrind-Tested*

</div>

---

## ✨ Overview

C-Structures is a feature-rich library providing **STL-inspired containers** for C developers. Whether you're learning data structures, teaching algorithms, or building lightweight software, this library offers battle-tested implementations with consistent, intuitive APIs.

### 🎯 Core Features

| Feature | Description |
|---------|-------------|
| 🔧 **Modular Design** | Use only what you need — each structure is independent |
| 🛡️ **Memory Safety** | Built-in error handling and leak-free operations |
| 📐 **Generic Support** | Copy, free, and compare callbacks for any data type |
| 🧪 **Thoroughly Tested** | Comprehensive unit tests with Valgrind compatibility |
| 📖 **Well Documented** | Detailed README for each data structure |

---

## 🚀 Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/C-Structures.git
cd C-Structures

# Build and install (system-wide)
sudo make install

# Or build locally without installation
make libs
```

### Usage

```c
#include <cs/vector.h>
#include <cs/map.h>

int main() {
    // Create a vector of integers
    Vector *v = vector_create(sizeof(int), NULL);
    
    int value = 42;
    vector_push_back(v, &value);
    
    vector_destroy(v);
    return 0;
}
```

### Compilation

```bash
# If installed system-wide
gcc myprogram.c -lvector -lmap -o myprogram

# If using local build
gcc myprogram.c -Iinclude -Llib -lvector -lmap -o myprogram
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

---

## 📚 Data Structures

### Sequence Containers
> Linear data structures for ordered element storage

| Container | Description | Documentation |
|-----------|-------------|---------------|
| **Vector** | Dynamic array with automatic resizing | [📖 README](vector/README.md) |
| **Deque** | Double-ended queue with O(1) access at both ends | [📖 README](deque/README.md) |
| **List** | Doubly-linked list for efficient insertions | [📖 README](list/README.md) |
| **Forward List** | Singly-linked list for memory efficiency | [📖 README](forward_list/README.md) |

### Associative Containers
> Sorted containers using Red-Black Trees

| Container | Description | Documentation |
|-----------|-------------|---------------|
| **Set** | Unique sorted elements | [📖 README](set/README.md) |
| **Map** | Key-value pairs with unique keys | [📖 README](map/README.md) |
| **Multiset** | Sorted elements allowing duplicates | [📖 README](multiset/README.md) |
| **Multimap** | Key-value pairs allowing duplicate keys | [📖 README](multimap/README.md) |

### Unordered Associative Containers
> Hash-based containers for O(1) average access

| Container | Description | Documentation |
|-----------|-------------|---------------|
| **Unordered Set** | Hash set with unique elements | [📖 README](unordered_set/README.md) |
| **Unordered Map** | Hash map with unique keys | [📖 README](unordered_map/README.md) |
| **Unordered Multiset** | Hash set allowing duplicates | [📖 README](unordered_multiset/README.md) |
| **Unordered Multimap** | Hash map allowing duplicate keys | [📖 README](unordered_multimap/README.md) |

### Container Adaptors
> Restricted interfaces built on other containers

| Container | Description | Documentation |
|-----------|-------------|---------------|
| **Stack** | LIFO (Last-In-First-Out) structure | [📖 README](stack/README.md) |
| **Queue** | FIFO (First-In-First-Out) structure | [📖 README](queue/README.md) |
| **Priority Queue** | Heap-based priority container | [📖 README](priority_queue/README.md) |

### Specialized Containers
> Containers for specific use cases

| Container | Description | Documentation |
|-----------|-------------|---------------|
| **Flat Set** | Sorted vector-based set | [📖 README](flat_set/README.md) |
| **Large Number** | Arbitrary precision arithmetic | [📖 README](large_number/README.md) |
| **Pair** | Two-element tuple | [📖 README](pair/README.md) |

### Utilities

| Module | Description | Documentation |
|--------|-------------|---------------|
| **Cargs** | Command-line argument parser | [📖 README](cargs/README.md) |
| **CLogger** | Lightweight file logger with severity levels | [📖 README](clogger/README.md) |

---

## 🧪 Testing

Run the complete test suite:

```bash
# Basic test run
make unittest

# With memory leak detection
make unittest memcheck=true

# With debug logging
make unittest debug=true

# Custom random seed
make unittest SEED=12345
```

### Test Coverage

| Module | Status | Module | Status |
|--------|--------|--------|--------|
| Vector | ✅ | Unordered Set | ✅ |
| Deque | ✅ | Unordered Map | ✅ |
| List | ✅ | Unordered Multiset | ✅ |
| Forward List | ✅ | Unordered Multimap | ✅ |
| Set | ✅ | Stack | ✅ |
| Map | ✅ | Queue | ✅ |
| Multiset | ✅ | Priority Queue | ✅ |
| Multimap | ✅ | Flat Set | ✅ |
| Pair | ✅ | Large Number | ✅ |

### Each test suite includes:
- ✅ Boundary condition checks
- ✅ Randomized stress tests
- ✅ Memory leak verification (Valgrind)
- ✅ API behavior certification

---

## 🏗️ Project Structure

```
C-Structures/
├── include/cs/          # Public headers
│   ├── universal.h      # Common definitions
│   ├── vector.h
│   ├── map.h
│   ├── clogger.h
│   └── ...
├── lib/                 # Compiled shared libraries
├── vector/              # Vector implementation
├── map/                 # Map implementation
├── clogger/             # Logger implementation
├── ...                  # Other modules
├── rbt/                 # Red-Black Tree (internal)
├── hash_table/          # Hash Table (internal)
├── Makefile
└── unittest.c           # Test runner
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<div align="center">

**Made with ❤️ for the C community**

</div>