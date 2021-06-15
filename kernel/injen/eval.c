#include "injen.h"
#include "../string.h"
#include "../module.h"
#include "../stdlib.h"
#include <stdint.h>
#include <stdbool.h>

MODULE("INJEN_VM");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

#define FLAGS_NOTCOMPILED 1<<1
#define FLAGS_EXECUTABLE  1<<2

struct instance {
    struct instance_member {
        void* data;
        uint8_t flags;
        char name[16];
    } instance_members[1024];
    int member_count;
    int* stack;
    int* text;
    int* data;

    // GENERAL PURPOSE REGISTSERS

    //        A   A   M   E   E
    uint32_t r0, r1, r2, r3, r4;

    // SYSTEM REGISTERS
    uint32_t  bp;
    uint32_t* ip, sp;
    char name[16];
} instances[512];
int instance_count;
int current_instance;

#define cinstance instances[current_instance] 
#define cinstancem instances[current_instance].instance_members

int create_instance() {
    memset(&instances[instance_count],0,sizeof(instances[instance_count]));
    current_instance = instance_count;
    return instance_count++;
}

int create_member(char* name, void* data, uint8_t flags) {
    memcpy(cinstancem[cinstance.member_count].name,name,strlen(name));
    cinstancem[cinstance.member_count].data = data;
    cinstancem[cinstance.member_count].flags = flags;
    return cinstance.member_count++;
}

void switch_instance(int instance) {
    current_instance = instance;
}

void compile_instance() {
    mprintf("Compiling instance with %i members\n",cinstance.member_count);
    for(int i = 0; i < cinstance.member_count; i++) {
        if(cinstancem[i].flags & FLAGS_NOTCOMPILED) {

        }
    }
}

void load_instance_data(char* data, char* name) {
    create_member(name,data,FLAGS_NOTCOMPILED | FLAGS_EXECUTABLE);
}

void eval_instance() {
    int op, *tmp;
    bool halted = false;
    while(!halted) {
        op = *cinstance.ip++;
        switch(op) {

            case Halt:
                halted = true;
                break;

            case Noop:
                break;

            case MovRegImm:
                op = *cinstance.ip++;
                // register
                switch(op) {
                    case Register_0:
                        cinstance.r0 = *cinstance.ip++;
                        break;
                    case Register_1:
                        cinstance.r1 = *cinstance.ip++;
                        break;
                    case Register_2:
                        cinstance.r2 = *cinstance.ip++;
                        break;
                    case Register_3:
                        cinstance.r3 = *cinstance.ip++;
                        break;
                    case Register_4:
                        cinstance.r4 = *cinstance.ip++;
                        break;
                    case Register_Bp:
                        cinstance.bp = *cinstance.ip++;
                        break;
                }
                break;


        }
    }
}