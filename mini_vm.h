// 16DEC min 2:05

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
#define ErrInstr 0x08
// #define NoArgs {0x00, 0x00}

// flag setting macros
#define equal(x) (!!(((x->c.r.flags) & 0x08) >> 3))
#define gt(x) (!!(((x->c.r.flags) & 0x04) >> 2))
#define higher(x) (!!(((x->c.r.flags) & 0x02) >> 1))
#define lower(x) (!!(((x->c.r.flags) & 0x01)))

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;

/*
    16 bit  computer
    6 register addr : AX BX CX DX SP(stack pointer) IP(Instruction Pointer)
    extra register FLAGS: each bit representing a bool
    $bool repd: eq, gt, Lower Half, Higher Half
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
        Reg flags;
} Registers;

typedef struct s_cpu
{
        Registers r;
} CPU;

typedef enum e_opcode
{
    nop = 0x01,
    hlt = 0x02,
    mov = 0x08, /*enum val 8-15(0x08-0x0f) all represent move instructions to different registers*/
    ste = 0x10,
    cle = 0x11,
    stg = 0x12,
    clg = 0x13,
    sth = 0x14,
    clh = 0x15,
    stl = 0x16,
    cll = 0x17,
    /*
    reserved = 0x18,
    reserved = 0x19
    */

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
    {nop, 0x01},
    {hlt, 0x01},
    {mov, 0x03},
    {0x09, 0x03},
    {0x0a, 0x03},
    {0x0b, 0x03},
    {0x0c, 0x03},
    {0x0d, 0x03},
    {0x0e, 0x03},
    {0x0f, 0x03},
    {ste, 0x01},
    {cle, 0x01},
    {stg, 0x01},
    {clg, 0x01},
    {stl, 0x01},
    {cll, 0x01},
    {sth, 0x01},
    {clh, 0x01},
};

#define IM_SIZE ((sizeof(instruction_map) / sizeof(IM)))

void exec_mov(VM *, Opcode, Args, Args);
void vm_error(VM *, ErrorCode);
#define segfault(x) vm_error((x), ErrSegv)
void exec_instr(VM *vm, Instruction *instr);
void execute_instr_jb(VM *, Program *);
void execute(VM *);
VM *virtualmachine();
Program *example_program(VM *); // TEST FUNCTION
u16 map(Opcode o);
// int main(int, char **);
int main();
#endif
