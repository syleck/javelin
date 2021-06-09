#include "serial.h"
#include "../x86/asm.h"
#include "../module.h"
#include "../drv/device.h"
#include "tty.h"
#include "stdio.h"
#include <stdbool.h>

MODULE("SERIAL");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

#define COM1 0x3F8
bool com1_enable = false;
#define COM2 0x2F8
bool com2_enable = false;

int is_transmit_empty() {
   return inb(COM1 + 5) & 0x20;
}
 
void write_serial(char a) {
    if(!com1_enable)
        return;
    while (is_transmit_empty() == 0);
    
    outb(COM1,a);
}

void serial_writestream(char* a, int l) {
    int j = 0;
    while(l != 0) {
        write_serial(*(a+j));
        j++; l--;
    }
}

int serial_received() {
    return inb(COM1 + 5) & 1;   
}

char read_serial() {
    if(!com1_enable)
        return '\0';
    while (serial_received() == 0);

    return inb(COM1);
}

void serial_readstream(char* a, int l) {
    int j = 0;
    while(l != 0) {
        *(a+j) = read_serial();
        j++; l--;
    }
}

int is_transmit_empty2() {
   return inb(COM2 + 5) & 0x20;
}
 
void write_serial2(char a) {
    if(!com2_enable)
        return;
    while (is_transmit_empty2() == 0);
    
    outb(COM2,a);
}

void serial2_writestream(char* a, int l) {
    int j = 0;
    while(l != 0) {
        write_serial2(*(a+j));
        j++; l--;
    }
}

int serial2_received() {
    return inb(COM2 + 5) & 1;   
}

char read_serial2() {
    if(!com2_enable)
        return '\0';
    while (serial2_received() == 0);

    return inb(COM2);
}

void serial2_readstream(char* a, int l) {
    int j = 0;
    while(l != 0) {
        *(a+j) = read_serial2();
        j++; l--;
    }
}

io_struct serial0 = {
    .write_byte = write_serial,
    .write_stream = serial_writestream,
    .read_byte = read_serial,
    .read_stream = serial_readstream,
    .get_available = serial_received,
    .cmode = IO_INPUT_OUTPUT
};

io_struct serial1 = {
    .write_byte = write_serial2,
    .write_stream = serial2_writestream,
    .read_byte = read_serial2,
    .read_stream = serial2_readstream,
    .get_available = serial2_received,
    .cmode = IO_INPUT_OUTPUT
};

void init_serial() {
    // Serial port 1
    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    //                  (hi byte)
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
    
    if(inb(COM1 + 0) != 0xAE) {
        OOPS("com1 faulty");
    } else {
        outb(COM1 + 4, 0x0F);
        com1_enable = true;
        int d = add_simple_text("ttyS0",serial0);
        add_alias(get_device(d),"com1");
        add_echo_tty(&serial0);
    }

    // Serial port 2
    outb(COM2 + 1, 0x00);    // Disable all interrupts
    outb(COM2 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM2 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM2 + 1, 0x00);    //                  (hi byte)
    outb(COM2 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM2 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM2 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(COM2 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COM2 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if(inb(COM2 + 0) != 0xAE) {
        
    } else {
        outb(COM2 + 4, 0x0F);
        com2_enable = true;
        int d = add_simple_text("ttyS1",serial1);
        add_alias(get_device(d),"com2");
    }

    ser_handle = &serial0;
}