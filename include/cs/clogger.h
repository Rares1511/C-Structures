#ifndef __CLOGGER_H__
#define __CLOGGER_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <cs/universal.h>

typedef enum clogger_level {
    CLOGGER_DEBUG,
    CLOGGER_INFO,
    CLOGGER_WARNING,
    CLOGGER_ERROR
} clogger_level;

typedef struct clogger_options {
    clogger_level min_level; // Minimum logging level to log
    const char *modes;
} clogger_options;

typedef struct clogger {
    FILE *fp;
    clogger_options options;
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
 * Logs a formatted message to the file associated with the logger.
 * @param logger Pointer to the clogger structure.
 * @param level The logging level (DEBUG, INFO, WARNING, ERROR).
 * @param fmt The format string (like printf).
 * @param ... Additional arguments for the format string.
 */
#define clogger_log(logger, level, fmt, ...) \
    do { \
        if ((logger).fp != NULL && (logger).options.min_level <= (level)) { \
            time_t _clog_now = time(NULL); \
            struct tm *_clog_tm = localtime(&_clog_now); \
            fprintf((logger).fp, "%02d:%02d:%02d.%03ld [%-7s] %s:%s():%d - " fmt, \
                _clog_tm->tm_hour, _clog_tm->tm_min, _clog_tm->tm_sec, \
                (long)(_clog_now % 1000), \
                (level) == CLOGGER_DEBUG ? "DEBUG" : \
                (level) == CLOGGER_INFO ? "INFO" : \
                (level) == CLOGGER_WARNING ? "WARNING" : "ERROR", \
                __FILE__, __func__, __LINE__, \
                ##__VA_ARGS__); \
            fflush((logger).fp); \
        } \
    } while(0)

/*!
 * Closes the logger by closing the associated file.
 * @param logger Pointer to the clogger structure to close.
 */
void clogger_close(clogger *logger);

#endif 