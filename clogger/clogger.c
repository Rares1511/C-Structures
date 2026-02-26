#include <cs/clogger.h>

cs_codes clogger_init(clogger *logger, const char *filename, const char *modes) {
    CS_RETURN_IF(logger == NULL || filename == NULL || modes == NULL, CS_NULL);
    logger->fp = fopen(filename, modes);
    CS_RETURN_IF(logger->fp == NULL, CS_MEM);
    return CS_SUCCESS;
}

void clogger_log(clogger logger, clogger_level level, const char *fmt, ...) {
    if (logger.fp != NULL && fmt != NULL) {
        va_list args;
        va_start(args, fmt);
        switch(level) {
            case CLOGGER_DEBUG:
                fprintf(logger.fp, "[DEBUG]");
                break;
            case CLOGGER_INFO:
                fprintf(logger.fp, "[INFO]");
                break;
            case CLOGGER_WARNING:
                fprintf(logger.fp, "[WARNING]");
                break;
            case CLOGGER_ERROR:
                fprintf(logger.fp, "[ERROR]");
                break;
        }
        vfprintf(logger.fp, fmt, args);
        va_end(args);
        fflush(logger.fp);
    }
}

void clogger_close(clogger *logger) {
    if (logger != NULL && logger->fp != NULL) {
        fclose(logger->fp);
        logger->fp = NULL;
    }
}