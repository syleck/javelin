# INJEN IVM Specification

Javelin Compiled Language, licensed under the AGPL

Current spec version: 0x00010001 (Version 1, Revision 1)

The spec is technically two uint16_t's, the first 4 numbers (0001) are the specification version, and the last 4 (0001) are the specification revision.

I got inspired by HolyC.

Syntax:

    [instruction] [destination] [source]

For example:

    ;    whitespaces & tabs are ignored
    alc     $variable,  0              ; COMMENT, compiler automatically allocates $variable
    mov     %r0,        $variable
    add     %r0,        10
    mov     $variable,  %r0

end result:

    $variable (at <some_addr>) = 10
    %r0 = 10

This compiles to (instructions/registers defined in injen.h)

    ; alc $variable, 0
    OffsetData
    MovMImm32 <some_addr>,0
    ; mov %r0, $variable
    MovReverse 
    OffsetData
    MovMRegImm32 Register_0 <some_addr>
    ; add %r0, 10
    AddRegImm Register_0 10
    ; mov $variable, %r0
    OffsetData
    MovMReg <some_addr> Register_0

## Injen Spec

There are 12 or more general purpose registers. BP is kinda for calling convention stuff, but if youre using pure IVM its general purpose.

    r0, r1, r2, r3, r4, bp

### SORTA GENERAL PURPOSE

Do not rely on these registers existing. I mean you kinda can, but if they get changed/removed support/warranty is not given.

    (added in spec 0x00010001) lf0, lf1, a0, a1, a2, a3, a4

There is a stack pointer, and an instruction pointer, and a status register (CMP).

Prefixes may be in any order, so

    OffsetData
    MovReverse

is fine, and

    MovReverse
    OffsetData

is fine aswell.

## IO Ports

Rewrite this im lazy

    0x00000000-0x0000FFFF
        Should be sent to the respective IO ports with a inl or outl on the hypervisors end, when IVM permits it. (permission level 2 or above)
    0xFFFF0000-0xFFFFFFFF
        Should be sent to IVM IO ports.
            Port Offset 0: Terminal Data
                All data sent to this port is displayed on the terminal.
            Port Offset 1: Terminal Control
                All data sent to this port controls the terminal.

## Permissions

There are 3 permissions levels,
    - 0, called the lowerproc level
    - 1, called the proc level
    - 2, called the superproc level
Procs at level 2 are akin to adminisrator level processes, and procs at level 0 are akin to threads with limited access to the system.

Port 2 IO allows procs initially at level 2 to change the io_control level, which allows lower procs to write out to IO ports. For example, terminal access is restricted until a superproc allows a proc or lowerproc to use it.
    
## Licensing

The Injen/IVM spec itself is licensed under the MIT, but this implementation of it is AGPL.

    Copyright 2021-2022 The Javelin Team & Kernelvega

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.