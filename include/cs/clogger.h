#ifndef __CLOGGER_H__
#define __CLOGGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <execinfo.h>
#include <cs/universal.h>

#define __MAX_TEMP_BUFFER_SIZE 1024
#define __MAX_BUFFER_SIZE __MAX_TEMP_BUFFER_SIZE * 100

#define __CLOGGER_MAX_STACK_FRAMES 64

typedef enum clogger_level {
    CLOGGER_DEBUG,
    CLOGGER_INFO,
    CLOGGER_WARNING,
    CLOGGER_ERROR
} clogger_level;

/*!
 * Flags to control which metadata is displayed in log messages.
 * Multiple flags can be combined using bitwise OR (e.g., CLOGGER_SHOW_TIME | CLOGGER_SHOW_FILE).
 */
typedef enum clogger_flags {
    CLOGGER_SHOW_NONE = 0,        /*!< Show no metadata, only the log message */
    CLOGGER_SHOW_TIME = 1 << 0,   /*!< Show timestamp (HH:MM:SS.mmm) */
    CLOGGER_SHOW_FILE = 1 << 1,   /*!< Show source file name */
    CLOGGER_SHOW_FUNC = 1 << 2,   /*!< Show function name */
    CLOGGER_SHOW_LINE = 1 << 3,   /*!< Show line number */
    CLOGGER_SHOW_LEVEL = 1 << 4,  /*!< Show log level (DEBUG, INFO, etc.) */
    CLOGGER_SHOW_STACKTRACE = 1 << 5, /*!< Show stack trace on ERROR level */
    CLOGGER_SHOW_ALL  = CLOGGER_SHOW_TIME  | 
                        CLOGGER_SHOW_FILE  | 
                        CLOGGER_SHOW_FUNC  | 
                        CLOGGER_SHOW_LINE  | 
                        CLOGGER_SHOW_LEVEL |
                        CLOGGER_SHOW_STACKTRACE /*!< Show all metadata */
} clogger_flags;

typedef struct clogger_options {
    clogger_level min_level; /*!< Minimum logging level to log */ 
    const char *modes;       /*!< File opening modes (e.g., "w" for write, "a" for append) */
    clogger_flags flags;     /*!< Flags controlling which metadata to display */
    int thread_safe;         /*!< Enable thread-safe logging with mutex protection (0 = disabled, non-zero = enabled) */
} clogger_options;

typedef struct clogger {
    FILE *fp;                 /*!< File pointer to the log file */
    clogger_options options;  /*!< Logging options, including minimum log level and file opening modes */
    pthread_mutex_t *mutex;   /*!< Mutex for thread-safe logging (NULL if thread_safe is disabled) */
} clogger;

/*!
 * Initializes the logger by opening the specified file for writing.
 * @param logger Pointer to the clogger structure to initialize.
 * @param filename The name of the file to which logs will be written.
 * @param options The clogger_options structure containing logging options.
 * @return CS_SUCCESS on success, CS_NULL if logger, filename, or modes is NULL, CS_MEM if file opening fails.
 */
cs_codes clogger_init(clogger *logger, const char *filename, clogger_options options);

/*!
 * Prints the current stack trace to the logger's file.
 * @param logger Pointer to the clogger structure.
 * @note This function is typically called automatically by clogger_log on ERROR level
 *       when CLOGGER_SHOW_STACKTRACE flag is enabled.
 */
void clogger_print_stacktrace(clogger logger);

/*!
 * Logs a formatted message to the file associated with the logger.
 * @param logger Pointer to the clogger structure.
 * @param level The logging level (DEBUG, INFO, WARNING, ERROR).
 * @param fmt The format string (like printf).
 * @param ... Additional arguments for the format string.
 */
#define clogger_log(logger, level, fmt, ...) \
    do { \
        if ((logger).fp != NULL && (logger).options.min_level <= (level)) { \
            if ((logger).mutex != NULL) pthread_mutex_lock((logger).mutex); \
            clogger_flags _clog_flags = (logger).options.flags; \
            if (_clog_flags & CLOGGER_SHOW_TIME) { \
                time_t _clog_now = time(NULL); \
                struct tm *_clog_tm = localtime(&_clog_now); \
                fprintf((logger).fp, "%02d:%02d:%02d.%03ld ", \
                    _clog_tm->tm_hour, _clog_tm->tm_min, _clog_tm->tm_sec, \
                    (long)(_clog_now % 1000)); \
            } \
            if (_clog_flags & CLOGGER_SHOW_LEVEL) { \
                fprintf((logger).fp, "[%s] ", \
                    (level) == CLOGGER_DEBUG ? "DEBUG" : \
                    (level) == CLOGGER_INFO ? "INFO" : \
                    (level) == CLOGGER_WARNING ? "WARNING" : "ERROR"); \
            } \
            if (_clog_flags & CLOGGER_SHOW_FILE) { \
                fprintf((logger).fp, "%s:", __FILE__); \
            } \
            if (_clog_flags & CLOGGER_SHOW_FUNC) { \
                fprintf((logger).fp, "%s():", __func__); \
            } \
            if (_clog_flags & CLOGGER_SHOW_LINE) { \
                fprintf((logger).fp, "%d", __LINE__); \
            } \
            if (_clog_flags & (CLOGGER_SHOW_FILE | CLOGGER_SHOW_FUNC | CLOGGER_SHOW_LINE | CLOGGER_SHOW_LEVEL)) { \
                fprintf((logger).fp, " "); \
            } \
            fprintf((logger).fp, fmt, ##__VA_ARGS__); \
            if ((level) == CLOGGER_ERROR && (_clog_flags & CLOGGER_SHOW_STACKTRACE)) { \
                clogger _clog_tmp = (logger); \
                clogger_print_stacktrace(_clog_tmp); \
            } \
            fflush((logger).fp); \
            if ((logger).mutex != NULL) pthread_mutex_unlock((logger).mutex); \
        } \
    } while(0)

/*!
 * Logs an assertion failure message if the condition is false.
 * @param logger Pointer to the clogger structure.
 * @param cond The condition to assert (if false, the assertion fails).
 * @param fmt The format string for the assertion failure message.
 * @param ... Additional arguments for the format string.
 */
#define clogger_assert(logger, cond, fmt, ...) \
    do { \
        if (!(cond)) { \
            clogger_log(logger, CLOGGER_ERROR, "Assertion failed: " fmt "\n", ##__VA_ARGS__); \
        } \
    } while(0)

/*!
 * Closes the logger by closing the associated file.
 * @param logger Pointer to the clogger structure to close.
 */
void clogger_close(clogger *logger);

#endif 