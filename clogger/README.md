# CLogger — Lightweight File Logger

A minimal, zero-dependency file logging utility in C with automatic function name capture and four severity levels.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Data Structures](#data-structures)
  - [Initialization & Cleanup](#initialization--cleanup)
  - [Logging Operations](#logging-operations)
- [Log Output Format](#log-output-format)
- [Memory Architecture](#memory-architecture)
- [Build & Usage](#build--usage)

---

## Quick Start

```c
#include <cs/clogger.h>

void process_data(clogger *log) {
    clogger_log(*log, CLOGGER_DEBUG, "Processing started\n");
    // ... work ...
    clogger_log(*log, CLOGGER_INFO, "Processed %d items\n", 42);
}

int main(void) {
    clogger log;
    clogger_init(&log, "app.log", "w");

    clogger_log(log, CLOGGER_INFO, "Application started\n");
    process_data(&log);
    clogger_log(log, CLOGGER_WARNING, "Low memory: %d bytes remaining\n", 1024);

    clogger_close(&log);
    return 0;
}
```

**Output in `app.log`:**
```
main: [INFO]Application started
process_data: [DEBUG]Processing started
process_data: [INFO]Processed 42 items
main: [WARNING]Low memory: 1024 bytes remaining
```

---

## API Reference

### Data Structures

```c
typedef enum clogger_level {
    CLOGGER_DEBUG,      /* Detailed debugging information */
    CLOGGER_INFO,       /* General informational messages */
    CLOGGER_WARNING,    /* Warning conditions */
    CLOGGER_ERROR       /* Error conditions */
} clogger_level;

typedef struct clogger {
    FILE *fp;           /* File pointer for log output */
} clogger;
```

### Initialization & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `clogger_init` | `cs_codes clogger_init(clogger *logger, const char *filename, const char *modes)` | `CS_SUCCESS`, `CS_NULL`, or `CS_MEM` | Opens file for logging; `modes` follows `fopen()` semantics ("w", "a", etc.) |
| `clogger_close` | `void clogger_close(clogger *logger)` | void | Closes log file and sets `fp` to NULL |

**Return codes:**
- `CS_SUCCESS` — Initialization successful
- `CS_NULL` — One or more arguments is NULL
- `CS_MEM` — File could not be opened

### Logging Operations

| Macro | Signature | Description |
|-------|-----------|-------------|
| `clogger_log` | `clogger_log(logger, level, fmt, ...)` | Writes formatted message to log file with automatic function name capture |

**Parameters:**
- `logger` — `clogger` struct (passed by value)
- `level` — One of `CLOGGER_DEBUG`, `CLOGGER_INFO`, `CLOGGER_WARNING`, `CLOGGER_ERROR`
- `fmt` — printf-style format string
- `...` — Format arguments

**Behavior:**
- Automatically captures calling function name via `__func__`
- Flushes output after each write (no buffering)
- No-op if `logger.fp` is NULL

---

## Log Output Format

Each log entry follows this format:

```
<function_name>: [<LEVEL>]<message>
```

| Level | Tag |
|-------|-----|
| `CLOGGER_DEBUG` | `[DEBUG]` |
| `CLOGGER_INFO` | `[INFO]` |
| `CLOGGER_WARNING` | `[WARNING]` |
| `CLOGGER_ERROR` | `[ERROR]` |

**Example output:**
```
main: [INFO]Server starting on port 8080
handle_request: [DEBUG]Received request from 192.168.1.1
handle_request: [WARNING]Request timeout after 30s
cleanup: [ERROR]Failed to close connection: fd=5
```

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `fp` | `FILE *` | 8 bytes | Pointer to open file stream |

**Total**: `sizeof(clogger)` = 8 bytes

### Resource Management

- `clogger_init` opens a file handle (consumes 1 file descriptor)
- `clogger_close` releases the file descriptor
- Always call `clogger_close` before program exit to ensure logs are flushed

---

## Build & Usage

### Compilation

```bash
# Build the library
make libs

# Link against libclogger
gcc -o myprogram myprogram.c -Iinclude -Llib -lclogger
```

### Runtime (shared library)

```bash
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./myprogram
```

### Example: Multi-level Logging

```c
#include <cs/clogger.h>

void database_query(clogger *log, const char *query) {
    clogger_log(*log, CLOGGER_DEBUG, "Executing: %s\n", query);
    
    // Simulate query
    int rows = 150;
    
    if (rows > 100) {
        clogger_log(*log, CLOGGER_WARNING, "Large result set: %d rows\n", rows);
    }
    clogger_log(*log, CLOGGER_INFO, "Query completed, %d rows returned\n", rows);
}

int main(void) {
    clogger log;
    
    if (clogger_init(&log, "debug.log", "a") != CS_SUCCESS) {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }

    clogger_log(log, CLOGGER_INFO, "=== Session started ===\n");
    database_query(&log, "SELECT * FROM users");
    clogger_log(log, CLOGGER_INFO, "=== Session ended ===\n");

    clogger_close(&log);
    return 0;
}
```

