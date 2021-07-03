#include "injen.h"
#include "../module.h"
#include "../mem.h"
#include "../string.h"

// ASM version of the Injen Lexer (not written in asm, just compiles asm-like code)

MODULE("INJEN_ASMLEX");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

int offst = 0;
/**
 * @brief Insert instruction/data
 * 
 * @param c data pointer
 * @param p size pointer
 * @param i instruction
 */
void insinstruction(int* c, int i) {
    *(c+offst) = i;
    offst++;
}


void insinstructionxy(int* c, int i, int x, int y) {
    insinstruction(c,i);
    insinstruction(c,x);
    insinstruction(c,y);
}

void insinstructionx(int* c, int i, int x) {
    insinstruction(c,i);
    insinstruction(c,x);
}


#define STRCMP(x,y) (strcmp(token,"")==0)

void* asmcompile(int* size, char* code) {
    int* data = malloc(1024);
    enum tokene {
        Mov,
        In,
        Out,
        Jmp,
        Call,
        
        Misc,
        WriteByte,
        LabelDef,
    };
    struct {
        enum tokene type;
        char arg0[16];
        char arg1[16];
    } tokens[512];

    char* saveptr;

    // remove useless data
    for(int i = 0; i < strlen(size); i++) {
        if(code[i] == ',')
            code[i] = ' ';
    }
    for(int i = 0; i < strlen(size); i++) {
        if(code[i] == '\t')
            code[i] = ' ';
    }

    char* token = 0;
    //__strtok_r(code, " ", &saveptr);
    int tokenc;
    int bytes;
    while(token != NULL) {
        enum tokene tokentype = Misc;

        if(STRCMP(token,"mov")) 
            tokentype = Mov;
        else if(STRCMP(token,"jmp"))
            tokentype = Jmp;
        else if(STRCMP(token,"in"))
            tokentype = In;
        else if(STRCMP(token,"out"))
            tokentype = Out;
        else if(STRCMP(token,"call"))
            tokentype = Call;
        else if(STRCMP(token,".dd"))
            tokentype = WriteByte;
        else if(STRCMP(token,".lb"))
            tokentype = LabelDef;

        if(tokentype != WriteByte) {
            bytes++;
        }

        if(tokentype == Misc) {
            //token = __strtok_r(NULL, " ", &saveptr);
            continue;
        }



        //token = __strtok_r(NULL, " ", &saveptr);
    }
    offst = 0;
    int tmpoffst = 0;
    *size = offst+1;
    offst = 0;
    return data;
}
