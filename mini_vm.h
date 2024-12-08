#ifndef MINIVM_H
#define MINIVM_H
// #define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <unistd.h>
// #pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic push

#define NoErr 0x00
#define SysHlt 0x01
#define ErrMem 0x02
#define ErrSegv 0x04
// #define NoArgs {0x00, 0x00}

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
typedef unsigned char ErrorCode;

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

typedef enum e_opcode
{
    mov = 0x01,
    nop = 0x02,
    hlt = 0x03,
} Opcode;

typedef struct s_instrmap
{
        Opcode o;
        u8 size;
} IM;

typedef u16 Args;

typedef struct s_instruction
{
        Opcode o;
        Args a[];
} Instruction;

typedef u8 Memory[((u16)(-1))];
typedef u8 Program;

typedef struct s_virtualmachine
{
        CPU c;
        Memory m;
        u16 b;
        // Program p;
} VM;

typedef Memory *Stack;

static IM instruction_map[] = {
    {mov, 0x03},
    {nop, 0x01},
    {hlt, 0x01},
};

#define IM_SIZE ((sizeof(instruction_map) / sizeof(IM)))

void exec_mov(VM *, Opcode, Args, Args);
void vm_error(VM *, ErrorCode);
#define segfault(x) vm_error((x), ErrSegv)
void exec_instr(VM *vm, Instruction *instr);
void execute(VM *);
VM *virtualmachine();
Program *example_program(VM *); // TEST FUNCTION
u16 map(Opcode o);
// int main(int, char **);
int main();
#endif
