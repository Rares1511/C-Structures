#include <cs/cargs.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char* cargs_type_metavar(carg_type t) {
    switch (t) {
        case CARG_TYPE_INT:    return "INT";
        case CARG_TYPE_FLOAT:  return "FLOAT";
        case CARG_TYPE_STRING: return "STR";
        case CARG_TYPE_BOOL:   return "";      // flags have no metavar
        default:               return "VAL";
    }
}

static void cargs_build_usage(char* out, size_t out_sz, cparser* parser) {
    // usage: prog [-h] [--count INT] [--name STR] ...
    size_t used = 0;
    used += (size_t)snprintf(out + used, out_sz - used,
                             "usage: %s", parser->argv[0]);

    for (int i = 0; i < parser->num_parsed; i++) {
        carg* a = &parser->parsed_args[i];

        const int required = (a->metadata & (1 << __CARGS_REQUIRED_BIT)) != 0;
        const char* mv = cargs_type_metavar(a->type);

        if (a->type == CARG_TYPE_BOOL) {
            used += (size_t)snprintf(out + used, out_sz - used,
                                     required ? " %s" : " [%s]",
                                     a->large_name);
        } else {
            used += (size_t)snprintf(out + used, out_sz - used,
                                     required ? " %s %s" : " [%s %s]",
                                     a->large_name, mv);
        }

        if (used >= out_sz) break;
    }
}

void cargs_help(cparser* parser) {
    // ---------- usage line ----------
    char usage[2048];
    usage[0] = '\0';
    cargs_build_usage(usage, sizeof(usage), parser);
    puts(usage);
    puts("");

    // argparse uses "options:" nowadays
    puts("options:");

    // ---------- compute column width for alignment ----------
    // left column example: "-c, --count INT" or "-h, --help"
    size_t left_w = 0;
    for (int i = 0; i < parser->num_parsed; i++) {
        carg* a = &parser->parsed_args[i];
        const char* mv = cargs_type_metavar(a->type);

        char left[256];
        if (a->short_name && a->short_name[0]) {
            if (a->type == CARG_TYPE_BOOL) {
                snprintf(left, sizeof(left), "%s, %s", a->short_name, a->large_name);
            } else {
                snprintf(left, sizeof(left), "%s, %s %s", a->short_name, a->large_name, mv);
            }
        } else {
            if (a->type == CARG_TYPE_BOOL) {
                snprintf(left, sizeof(left), "%s", a->large_name);
            } else {
                snprintf(left, sizeof(left), "%s %s", a->large_name, mv);
            }
        }

        size_t lw = strlen(left);
        if (lw > left_w) left_w = lw;
    }

    // clamp width so huge option names don’t destroy formatting
    if (left_w > 40) left_w = 40;

    // ---------- print options ----------
    for (int i = 0; i < parser->num_parsed; i++) {
        carg* a = &parser->parsed_args[i];
        const int required = (a->metadata & (1 << __CARGS_REQUIRED_BIT)) != 0;
        const char* mv = cargs_type_metavar(a->type);

        char left[256];
        if (a->short_name && a->short_name[0]) {
            if (a->type == CARG_TYPE_BOOL) {
                snprintf(left, sizeof(left), "%s, %s", a->short_name, a->large_name);
            } else {
                snprintf(left, sizeof(left), "%s, %s %s", a->short_name, a->large_name, mv);
            }
        } else {
            if (a->type == CARG_TYPE_BOOL) {
                snprintf(left, sizeof(left), "%s", a->large_name);
            } else {
                snprintf(left, sizeof(left), "%s %s", a->large_name, mv);
            }
        }

        // right side: help + annotations like "(required)".
        // Default printing is tricky because you store raw memory; we do safe minimal hints.
        char right[512];
        if (required) {
            snprintf(right, sizeof(right), "%s (required)", a->help ? a->help : "");
        } else {
            snprintf(right, sizeof(right), "%s", a->help ? a->help : "");
        }

        // argparse-like indentation: 2 spaces before flags, then aligned description
        // If left text is too long, wrap description on next line.
        if (strlen(left) > left_w) {
            printf("  %s\n", left);
            printf("  %*s  %s\n", (int)left_w, "", right);
        } else {
            printf("  %-*s  %s\n", (int)left_w, left, right);
        }
    }
}

char* cargs_compute_short_name(const char* large_name) {
    if (!large_name) return NULL;

    size_t len = strlen(large_name);
    if (len < 3 || strncmp(large_name, __CARGS_LARGE_PREFIX, 2) != 0) {
        return NULL;
    }

    // Count how many letters we'll take: 1 per word after "--"
    size_t letters = 0;
    for (const char* p = large_name + 2; *p; ) {
        // start of a word
        if (*p != '-') {
            letters++;
            // skip to next dash or end
            while (*p && *p != '-') p++;
        } else {
            p++;
        }
    }

    // "-" + letters + '\0'
    char* short_name = malloc(1 + letters + 1);
    if (!short_name) return NULL;

    short_name[0] = __CARGS_SMALL_PREFIX[0];
    size_t idx = 1;

    for (const char* p = large_name + 2; *p; ) {
        if (*p != '-') {
            short_name[idx++] = *p;           // first char of the word
            while (*p && *p != '-') p++;
        } else {
            p++;
        }
    }
    short_name[idx] = '\0';
    return short_name;
}

void cargs_init(cparser* parser, int argc, char** argv) {
    parser->argc = argc;
    parser->argv = argv;
    parser->parsed_args = NULL;
    parser->num_parsed = 0;
    cargs_add_arg(parser, "--help", "Display this help message", 0, CARG_TYPE_BOOL, NULL);
}

void cargs_add_arg(cparser* parser, const char* name, const char* help, char required, carg_type type, void* default_value) {
    parser->num_parsed += 1;
    parser->parsed_args = realloc(parser->parsed_args, parser->num_parsed * sizeof(carg));
    carg* new_arg = &parser->parsed_args[parser->num_parsed - 1];
    new_arg->large_name = name;
    new_arg->short_name = cargs_compute_short_name(name);
    new_arg->help = help;
    new_arg->metadata = 0;
    if (required) {
        new_arg->metadata |= (1 << __CARGS_REQUIRED_BIT);
    }
    new_arg->type = type;
    switch (type) {
        case CARG_TYPE_INT:
            new_arg->value = malloc(sizeof(int));
            if (default_value != NULL) {
                *(int*)(new_arg->value) = *(int*)default_value;
            }
            break;
        case CARG_TYPE_FLOAT:
            new_arg->value = malloc(sizeof(float));
            if (default_value != NULL) {
                *(float*)(new_arg->value) = *(float*)default_value;
            }
            break;
        case CARG_TYPE_STRING:
            if (default_value) {
                new_arg->value = malloc(__CARGS_STRING_MAX_LEN * sizeof(char));
                if (default_value != NULL) {
                    strcpy((char*)(new_arg->value), (char*)default_value);
                }
            }
            break;
        case CARG_TYPE_BOOL:
            new_arg->value = malloc(sizeof(char));
            if (default_value != NULL) {
                *(char*)(new_arg->value) = *(char*)default_value;
            } else {
                *(char*)(new_arg->value) = 0; // Default to false
            }
            break;
        default:
            new_arg->value = NULL;
            break;
    }
}

void* cargs_get_arg(cparser* parser, const char* name) {
    char large_name[__CARGS_STRING_MAX_LEN] = "";
    if (strncmp(name, __CARGS_LARGE_PREFIX, 2) != 0) {
        strcat(large_name, __CARGS_LARGE_PREFIX);
    }
    strcat(large_name, name);
    for (int i = 0; i < parser->num_parsed; i++) {
        if (strcmp(parser->parsed_args[i].large_name, large_name) == 0) {
            return parser->parsed_args[i].value;
        }
    }
    return NULL; // Argument not found
}

void cargs_parse(cparser* parser) {
    for (int i = 1; i < parser->argc; i++) {
        if (strcmp(parser->argv[i], "--help") == 0 || strcmp(parser->argv[i], "-h") == 0) {
            cargs_help(parser);
            cargs_free(parser);
            exit(EXIT_SUCCESS);
        }
        for (int j = 1; j < parser->num_parsed; j++) {
            if (parser->parsed_args[j].metadata & (1 << __CARGS_PARSED_BIT)) {
                continue; // Already parsed
            }
            if (strcmp(parser->argv[i], parser->parsed_args[j].large_name) == 0 || strcmp(parser->argv[i], parser->parsed_args[j].short_name) == 0) {
                switch(parser->parsed_args[j].type) {
                    case CARG_TYPE_BOOL:
                        *(char*)(parser->parsed_args[j].value) = 1;
                        parser->parsed_args[j].metadata |= (1 << __CARGS_PARSED_BIT);
                        break;
                    default:
                        // Expect a value in the next argument
                        if (i + 1 < parser->argc) {
                            switch (parser->parsed_args[j].type) {
                                case CARG_TYPE_INT:
                                    *(int*)(parser->parsed_args[j].value) = atoi(parser->argv[i + 1]);
                                    break;
                                case CARG_TYPE_FLOAT:
                                    *(float*)(parser->parsed_args[j].value) = atof(parser->argv[i + 1]);
                                    break;
                                case CARG_TYPE_STRING:
                                    if (parser->parsed_args[j].value == NULL) {
                                        parser->parsed_args[j].value = malloc(__CARGS_STRING_MAX_LEN * sizeof(char));
                                    }
                                    strcpy((char*)(parser->parsed_args[j].value), parser->argv[i + 1]);
                                    break;
                                default:
                                    break;
                            }
                            parser->parsed_args[j].metadata |= (1 << __CARGS_PARSED_BIT);
                            i++; // Skip the value argument
                        }
                        break;
                }
            }
        }
    }

    // Check for required arguments
    for (int j = 0; j < parser->num_parsed; j++) {
        if ((parser->parsed_args[j].metadata & (1 << __CARGS_REQUIRED_BIT)) &&
            !(parser->parsed_args[j].metadata & (1 << __CARGS_PARSED_BIT))) {
            cargs_help(parser);
            cargs_free(parser);
            exit(EXIT_FAILURE);
        }
    }
}

void cargs_free(cparser* parser) {
    for (int i = 0; i < parser->num_parsed; i++) {
        free(parser->parsed_args[i].value);
        free((void*)parser->parsed_args[i].short_name);
    }
    free(parser->parsed_args);
    parser->parsed_args = NULL;
    parser->num_parsed = 0;
}