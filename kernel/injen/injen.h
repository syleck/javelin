#ifndef INJEN_H
#define INJEN_H
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Create a instance. Call switch_instance(instance) to switch to it.
 * 
 * @return int The new instance.
 */
int create_instance();
/**
 * @brief Switches to a new instance.
 * 
 * @param instance The new instance
 */
void switch_instance(int instance);
/**
 * @brief Compiles the entire instance.
 * 
 *
 */
void compile_instance();
/**
 * @brief Adds a string to the instance.
 * 
 * @param data Data to add.
 * @param name Name of data.
 */
void load_instance_data(char* data, char* name);
/**
 * @brief Executes the instance.
 * 
 */
int eval_instance();
/**
 * @brief Tells the lexer to compile a string.
 * 
 * @param size Outputted size of data
 * @param code Code itself
 * @return void* Where the compiled data is
 */
void* asmcompile(int* size, char* code);
/**
 * @brief Converts an instruction to its name.
 * 
 * @param i Instruction.
 * @return const char* Name.
 */
const char* instoname(int i);

/**
 * @brief Register names
 * 
 */
enum injen_register {
    Address_0 = 0, Address_1, Address_2, Address_3, Address_4,
    Register_0 = 45, Register_1, Register_2, Register_3, Register_4,
    Register_Bp = 99, Register_Sp, Register_Ip,
    Cpu2prog_Lf0 = 250, Cpu2prog_Lf1,
};

/**
 * @brief Bytecode representations of all the registers
 * 
 */
enum injen_instruction {
    Halt, Noop,
    // mov instructions
    MovRegImm = 55, MovRegReg,
    MovMRegImm, MovMImm, MovMReg,
    MovMRegReg,
    PushImm,
    PushReg,
    PopReg,
    // math instructions
    IncReg = 128, IncRegStep, DecReg, DecRegStep,
    AddRegImm, SubRegImm, MulRegImm, DivRegImm,
    AddRegReg, SubRegReg, MulRegReg, DivRegReg,
    CmpRegReg, CmpRegImm,
    // flow control
    CallImm = 200, CallReg,
    JmpImm, JmpReg,
    Ret, IRet,
    // io
    InRegImm, 
    InRegReg, 
    OutImmImm,
    OutRegImm,
    OutRegReg,
    // etc 
    InjenState = 250,
    LoadFh,
    // prefixes
    MovReverse = 300, // only really works on the MRegImm instructions and OutRegImm instructions, which tells it to instead move Reg to ImmXX
    OffsetText, OffsetData, OffsetStack // this probably should only be done by this vm, but keep them in anyways for support. they tell the vm that this is offsetted by text, data, or stack (by default its offsetted by data)
};

// additional functions
void ivm_dumpregs();
int injen_in(int* permissions, int address);
void injen_out(int* permissions, int address, int data);
void ivm_fault(int faultid, int code);

#define IO_OFFSET 0xFFFF0000
#define IO_PORT(x) IO_OFFSET+x

typedef struct {
    uint32_t* ip;
} ivm_fhentry;

typedef struct {
    uint32_t address;
    ivm_fhentry* true_address;
} ivm_fhdt;

#endif