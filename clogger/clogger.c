#include <cs/clogger.h>

cs_codes clogger_init(clogger *logger, const char *filename, clogger_options options) {
    CS_RETURN_IF(logger == NULL || filename == NULL || options.modes == NULL, CS_NULL);
    logger->fp = fopen(filename, options.modes);
    logger->options = options;
    logger->mutex = NULL;
    CS_RETURN_IF(logger->fp == NULL, CS_MEM);
    
    if (options.thread_safe) {
        logger->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        if (logger->mutex == NULL) {
            fclose(logger->fp);
            logger->fp = NULL;
            return CS_MEM;
        }
        if (pthread_mutex_init(logger->mutex, NULL) != 0) {
            free(logger->mutex);
            logger->mutex = NULL;
            fclose(logger->fp);
            logger->fp = NULL;
            return CS_MEM;
        }
    }

    return CS_SUCCESS;
}

void clogger_close(clogger *logger) {
    if (logger != NULL) {
        if (logger->mutex != NULL) {
            pthread_mutex_destroy(logger->mutex);
            free(logger->mutex);
            logger->mutex = NULL;
        }
        if (logger->fp != NULL) {
            fclose(logger->fp);
            logger->fp = NULL;
        }
    }
}

void clogger_print_stacktrace(clogger logger) {
    if (logger.fp == NULL) return;
    
    void *buffer[__CLOGGER_MAX_STACK_FRAMES];
    int nframes = backtrace(buffer, __CLOGGER_MAX_STACK_FRAMES);
    char **symbols = backtrace_symbols(buffer, nframes);
    
    if (symbols == NULL) {
        fprintf(logger.fp, "  [Stack trace unavailable]\n");
        return;
    }
    
    fprintf(logger.fp, "  Stack trace:\n");
    /* Skip first 2 frames (clogger_print_stacktrace and clogger_log macro expansion) */
    for (int i = 2; i < nframes; i++) {
        fprintf(logger.fp, "    #%d %s\n", i - 2, symbols[i]);
    }
    
    free(symbols);
}