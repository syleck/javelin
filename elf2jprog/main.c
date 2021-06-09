#include "jprog.h"
#include <elf.h>
#include <argp.h>

// I GOTTA FINISH THIS BUT IM LAZY AS HELL

const char *argp_program_version = "elf2jprog 1.0";
const char *argp_program_bug_address = "<watergatchi@protonmail.com>";
static char doc[] = "Converts ELF executables to JPROG executables.";
static char args_doc[] = "ELFINPUT";

static struct argp_option options[] = { 
    { "no-relocate", 'n', 0, 0, "Do not mark the jprog as relocatable (not compiling gcc with -fPIC, for example)."},
    { "output-file", 'o', "OUTFILE", 0, "Output to specified file."},
    { "start-sym", 's', "SYMBOL", 0, "Set start symbol name to SYMBOL."},
    { 0 } 
};

struct arguments {
    enum { NOT_RELOCATABLE, RELOCATABLE } mode;
    char* input_file;
    char* output_file;
    char* start_sym;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'n': arguments->mode = NOT_RELOCATABLE; break;
    case 'o': arguments->output_file = arg; break;
    case 's': arguments->start_sym = arg; break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
        {
            argp_usage(state);
        }
        arguments->input_file = arg;
        break;
    default: return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char** argv) {
    struct arguments arguments;
    arguments.mode = RELOCATABLE;
    arguments.start_sym = "_start";
    arguments.output_file = "out.jpr";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    printf("Writing %s to %s...\n",arguments.input_file,arguments.output_file);
    if(arguments.mode == NOT_RELOCATABLE) {
        printf("Warning: Non relocating executables may not be supported by Javelin\n");
    }
}