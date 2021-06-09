#include "jprog.h"
#include <elf.h>
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *argp_program_version = "elf2jprog 1.0";
const char *argp_program_bug_address = "<watergatchi@protonmail.com>";
static char doc[] = "Converts ELF executables to JPROG executables.";
static char args_doc[] = "ELFINPUT";

static struct argp_option options[] = { 
    { "no-relocate", 'n', 0, 0, "Do not mark the jprog as relocatable (not compiling gcc with -fPIC, for example)."},
    { "output-file", 'o', "OUTFILE", 0, "Output to specified file."},
    { "start-sym", 's', "SYMBOL", 0, "Set start symbol name to SYMBOL."},
    { "jprog-contact", 'c', "CONTACT", 0, "Metadata: Contact address of jprog."},
    { "jprog-author", 'a', "AUTHOR", 0, "Metadata: Author of jprog."},
    { "jprog-license", 'l', "LICENSE", 0, "Metadata: Short name of license (GNU AGPLv3 becomes AGPL) for the jprog."},
    { 0 } 
};

struct arguments {
    enum { NOT_RELOCATABLE, RELOCATABLE } mode;
    char* input_file;
    char* output_file;
    char* start_sym;
    char* jauthor;
    char* jlicense;
    char* jcontact;
};

#ifdef ELF64
#define ElfW(type) Elf64_ ## type
#else
#define ElfW(type) Elf32_ ## type
#endif

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'n': arguments->mode = NOT_RELOCATABLE; break;
    case 'o': arguments->output_file = arg; break;
    case 's': arguments->start_sym = arg; break;
    case 'c': arguments->jcontact = arg; break;
    case 'l': arguments->jlicense = arg; break;
    case 'a': arguments->jauthor = arg; break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
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
    arguments.jcontact = "no@contact.info";
    arguments.jlicense = "AGPL";
    arguments.jauthor = "elf2jprog tool";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    printf("Writing %s to %s...\n",arguments.input_file,arguments.output_file);
    if(arguments.mode == NOT_RELOCATABLE) {
        printf("Warning: Non relocating executables may not be supported by Javelin\n");
    }

    int current = 0;
    void* textdata;
    FILE* elfIn = fopen(arguments.input_file,"rb");
    if(elfIn) {
        FILE* jprOut = fopen(arguments.output_file,"wb");
        if(jprOut) {
            ElfW(Ehdr) ehdr;
            fread(&ehdr,sizeof(ElfW(Ehdr)),1,elfIn);
            if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) == 0) {
                jprog_hdr jph;
                memset(&jph,0,sizeof(jprog_hdr));
                strcpy(jph.author,arguments.jauthor);
                strcpy(jph.contact,arguments.jcontact);
                strcpy(jph.license,arguments.jlicense);
                jph.machine_type = ehdr.e_machine;
                if(arguments.mode == NOT_RELOCATABLE)
                    jph.relocatable = false;
                else
                    jph.relocatable = true;
                strcpy(jph.signature,JPH_SIGNATURE);
                fwrite(&jph,sizeof(jprog_hdr),1,jprOut);
                fseek(elfIn,(ehdr.e_shentsize * ehdr.e_shstrndx) + ehdr.e_shoff,0);
                printf(".strtab:\n");
                ElfW(Shdr) strtab;
                fread(&strtab,sizeof(strtab),1,elfIn);
                printf("\tOff:%08x-Size:%08x\n",strtab.sh_offset,strtab.sh_size);
                int x = 0;
                for(int i = 0; i < ehdr.e_shentsize * ehdr.e_shnum; i += ehdr.e_shentsize) {
                    fseek(elfIn,i+ehdr.e_shoff,0);
		            ElfW(Shdr) shdr;
                    fread(&shdr,sizeof(shdr),1,elfIn);
                    printf("\tOff:%08x-Size:%08x\n",shdr.sh_offset,shdr.sh_size);
                    printf("\tNid:%08x-Type:%08x\n",shdr.sh_name,shdr.sh_type);
                    textdata = malloc(shdr.sh_size);
                    int offset = 0;
                    fgetpos(jprOut,&offset);
                    memset(&jph.shdrs[x],x,sizeof(jprog_shdr));
                    strcpy(jph.shdrs[x].name,"UnkwnSgmt");
                    jph.shdrs[x].flags = JPROG_UNKN;
                    jph.shdrs[x].length = shdr.sh_size;
                    jph.shdrs[x].offset = offset;
                    if(x == 0) {
                        strcpy(jph.shdrs[x].name,"CodeSgmt");
                        jph.shdrs[x].type = JPROG_CODE;
                        jph.shdrs[x].flags = JPROG_EXECUTABLE;
                    }
                    printf("Writing sgmt to %s %i\n",jph.shdrs[x].name,x);
                    printf("Setting data\n");
                    fseek(elfIn,shdr.sh_offset,0);
                    fread(textdata,shdr.sh_size,1,elfIn);
                    fwrite(textdata,shdr.sh_size,1,jprOut);
                    free(textdata);
                    current++;
                    x++;
                }
                fseek(jprOut,0,0);
                fwrite(&jph,sizeof(jprog_hdr),1,jprOut);
            } else {
                printf("Invalid elf file\n");
                fclose(jprOut);
                fclose(elfIn);
                return -2;
            }
            fclose(jprOut);
            fclose(elfIn);
        } else {
            printf("Could not open %s for writing\n",arguments.output_file);
            fclose(elfIn);
            return -1;
        }
    } else {
        printf("Could not open %s for reading\n",arguments.input_file);
        return -1;
    }
    return 0;
}