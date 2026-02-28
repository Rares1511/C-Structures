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
    const char* large_name;  /*!< The long name of the argument (e.g., "output") */
    const char* short_name;  /*!< The short name of the argument (e.g., "o") */
    const char* help;        /*!< Help description for the argument */
    char metadata;           /*!< Metadata flags: bit 0 = required, bit 1 = parsed, bit 2 = no default value */
    carg_type type;          /*!< The type of the argument (int, float, string, bool) */
    void* value;             /*!< Pointer to the variable where the parsed value will be stored */
} carg;

typedef struct {
    int argc;            /*!< Original argument count */
    char **argv;         /*!< Original command-line arguments */
    carg *parsed_args;   /*!< Array of parsed arguments */
    int num_parsed;      /*!< Number of parsed arguments */
} cparser;

/*!
 * Initializes the argument parser with the original command-line arguments.
 * @param parser Pointer to the cparser structure to initialize.
 * @param argc The original argument count from main().
 * @param argv The original argument vector from main().
 */
void cargs_init(cparser* parser, int argc, char** argv);

/*!
 * Adds an argument definition to the parser.
 * @param parser Pointer to the cparser structure.
 * @param name The long name of the argument (e.g., "output").
 * @param help A help description for the argument.
 * @param required Whether the argument is required (1 for required, 0 for optional).
 * @param type The type of the argument (CARG_TYPE_INT, CARG_TYPE_FLOAT, CARG_TYPE_STRING, CARG_TYPE_BOOL).
 * @param default_value Pointer to the default value for the argument (can be NULL if no default).
 */
void cargs_add_arg(cparser* parser, const char* name, const char* help, char required, carg_type type, void* default_value);

/*!
 * Retrieves the value of a parsed argument by its long name.
 * @param parser Pointer to the cparser structure.
 * @param name The long name of the argument to retrieve.
 * @return Pointer to the value of the argument, or NULL if not found or not parsed.
 */
void* cargs_get_arg(cparser* parser, const char* name);

/*!
 * Parses the command-line arguments based on the defined argument specifications.
 * @param parser Pointer to the cparser structure to parse.
 */
void cargs_parse(cparser* parser);

/*!
 * Frees any resources allocated by the argument parser.
 * @param parser Pointer to the cparser structure to free.
 */
void cargs_free(cparser* parser);

#endif