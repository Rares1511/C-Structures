#ifndef __CS_CARGS_H__
#define __CS_CARGS_H__

#define __CARGS_REQUIRED_BIT 0
#define __CARGS_PARSED_BIT 1
#define __CARGS_NO_DEFAULT_BIT 2

#define __CARGS_LARGE_PREFIX "--"
#define __CARGS_SMALL_PREFIX "-"

#define __CARGS_STRING_MAX_LEN 256

typedef enum {
    CARG_TYPE_INT,
    CARG_TYPE_FLOAT,
    CARG_TYPE_STRING,
    CARG_TYPE_BOOL
} carg_type;

typedef struct carg {
    const char* large_name;
    const char* short_name;
    const char* help;
    char metadata;
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
void* cargs_get_arg(cparser* parser, const char* name);
void cargs_parse(cparser* parser);
void cargs_free(cparser* parser);

#endif