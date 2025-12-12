#include <cs/cargs.h>
#include <stddef.h>

void cargs_init(cparser* parser, int argc, char** argv) {
    parser->argc = argc;
    parser->argv = argv;
    parser->parsed_args = NULL;
    parser->num_parsed = 0;
}

void cargs_add_arg(cparser* parser, const char* name, const char* help, char required, carg_type type, void* default_value) {
    // Implementation for adding an argument to the parser
    // This is a placeholder implementation
    parser->num_parsed += 1;
    parser->parsed_args = realloc(parser->parsed_args, parser->num_parsed * sizeof(carg));
    carg* new_arg = &parser->parsed_args[parser->num_parsed - 1];
    new_arg->name = name;
    new_arg->help = help;
    new_arg->required = required;
    new_arg->type = type;
    new_arg->value = default_value;
}