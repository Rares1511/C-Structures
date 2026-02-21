# **Large Number**

Arbitrary-precision arithmetic supporting integers and fractions in any base (2-36), enabling calculations beyond native type limits.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/large_number.h>

int main() {
    large_number a, b, result;
    
    // Initialize from string (numbers beyond LONG_MAX)
    large_number_init(&a, 10, LN_CHAR, "123456789012345678901234567890");
    large_number_init(&b, 10, LN_CHAR, "987654321098765432109876543210");
    large_number_init(&result, 10, LN_EMPTY);
    
    // Arithmetic operations
    large_number_add(&result, a, b);
    large_number_print(stdout, &result);  // Prints sum
    
    // Fractional numbers
    large_number frac;
    large_number_init(&frac, 10, LN_CHAR, "3.14159265358979");
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&result);
    large_number_free(&frac);
    return 0;
}
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `large_number_init(large_number *ln, unsigned int base, large_number_type type, ...)` | `CS_SUCCESS` on success, `CS_MEM` on failure, `CS_ELEM` on invalid input | Initializes from various sources. See initialization types below. |
| `large_number_add(large_number *out, const large_number a, const large_number b)` | `CS_SUCCESS` on success, `CS_NULL` if out is NULL, `CS_SIZE` if bases differ | Adds two numbers. Handles signs and fractional parts. |
| `large_number_sub(large_number *out, const large_number a, const large_number b)` | `CS_SUCCESS` on success, `CS_NULL` if out is NULL, `CS_SIZE` if bases differ | Subtracts b from a. Handles sign flipping when result is negative. |
| `large_number_mul(large_number *out, const large_number a, const large_number b)` | `CS_SUCCESS` on success, `CS_NULL` if out is NULL, `CS_SIZE` if bases differ | Multiplies two numbers using grade-school algorithm. |
| `large_number_swap_base(large_number *ln, unsigned int new_base)` | `CS_SUCCESS` on success, `CS_NULL` if ln is NULL, `CS_SIZE` if base < 2 | Converts number to different base (e.g., decimal to binary). |
| `large_number_comp(const large_number a, const large_number b)` | Positive if a > b, negative if a < b, 0 if equal | Compares two numbers considering sign, whole, and fractional parts. |
| `large_number_equal(const large_number a, const large_number b)` | `1` if equal, `0` otherwise | Shorthand for `large_number_comp(a, b) == 0`. |
| `large_number_clear(large_number *ln)` | `void` | Resets to zero, preserves allocated capacity. |
| `large_number_swap(large_number *a, large_number *b)` | `void` | Swaps contents of two large_numbers. |
| `large_number_print(FILE *stream, void *v_ln)` | `void` | Prints number with sign and decimal point if fractional. |
| `large_number_free(void *v_ln)` | `void` | Frees all allocated memory. |

### Initialization Types

| Type | Argument | Description |
|------|----------|-------------|
| `LN_CHAR` | `char*` | Parse from string (e.g., `"-123.456"`) |
| `LN_INT` | `long` | Initialize from native integer |
| `LN_DOUBLE` | `double` | Initialize from floating-point (extracts whole and fractional) |
| `LN_NUM` | `large_number` | Copy from another large_number |
| `LN_EMPTY` | (none) | Initialize as zero with capacity |

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct large_number {
    unsigned int *whole_digits;  // Whole part (reversed: LSB at index 0)
    int whole_size;              // Digits in whole part
    int whole_capacity;          // Allocated whole capacity
    
    unsigned int *frac_digits;   // Fractional part (normal: MSB at index 0)
    int frac_size;               // Digits in fractional part
    int frac_capacity;           // Allocated fractional capacity
    
    int sign;                    // +1 or -1
    unsigned int base;           // Number base (2-36)
} large_number;
```

| Field | Size (bytes) | Description |
|-------|--------------|-------------|
| `whole_digits` | 8 | Pointer to whole digit array |
| `whole_size` | 4 | Current whole digit count |
| `whole_capacity` | 4 | Allocated whole slots |
| `frac_digits` | 8 | Pointer to fractional digit array |
| `frac_size` | 4 | Current fractional digit count |
| `frac_capacity` | 4 | Allocated fractional slots |
| `sign` | 4 | Sign indicator |
| `base` | 4 | Number base |
| **Total** | **40** | Plus digit arrays |

### Memory Growth Formula

Total memory $M$ for a number with $w$ whole digits and $f$ fractional digits:

$$M(w, f) = S_{header} + (capacity_w \times 4) + (capacity_f \times 4)$$

Where:
- $S_{header} = 40$ bytes
- Initial capacity: 32 digits per array
- Capacity doubles when exceeded

### Digit Representation

Each digit stored as `unsigned int` allowing bases up to 2^32. For base 10:
- Digit values: 0-9
- 4 bytes per digit (could be optimized to 1 byte for base ≤ 256)

### Scaling Behavior

- **Space Complexity**: $O(d)$ where $d$ is total digit count
- **Addition/Subtraction**: $O(d)$
- **Multiplication**: $O(d^2)$ grade-school algorithm
- **Base Conversion**: $O(d^2)$

> [!NOTE]
> Whole digits are stored LSB-first (reversed) for efficient carry propagation. Fractional digits are stored MSB-first (normal reading order).

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -llarge_number -lm -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

### Base Conversion Example

```c
large_number num;
large_number_init(&num, 10, LN_INT, 255L);    // Decimal 255
large_number_swap_base(&num, 16);              // Convert to hex
large_number_print(stdout, &num);              // Prints "FF"
large_number_swap_base(&num, 2);               // Convert to binary
large_number_print(stdout, &num);              // Prints "11111111"
```

> [!WARNING]
> Operands must have the same base for arithmetic operations. Use `large_number_swap_base()` before mixing bases.
