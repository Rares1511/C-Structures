# CLogger — Lightweight File Logger

A minimal file logging utility in C with automatic function name capture, four severity levels, and optional thread safety.

---

## Table of Contents

- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Data Structures](#data-structures)
  - [Initialization & Cleanup](#initialization--cleanup)
  - [Logging Operations](#logging-operations)
- [Log Output Format](#log-output-format)
- [Thread Safety](#thread-safety)
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
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,  // Show time, file, func, line, and level
        .thread_safe = 0            // Disable thread safety (no mutex overhead)
    };
    clogger_init(&log, "app.log", opts);

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

/* Flags to control which metadata is displayed in log messages */
typedef enum clogger_flags {
    CLOGGER_SHOW_NONE = 0,       /* Show no metadata, only the log message */
    CLOGGER_SHOW_TIME = 1 << 0,  /* Show timestamp (HH:MM:SS.mmm) */
    CLOGGER_SHOW_FILE = 1 << 1,  /* Show source file name */
    CLOGGER_SHOW_FUNC = 1 << 2,  /* Show function name */
    CLOGGER_SHOW_LINE = 1 << 3,  /* Show line number */
    CLOGGER_SHOW_ALL  = CLOGGER_SHOW_TIME | CLOGGER_SHOW_FILE | 
                        CLOGGER_SHOW_FUNC | CLOGGER_SHOW_LINE
} clogger_flags;

typedef struct clogger_options {
    clogger_level min_level;  /* Minimum logging level to log */
    const char *modes;        /* File opening modes ("w", "a", etc.) */
    clogger_flags flags;      /* Flags controlling which metadata to display */
    int thread_safe;          /* Enable thread-safe logging (0 = disabled) */
} clogger_options;

typedef struct clogger {
    FILE *fp;                  /* File pointer for log output */
    clogger_options options;   /* Logging options */
    pthread_mutex_t *mutex;    /* Mutex for thread safety (NULL if disabled) */
} clogger;
```

**Flag Combinations:**

| Flags | Output Format |
|-------|--------------|
| `CLOGGER_SHOW_NONE` | `message` |
| `CLOGGER_SHOW_LEVEL` | `[LEVEL  ] message` |
| `CLOGGER_SHOW_TIME \| CLOGGER_SHOW_LEVEL` | `HH:MM:SS.mmm [LEVEL  ] message` |
| `CLOGGER_SHOW_ALL` | `HH:MM:SS.mmm [LEVEL  ] file.c:func():123 message` |

### Initialization & Cleanup

| Function | Signature | Return | Description |
|----------|-----------|--------|-------------|
| `clogger_init` | `cs_codes clogger_init(clogger *logger, const char *filename, clogger_options options)` | `CS_SUCCESS`, `CS_NULL`, or `CS_MEM` | Opens file for logging with specified options |
| `clogger_close` | `void clogger_close(clogger *logger)` | void | Closes log file, destroys mutex (if enabled), and sets pointers to NULL |

**Return codes:**
- `CS_SUCCESS` — Initialization successful
- `CS_NULL` — One or more arguments is NULL
- `CS_MEM` — File could not be opened or mutex allocation failed

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

The log entry format depends on the `clogger_flags` set in options. With `CLOGGER_SHOW_ALL`:

```
HH:MM:SS.mmm [LEVEL  ] file.c:func():line message
```

With `CLOGGER_SHOW_LEVEL` only:

```
[LEVEL  ] message
```

With `CLOGGER_SHOW_NONE` (minimal output):

```
message
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

## Thread Safety

The logger supports optional thread-safe operation via POSIX mutexes. Enable it by setting `thread_safe = 1` in options:

```c
clogger_options opts = {
    .min_level = CLOGGER_DEBUG,
    .modes = "a",
    .flags = CLOGGER_SHOW_ALL,
    .thread_safe = 1  // Enable mutex protection
};
```

**Behavior when enabled:**
- A mutex is allocated and initialized during `clogger_init()`
- Each `clogger_log()` call acquires the mutex before writing and releases it after
- `clogger_close()` destroys and frees the mutex

**Performance note:** Thread safety adds overhead from mutex lock/unlock operations. Disable it for single-threaded applications.

---

## Memory Architecture

### Structure Layout (64-bit)

| Field | Type | Size | Description |
|-------|------|------|-------------|
| `fp` | `FILE *` | 8 bytes | Pointer to open file stream |
| `options.min_level` | `clogger_level` | 4 bytes | Minimum log level |
| `options.modes` | `const char *` | 8 bytes | File opening mode string |
| `options.flags` | `clogger_flags` | 4 bytes | Display flags |
| `options.thread_safe` | `int` | 4 bytes | Thread safety flag |
| `mutex` | `pthread_mutex_t *` | 8 bytes | Mutex pointer (NULL if disabled) |

### Resource Management

- `clogger_init` opens a file handle (consumes 1 file descriptor)
- If `thread_safe` is enabled, allocates and initializes a pthread mutex
- `clogger_close` releases the file descriptor and mutex (if allocated)
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
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "a",
        .flags = CLOGGER_SHOW_TIME | CLOGGER_SHOW_FUNC | CLOGGER_SHOW_LEVEL,
        .thread_safe = 0  // Single-threaded, no mutex needed
    };
    
    if (clogger_init(&log, "debug.log", opts) != CS_SUCCESS) {
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

