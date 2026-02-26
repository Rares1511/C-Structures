#ifndef __CLOGGER_H__
#define __CLOGGER_H__

#include <stdio.h>
#include <stdarg.h>
#include <cs/universal.h>

typedef enum clogger_level {
    CLOGGER_DEBUG,
    CLOGGER_INFO,
    CLOGGER_WARNING,
    CLOGGER_ERROR
} clogger_level;

typedef struct clogger {
    FILE *fp;
} clogger;

/*!
 * Initializes the logger by opening the specified file for writing.
 * @param logger Pointer to the clogger structure to initialize.
 * @param filename The name of the file to which logs will be written.
 * @param modes The file modes string (e.g., "w", "a").
 * @return CS_SUCCESS on success, CS_NULL if logger, filename, or modes is NULL, CS_MEM if file opening fails.
 */
cs_codes clogger_init(clogger *logger, const char *filename, const char *modes);

/*!
 * Logs a formatted message to the file associated with the logger.
 * @param logger Pointer to the clogger structure.
 * @param level The logging level (DEBUG, INFO, WARNING, ERROR).
 * @param fmt The format string (like printf).
 * @param ... Additional arguments for the format string.
 */
void clogger_log(clogger logger, clogger_level level, const char *fmt, ...);

/*!
 * Closes the logger by closing the associated file.
 * @param logger Pointer to the clogger structure to close.
 */
void clogger_close(clogger *logger);

#endif 