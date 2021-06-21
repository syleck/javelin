#include "injen.h"
#include "../x86/asm.h"
#include "../io/tty.h"

struct {
    uint32_t function; // function sel
    uint32_t io_control; // normal procs start in 1, superprocs start in 2, and lower procs start in 1. this allows superprocs to give procs access, and procs to give lower procs access
} port2_ivm_control_state;

struct {
    uint32_t function;
    io_struct* current_tty;
} port1_tty_control_state;

int injen_in(int* permissions, int address) {
    switch(address) {
        case IO_PORT(2): // io control level
            return port2_ivm_control_state.io_control;
        case IO_PORT(3): // read permissions
            return *permissions;
        default:
            if(*permissions > 2)
                return inl(address);
            else
                ivm_fault(2,address);
                return 0;
    }
}

void injen_out(int* permissions, int address, int data) {
    if(permissions > port2_ivm_control_state.io_control) {
        ivm_fault(2,*permissions);
        return;
    }
    switch(address){
        case IO_PORT(0): // Terminal data
            port1_tty_control_state.current_tty->write_byte(data);
            break;
        case IO_PORT(1): // Terminal control
            if(*permissions > 1) {
                ivm_fault(2,*permissions);
                break;
            }
            if(port1_tty_control_state.function == 0) {
                port1_tty_control_state.function = data;
            } else {
                switch(port1_tty_control_state.function) {
                    case 1: // switch tty
                        switch(data) {
                            case 0: // VGA term
                                port1_tty_control_state.current_tty = tty_handle;
                                break;
                            case 1: // Serial
                                port1_tty_control_state.current_tty = ser_handle;
                                break;
                            case 3: // Debug
                                port1_tty_control_state.current_tty = bochs_handle;
                                break;
                            case 4: // Parallel
                                port1_tty_control_state.current_tty = lpt_handle;
                                break;
                        }
                        break;
                }
            }
            break;
        case IO_PORT(2): // Static IVM control (spec)
            if(port2_ivm_control_state.function == 0) {
                port2_ivm_control_state.function = data;
            } else {
                switch(port2_ivm_control_state.function) {
                    case 1: // Force Fault
                        ivm_fault(data,0);
                        break;
                    case 2: // Dump Regs
                        ivm_dumpregs();
                        break; 
                    case 3: // Set Io Control Level
                        if(port2_ivm_control_state.io_control <= *permissions)
                            port2_ivm_control_state.io_control = data;
                        break;
                }
                port2_ivm_control_state.function = 0;
            }
            break;
        case IO_PORT(3): // permissions management (write to request that permission, faults 3 if failed)
            ivm_fault(3,*permissions); // not implemented yet
            break;
        default:
            if(*permissions > 2)
                outl(address, data);
            else
                ivm_fault(2,*permissions);
            break;
    }
}