#include <cs/clogger.h>

cs_codes clogger_init(clogger *logger, const char *filename, clogger_options options) {
    CS_RETURN_IF(logger == NULL || filename == NULL || options.modes == NULL, CS_NULL);
    logger->fp = fopen(filename, options.modes);
    logger->options = options;
    CS_RETURN_IF(logger->fp == NULL, CS_MEM);
    return CS_SUCCESS;
}

void clogger_close(clogger *logger) {
    if (logger != NULL && logger->fp != NULL) {
        fclose(logger->fp);
        logger->fp = NULL;
    }
}