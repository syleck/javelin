# INJEN IVM

Javelin Compiled Language, licensed under the AGPL

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

There are 6 general purpose registers. BP is kinda for calling convention stuff, but if youre using pure IVM its general purpose.

    r0, r1, r2, r3, r4, bp

There is a stack pointer, and an instruction pointer.

## Licensing

The IVM spec itself is licensed under the MIT, but this implementation of it is AGPL.

    Copyright 2021-2022 The Javelin Team & Kernelvega

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.