#include "injen.h"
#include "../string.h"
#include "../module.h"
#include "../stdlib.h"
#include "../mem.h"
#include "../x86/int/isr.h"

MODULE("INJEN_VM");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

#define FLAGS_NOTCOMPILED 1<<1
#define FLAGS_EXECUTABLE  1<<2
#define FLAGS_TEXT        1<<3
#define FLAGS_DATA        1<<4
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

//                          format

//                         VERSrevs
#define IVM_SPEC_VERSION 0x00010001
//                         VERSrevs
#define EVL_EMUL_VERSION 0x00010002
//                         VENDprod
#define EVL_IDENTIFIER   0x4E150000

struct instance {
    struct instance_member {
        void* data;
        uint8_t flags;
        uint32_t size;
        uint32_t did;
        char name[16];
    } instance_members[1024];
    int member_count;
    int* stack;
    int* text;
    int* data;
    int stacksize;
    int textsize;
    int datasize;
    int permissions;
    bool state;
    bool faulted;
    bool fhdt_init;
    uint32_t fault;

    // GENERAL PURPOSE REGISTSERS
    uint32_t r0, r1, r2, r3, r4;
    // ADDRESSING REGISTERS (basically general purpose)
    uint32_t a0, a1, a2, a3, a4;
    // CPU2PROGRAM REGISTERS (use only for ivm/program communication)
    uint32_t lf0, lf1;

    // SYSTEM REGISTERS
    uint32_t  bp;
    uint32_t* ip, *sp;
    ivm_fhdt* fhdt;
    uint8_t sr;
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

int create_member(char* name, void* data, uint32_t size, uint8_t flags) {
    memcpy(cinstancem[cinstance.member_count].name,name,strlen(name));
    cinstancem[cinstance.member_count].data = data;
    cinstancem[cinstance.member_count].size = size;
    cinstancem[cinstance.member_count].flags = flags;
    return cinstance.member_count++;
}

void switch_instance(int instance) {
    current_instance = instance;
}

void compile_instance() {
    mprintf("Compiling instance with %i members\n",cinstance.member_count);
    mputs("Compilation pass\n");
    int compiled = 0;
    for(int i = 0; i < cinstance.member_count; i++) {
        if(cinstancem[i].flags == FLAGS_NOTCOMPILED | FLAGS_EXECUTABLE) {
            // compile it
            cinstancem[i].flags = FLAGS_TEXT;
            cinstancem[i].data = asmcompile(&cinstancem[i].size,cinstancem[i].data);
            printf(".");
            compiled++;
        }
    }
    printf(" %i members compiled.\n",compiled);
    create_member("evlex_state",0,0,FLAGS_DATA);
    mputs("Sizing pass\n");
    int text_poolsize = 0;
    int data_poolsize = 0;
    for(int i = 0; i < cinstance.member_count; i++) {
        if(CHECK_BIT(cinstancem[i].flags,3)) {
            cinstancem[i].did = text_poolsize;
            text_poolsize += cinstancem[i].size;
            printf("t");
        } else if(CHECK_BIT(cinstancem[i].flags,4)) {
            cinstancem[i].did = data_poolsize;
            data_poolsize += cinstancem[i].size;
            printf("d");
        }
    }
    printf("\n");
    DVERBOSE(mprintf("Text poolsize: %iB, data poolsize: %iB\n",text_poolsize,data_poolsize));
    mputs("Translation pass\n");
    for(int i = 0; i < cinstance.member_count; i++) {
        if(cinstancem[i].flags == FLAGS_TEXT) {
            for(int i = 0; i < cinstancem[i].size; i++) {
                int* ptr = i + cinstancem[i].data;
                if(*ptr & 0xFF000000) {
                    int dsel = *ptr & 0x00FFFFFF;
                    *ptr = cinstancem[dsel].did;
                }
            }
        }
    }
    mputs("Packing pass\n");
    cinstance.text = malloc(text_poolsize);
    cinstance.data = malloc(data_poolsize);
    cinstance.stack = malloc(4*1024);
    cinstance.textsize = text_poolsize;
    cinstance.datasize = data_poolsize;
    cinstance.stacksize = 4*1024;
    int x = 0;
    for(int i = 0; i < cinstance.member_count; i++) {
        if(cinstancem[i].flags == FLAGS_TEXT) {
            memcpy(cinstance.text+x,cinstancem[i].data,cinstancem[i].size);
            free(cinstancem[i].data);
            x += cinstancem[i].size;
            printf("T");
        }
    }
    x = 0;
    for(int i = 0; i < cinstance.member_count; i++) {
        if(cinstancem[i].flags == FLAGS_DATA) {
            memcpy(cinstance.data+x,cinstancem[i].data,cinstancem[i].size);
            free(cinstancem[i].data);
            x += cinstancem[i].size;
            printf("D");
        }
    }
    printf("\n");
    mputs("Compilation finished\n");
    cinstance.ip = (uint32_t)cinstance.text;
    cinstance.sp = (uint32_t)cinstance.stack;
    cinstance.state = true;
    /*for(int i = cinstance.text; i < cinstance.textsize + cinstance.text; i++) {
        printf("%x %s ",*(uint32_t*)i,instoname(*(uint32_t*)i));
    }*/
}

void load_instance_data(char* data, char* name) {
    create_member(name,data,strlen(data),FLAGS_NOTCOMPILED | FLAGS_EXECUTABLE);
}

void ivm_dumpregs() {
    mprintf("instance dump\nr0 r1 r2 r3 r4\n%x %x %x %x %x\nbp sp ip\n%x %x %x\nstack %x data %x text %x\nlast fault: %i\n",cinstance.r0,cinstance.r1,cinstance.r2,cinstance.r3,cinstance.r4,cinstance.bp,cinstance.sp,cinstance.ip,cinstance.stack,cinstance.data,cinstance.text,cinstance.fault);
}

#define INC_BYTE() *cinstance.ip++
#define VALID(x) if(!x) { cinstance.state = false; break; }
#define REGAB() { rega = regfromop(INC_BYTE()); regb = regfromop(INC_BYTE()); VALID(rega); VALID(regb); }
#define REGA() { rega = regfromop(INC_BYTE()); VALID(rega); }

int* regfromop(int i) {
    switch(i) {
        case Register_0:
            return &cinstance.r0;
            break;
        case Register_1:
            return &cinstance.r1;
            break;
        case Register_2:
            return &cinstance.r2;
            break;
        case Register_3:
            return &cinstance.r3;
            break;
        case Register_4:
            return &cinstance.r4;
            break;
        case Register_Bp:
            return &cinstance.bp;
            break;
        case Register_Sp:
            return &cinstance.sp;
            break;
        case Register_Ip:
            return &cinstance.ip;
            break;
        case Cpu2prog_Lf0:
            return &cinstance.lf0;
            break;
        case Cpu2prog_Lf1:
            return &cinstance.lf1;
            break;
        case Address_0:
            return &cinstance.a0;
            break;
        case Address_1:
            return &cinstance.a1;
            break;
        case Address_2:
            return &cinstance.a2;
            break;
        case Address_3:
            return &cinstance.a3;
            break;
        case Address_4:
            return &cinstance.a4;
            break;
    }
}

int pop() {
    return *cinstance.sp--;
}

void push(int x) {
    *cinstance.sp++ = x;
}

int eval_instance() {
    int op, *tmp;
    void* offset = cinstance.data;
    bool reverse = false;
    int* rega, *regb;
    while(cinstance.state) {
        op = INC_BYTE();
        regb = rega = 0;
        switch(op) {

            // INJEN

            case InjenState:
                switch(cinstance.r0) {
                    case 0:
                        cinstance.r1 = EVL_EMUL_VERSION;
                        cinstance.r2 = IVM_SPEC_VERSION;
                        cinstance.r3 = EVL_IDENTIFIER;
                        cinstance.r4 = 0x51CCE5;
                        break;
                    default:
                        cinstance.r4 = 0xFA15;
                        break;
                }
                break;

            case Halt:
                cinstance.state = false;
                break;

            case Noop:
                break;

            case LoadFh:
                cinstance.fhdt = (ivm_fhdt*)offset+(uint32_t)INC_BYTE();
                cinstance.fhdt->true_address = (ivm_fhentry*)offset + cinstance.fhdt->address;
                break;

            // INC

            case IncReg:
                REGA();
                *rega = (*rega) + 1;
                break;

            case IncRegStep:
                REGAB();
                *rega = (*rega) + (*regb);
                break;

            case DecReg:
                REGA();
                *rega = (*rega) - 1;
                break;

            case DecRegStep:
                REGAB();
                *rega = (*rega) - (*regb);
                break;

            // MOV

            case MovRegImm:
                rega = regfromop(INC_BYTE());
                if(rega)
                    *rega = INC_BYTE();
                else
                    // die
                    ivm_fault(1,cinstance.ip);
                break;

            case MovReverse:
                reverse = true;
                break;
            
            // STACK

            case PushImm:
                push(INC_BYTE());
                break;

            case PushReg:
                REGA();
                push(*rega);
                break;

            case PopReg:
                REGA();
                *rega = pop();
                break;

            // CMP

            case CmpRegReg:
                rega = regfromop(INC_BYTE());
                regb = regfromop(INC_BYTE());
                if(rega)
                    if(regb) {
                        int a = *rega;
                        int b = *regb; // for the sake of quickness
                        cinstance.sr = 0;
                        if(a == b) {
                            cinstance.sr = 0b00000010;
                        } else {
                            cinstance.sr = 0b00000001;
                        }
                        if(a < b) {
                            cinstance.sr = 0b00000100;
                        } else if(a > b) {
                            cinstance.sr = 0b00001000;
                        }
                    }
                else
                    ivm_fault(1,cinstance.ip);
                break;

            case CmpRegImm:
                rega = regfromop(INC_BYTE());
                if(rega) {
                    int a = *rega;
                    int b = INC_BYTE();
                    cinstance.sr = 0;
                    if(a == b) {
                        cinstance.sr = 0b00000010;
                    } else {
                        cinstance.sr = 0b00000001;
                    }
                    if(a < b) {
                        cinstance.sr = 0b00001000;
                    } else if(a > b) {
                        cinstance.sr = 0b00000100;
                    }
                } else
                    ivm_fault(1,cinstance.ip);
                break;

            // IN/OUT

            case InRegImm:
                REGA();
                *rega = injen_in(&cinstance.permissions,INC_BYTE());
                break;

            case OutImmImm:
                injen_out(&cinstance.permissions,INC_BYTE(),INC_BYTE());
                break;

            case InRegReg:
                REGAB(); 
                *regb = injen_in(&cinstance.permissions,*rega);
                break;

            case OutRegImm:
                REGA();
                if(!reverse)
                    injen_out(&cinstance.permissions,*rega,INC_BYTE());
                else
                    injen_out(&cinstance.permissions,INC_BYTE(),*rega);
                reverse = false;
                break;

            case OutRegReg:
                REGAB(); 
                injen_out(&cinstance.permissions,*rega,*regb);
                break;

            case JmpImm:
                cinstance.ip = ((int)cinstance.text + INC_BYTE());
                break;

            case IRet:
                cinstance.faulted = false;
                cinstance.ip = pop();
                break;

            case Ret:
                cinstance.ip = pop();
                break;

            default:   
                ivm_fault(1,cinstance.ip);
                break;

        }
    }
    DVERBOSE(mprintf("Returned %x",cinstance.r0));
    return cinstance.r0;
}

void ivm_fault(int faultid, int code) {
    if(cinstance.faulted) {
        mputs("Double fault\n");
        ivm_dumpregs();
        cinstance.state == false;
        return;
    }
    cinstance.faulted = true;
    cinstance.lf0 = code;
    cinstance.fault = faultid;
    cinstance.lf1 = faultid;
    if(!cinstance.fhdt_init) {
        mputs("No fhdt, killing\n");
        ivm_dumpregs();
        cinstance.state = false;
        return;
    }
    for(int i = 0; cinstance.fhdt->true_address[i].ip != 0; i++) {
        if(i == faultid) {
            push(cinstance.ip);
            cinstance.ip = cinstance.fhdt->true_address[i].ip;
            return;
        } 
    }
    mputs("Fault not handled\n");
    ivm_dumpregs();
    cinstance.state == false;
}