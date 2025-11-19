# C-Structures Library

A collection of efficient, modular, and reusable data structure implementations in C, designed to offer clean APIs for common containers such as vector, map, heap, queue, deque, stack, hash table, and more.

Each structure relies on the shared universal/ module (universal.h + universal.c) which provides:
* Generic copy/free/compare attribute handling
* Unified error codes
* Debugging utilities
* Memory-safe operations

This project is ideal for learning, teaching, and building lightweight C software without external dependencies.

## 🚀 Getting Started

To use any data structure:
1. Extract the archive.
2. Run the makefile (recommended to use sudo): `sudo make`
3. Include the header for the desired datastructure `eg. #include <cs/map.h>`

## 📚 Documentation by Data Structure

- [Vector](vector/README.md)
- [Map](map/README.md)
- [Deque](deque/README.md)
- [Heap](heap/README.md)

## Unit Tests - TODO
Unit tests are being progressively added for all modules:  
* vector  ✔
* map ✔
* deque ✔
* pair ☐  
* queue ☐ 
* deque ☐ 
* stack ☐ 
* heap ☐ 
* hash_table ☐ 
* string ☐ 
* list ☐ 
* large_number ☐

### Each test suite includes:
* Boundary checks
* Randomized tests
* Memory leak testing (valgrind-compatible)
* Behavior certification for every API function