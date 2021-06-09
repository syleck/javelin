#include "stdio.h"
#include <stdbool.h>
#include "../string.h"
#include "../module.h"
#include "tty.h"

MODULE("STDIO");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

io_struct* tty_handle = 0;
io_struct* ser_handle = 0;
io_struct* lpt_handle = 0;
io_struct* bochs_handle = 0;

void vfprintf(io_struct* handle, const char* fmt, va_list varg) {
    if(handle == 0)
        return;
    bool control_mode = false;
    char str[32] = {0};
    while(*fmt!='\x0') {
        if(control_mode) {
            char* string;
            int c = 0;
            switch(*fmt) {
                case 's':
                    string = va_arg(varg,char*);
                    handle->write_stream(string,strlen(string));
                    control_mode = false;
                    break;
                case 'i':
                    c = va_arg(varg, int);
					itoa(c, 10, str);
					handle->write_stream(str,strlen(str));
                    control_mode = false;
                    break;      
                case 'x':
                	c = va_arg(varg, int);
					itoa(c, 16, str);
					handle->write_stream(str,strlen(str));
                    control_mode = false;
                    break;
                case 'b':
                	c = va_arg(varg, int);
					itoa(c, 2, str);
					handle->write_stream(str,strlen(str));
                    control_mode = false;
                    break;
                case 'c':
                    handle->write_byte(va_arg(varg, char));
                    control_mode = false;
                    break;
                case '%':
                    handle->write_byte('%');
                    control_mode = false;
                    break;
                default:
	                OOPS("Printf is printing unknown character");
                    handle->write_byte('%');
                    handle->write_byte(*fmt);
                    control_mode = false;
                    break;
            }
        } else {
            if(*fmt=='%') {
                control_mode = true;
            } else {
                handle->write_byte(*fmt);
            }
        }
        fmt++;
    }
    update_cursor(terminal_col,terminal_row);
}

void vsprintf(int output, const char* fmt, va_list varg) {
    io_struct* handle = tty_handle;
    switch(output) {
        case stdio_tty_out:
            handle = tty_handle;
            break;
        case stdio_ser_out:
            handle = ser_handle;
            break;
        case stdio_lpt_out:
            handle = lpt_handle;
            break;
        case stdio_dbg_out:
            handle = bochs_handle;
            break;
    }
    vfprintf(handle, fmt, varg);
}

void printf(const char* fmt,...) {
    va_list va;
    va_start(va,fmt);
    vprintf(fmt,va);
    va_end(va);
}

void fprintf(io_struct* handle, const char* fmt,...) {
    va_list va;
    va_start(va,fmt);
    vfprintf(handle,fmt,va);
    va_end(va);
}