#include <cs/clogger.h>

#include <signal.h>

clogger* clogger_init(const char *filename, clogger_options options) {
    CS_RETURN_IF(options.modes == NULL, NULL);
    clogger *logger = (clogger *)malloc(sizeof(clogger));
    CS_RETURN_IF(logger == NULL, NULL);
    if (filename == NULL) {
        logger->fp = stdout;
    } else {
        logger->fp = fopen(filename, options.modes);
        CS_RETURN_IF(logger->fp == NULL, NULL);
    }
    logger->options = options;
    logger->mutex = NULL;
    CS_RETURN_IF(logger->fp == NULL, NULL);
    
    if (options.thread_safe) {
        logger->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        if (logger->mutex == NULL) {
            fclose(logger->fp);
            logger->fp = NULL;
            return NULL;
        }
        if (pthread_mutex_init(logger->mutex, NULL) != 0) {
            free(logger->mutex);
            logger->mutex = NULL;
            fclose(logger->fp);
            logger->fp = NULL;
            return NULL;
        }
    }

    return logger;
}

void clogger_close(clogger *logger) {
    if (logger != NULL) {
        if (logger->mutex != NULL) {
            pthread_mutex_destroy(logger->mutex);
            free(logger->mutex);
        }
        if (logger->fp != NULL && logger->fp != stdout) {
            fclose(logger->fp);
        }
        logger->mutex = NULL;
        logger->fp = NULL;
        free(logger);
    }
}

void clogger_print_stacktrace(clogger *logger) {
    if (logger->fp == NULL) return;
    
    void *buffer[__CLOGGER_MAX_STACK_FRAMES];
    int nframes = backtrace(buffer, __CLOGGER_MAX_STACK_FRAMES);
    char **symbols = backtrace_symbols(buffer, nframes);
    
    if (symbols == NULL) {
        fprintf(logger->fp, "  [Stack trace unavailable]\n");
        return;
    }
    
    fprintf(logger->fp, "  Stack trace:\n");
    /* Skip first 2 frames (clogger_print_stacktrace and clogger_log macro expansion) */
    for (int i = 2; i < nframes; i++) {
        fprintf(logger->fp, "    #%d %s\n", i - 2, symbols[i]);
    }
    
    free(symbols);
}