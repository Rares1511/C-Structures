# **Cargs**

A lightweight command-line argument parser supporting typed arguments with short/long names, help text, and default values.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/cargs.h>
#include <stdio.h>

int main(int argc, char **argv) {
    cparser parser;
    cargs_init(&parser, argc, argv);
    
    // Add arguments with defaults
    int port = 8080;
    char *host = "localhost";
    int verbose = 0;
    
    cargs_add_arg(&parser, "port", "Server port number", 0, CARG_TYPE_INT, &port);
    cargs_add_arg(&parser, "host", "Server hostname", 0, CARG_TYPE_STRING, &host);
    cargs_add_arg(&parser, "verbose", "Enable verbose output", 0, CARG_TYPE_BOOL, &verbose);
    
    cargs_parse(&parser);
    
    // Retrieve parsed values
    int *parsed_port = (int*)cargs_get_arg(&parser, "port");
    char **parsed_host = (char**)cargs_get_arg(&parser, "host");
    
    printf("Connecting to %s:%d\n", *parsed_host, *parsed_port);
    
    cargs_free(&parser);
    return 0;
}
```

### Command Line Usage

```bash
# Long form
./myprogram --port 3000 --host example.com --verbose

# Short form
./myprogram -p 3000 -h example.com -v
```

---

## API Reference

| Function | Return Value | Description |
|----------|--------------|-------------|
| `cargs_init(cparser *parser, int argc, char **argv)` | `void` | Initializes parser with command-line arguments. |
| `cargs_add_arg(cparser *parser, const char *name, const char *help, char required, carg_type type, void *default_value)` | `void` | Registers an argument with name, help text, requirement flag, type, and default value. |
| `cargs_parse(cparser *parser)` | `void` | Parses command-line arguments and populates values. |
| `cargs_get_arg(cparser *parser, const char *name)` | `void*` pointer to value, `NULL` if not found | Retrieves parsed value by argument name. |
| `cargs_free(cparser *parser)` | `void` | Frees all allocated parser memory. |

### Argument Types

| Type | Enum | CLI Format | Storage |
|------|------|------------|---------|
| **Integer** | `CARG_TYPE_INT` | `--port 8080` | `int` |
| **Float** | `CARG_TYPE_FLOAT` | `--threshold 0.5` | `float` |
| **String** | `CARG_TYPE_STRING` | `--name "value"` | `char*` |
| **Boolean** | `CARG_TYPE_BOOL` | `--verbose` (flag) | `int` (0 or 1) |

### Argument Naming

Arguments automatically support both forms:
- Long form: `--argument-name`
- Short form: `-a` (first letter)

---

## Memory Architecture

### Structure Breakdown

```c
typedef struct carg {
    const char* large_name;  // Long argument name
    const char* short_name;  // Short argument name
    const char* help;        // Help text
    char metadata;           // Flags (required, parsed, no_default)
    carg_type type;          // Argument type enum
    void* value;             // Pointer to value storage
} carg;

typedef struct cparser {
    int argc;                // Argument count
    char **argv;             // Argument vector
    carg *parsed_args;       // Array of registered arguments
    int num_parsed;          // Number of registered arguments
} cparser;
```

| Component | Size (bytes) | Description |
|-----------|--------------|-------------|
| `cparser` struct | 32 | Parser state |
| `carg` struct | 48 | Per-argument metadata |
| String storage | varies | Help text and names |

### Memory Growth Formula

Total memory $M$ for $n$ arguments:

$$M(n) = S_{parser} + n \times S_{carg}$$

Where:
- $S_{parser} = 32$ bytes
- $S_{carg} = 48$ bytes per argument

### Scaling Behavior

- **Space Complexity**: $O(n)$ where $n$ is argument count
- **Parse Complexity**: $O(argc \times n)$ linear scan
- **Lookup Complexity**: $O(n)$ linear search by name

---

## Build & Usage

### Compilation

```bash
# Build library
make libs

# Link in your project
gcc myprogram.c -Iinclude -Llib -lcargs -o myprogram

# Set library path
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```

### Metadata Flags

```c
// Internal flags (bitfield in metadata)
#define __CARGS_REQUIRED_BIT 0    // Argument is required
#define __CARGS_PARSED_BIT 1      // Argument was found and parsed
#define __CARGS_NO_DEFAULT_BIT 2  // No default value provided
```

> [!NOTE]
> Boolean arguments are flags that don't require a value. Their presence sets them to 1 (true).

> [!WARNING]
> String arguments point to `argv` memory. Do not free or modify them directly. Copy if you need to persist beyond `main()`.
