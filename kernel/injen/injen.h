#ifndef INJEN_H
#define INJEN_H

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
 * @brief Tokens for Injen.
 * Im not working on this right now, more of working on the VM. Someone else help with the lexer
 * Format for lexer should be
 * [TOKEN_INSTR] [TOKEN_DST],[TOKEN_SRC]\n
 */
enum injen_token {
    // Move, Dst <- Src
    Mov,
    // Alloc, Member Dst <- Src
    Alc,
    // Add, Dst += Src
    Add,
    // Sub, Dst -= Src
    Sub,
    // Mul, Dst *= Src
    Mul,
    // Div, Dst /= Src
    Div,
    // Mod, Dst %= Src
    Mod,
    // Inc, Dst++
    Inc,
    // Dec, Dst--
    Dec,
    // Call, Dst + Src
    Call,
    // Jmp, Dst + Src
    Jmp,
    // Cmp, Dst (compare) Src
    Cmp,
    // Jump if Equal
    Je,
    // Jump if Not Equal
    Jne,
    // Jump if Status Bit (Dst)
    Jsb,
    // Reference to Variable ($<variable name>)
    RefVariable,
    // Reference to Register (%<register name>)
    RefRegister,
    // Number (0, 1, 2, etc)
    Number,
};

/**
 * @brief Register names
 * 
 */
enum injen_register {
    Register_0 = 45, Register_1, Register_2, Register_3, Register_4,
    Register_Bp = 99, Register_Sp, Register_Ip,
};

/**
 * @brief Bytecode representations of all the registers
 * 
 */
enum injen_instruction {
    Halt, Noop,
    // mov instructions
    MovRegImm, MovRegReg,
    MovMRegImm, MovMImm, MovMReg,
    MovMRegReg,
    // math instructions
    IncReg, IncRegStep, DecReg, DecRegStep,
    AddRegImm, SubRegImm, MulRegImm, DivRegImm,
    AddRegReg, SubRegReg, MulRegReg, DivRegReg,
    CmpRegReg, CmpRegImm,
    // flow control
    CallImm, CallReg,
    JmpImm, JmpReg,
    // prefixes
    MovReverse, // only really works on the MRegImm instructions, which tells it to instead move Reg to ImmXX
    OffsetText, OffsetData, OffsetStack // this probably should only be done by this vm, but keep them in anyways for support. they tell the vm that this is offsetted by text, data, or stack (by default its offsetted by data)
};

#endif