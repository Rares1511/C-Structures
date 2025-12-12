#ifndef _CARGS_H
#define _CARGS_H

typedef enum {
    CARG_TYPE_INT,
    CARG_TYPE_FLOAT,
    CARG_TYPE_STRING,
    CARG_TYPE_BOOL
} carg_type;

typedef struct carg {
    const char* name;
    const char* help;
    char required;
    carg_type type;
    void* value;
} carg;

typedef struct {
    int argc;
    char **argv;
    carg *parsed_args;
    int num_parsed;
} cparser;

void cargs_init(cparser* parser, int argc, char** argv);
void cargs_add_arg(cparser* parser, const char* name, const char* help, char required, carg_type type, void* default_value);
void cargs_parse(cparser* parser);
void cargs_free(cparser* parser);

#endif