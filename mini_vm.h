#ifndef MINIVM_H
#define MINIVM_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <sys/random.h>

#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic push

#define ErrMem 0x01
#define NoArgs {0x00, 0x00}

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;
/*
16 bit  computer
6register addr : AX BX CX DX SP(stack pointer) IP(Instruction Pointer)
65kb memory
*/
typedef unsigned short int Reg;

typedef struct s_registers
{
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
} Registers;

typedef struct S_CPU
{
    Registers r;
} CPU;

// typedef u8 Memory[((u16)-1)];

typedef enum e_opcode
{
    mov = 0x01,
    nop = 0x02,
} Opcode;

typedef struct s_instrmap
{
    Opcode o;
    u8 size;
} IM;

typedef u8 Args;

typedef struct s_instruction
{
    Opcode o;
    Args a[];
} Instruction;

typedef u8 Memory[((unsigned int)(-1))];
typedef u8 Program;

typedef struct s_virtualmachine
{
    CPU c;
    Memory m;
    Program p;
} VM;

typedef Memory *Stack;

static IM instruction_map[] = {
    {mov, 0x03},
    {nop, 0x01},
};

#define IM_SIZE ((sizeof(instruction_map) / sizeof(IM)))
// #define IM_SIZE ((sizeof(IM) / sizeof(IM)))

void zero_out(u8 *str, u16 size)
{
    u8 *p;
    u16 n;
    for (n = 0, p = str; n < size; p++, n++)
        *p = 0;
}

void copy(u8 *destination, u8 *source, u16 size)
{
    u8 *dst;
    u8 *src;
    u16 n;
    for (n = size, dst = destination, src = source; n > 0; n--, src++, dst++)
        *dst = *src;
    return;
}

void print_hex(u8 *text, u16 size, u8 delimiter)
{
    u8 *p;
    u16 n;
    for (p = text, n = size; n > 0; n--, p++)
    {
        printf("%.02x", *p);
        if (delimiter)
            printf("%c", delimiter);
        fflush(stdout);
    }
    printf("\n");
    return;
}

VM *virtualmachine();
Program *example_program(VM *); // TEST FUNCTION
u16 map(Opcode o);
// int main(int, char **);
int main();
#endif