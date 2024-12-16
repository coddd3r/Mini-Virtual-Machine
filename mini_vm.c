#include "mini_vm.h"
#include <stdio.h>

#include "error_handling.h"
#include "execute_funcs.h"
#include "execute_mov.h"
#include "helpers.h"

// int main(int argc, char *argv[])
int main()
{
    VM *vm = virtualmachine();
    printf("vm address = %p, size:%ld\n", vm, (size_t)sizeof(VM));
    Program *new_program = example_program(vm);
    printf("program address = %p\n", new_program);

    execute(vm);
    printf("ax = %.04hx\n", vm->c.r.ax);

    print_hex((u8 *)new_program, (map(mov) + map(nop) + map(hlt)), ' ');
}

// generate a vm shell
VM *virtualmachine()
{
    VM *new_vm;
    size_t size;
    size = (size_t)sizeof(VM);
    new_vm = (VM *)calloc(1, size);

    if (!new_vm)
        errno = ErrMem;
    return new_vm;
}

// map opcode in the instruction map and return the size of the instruction map
u16 map(Opcode o)
{
    u16 n;
    IM *im;
    u16 ret = 0;
    for (n = IM_SIZE, im = instruction_map; n > 0; n--, im++)
        if (im->o == o)
        {
            ret = im->size;
            break;
        }
    // 0 if it fails
    return ret;
}

Program *example_program(VM *vm)
{
    printf("in example program size of vm:%d\n", (int)sizeof(*vm));
    Args arg1 = 0;
    u16 instr_size1 = map(mov);
    u16 instr_size2 = map(nop);

    assert(instr_size1 && instr_size2);
    printf("instruction sizes:%d, %d\n", instr_size1, instr_size2);
    // Make corrections, alloc only 1 byte each? all instructions are 1 byte size
    // and some e.g mov are only 3 byte size after args are added
    // Instruction *instr1 = (Instruction *)calloc(1, (int)instr_size1);
    // Instruction *instr2 = (Instruction *)calloc(1, (int)instr_size2);
    // Instruction *instr3 = (Instruction *)calloc(1, (int)instr_size2);
    Instruction *instr1 = (Instruction *)calloc(1, (int)sizeof(Instruction));
    Instruction *instr2 = (Instruction *)calloc(1, (int)sizeof(Instruction));
    Instruction *instr3 = (Instruction *)calloc(1, (int)sizeof(Instruction));
    assert(instr1 && instr2 && instr3);
    printf("instr1 size is:%d\n", (int)sizeof(*instr1));
    instr1->o = 0x0a; // mov to cx
    printf("in example program setting first instr opcode:%d\n", instr1->o);
    u16 arg_size = (instr_size1 - 1); // instructions is 1 byte

    // arguments
    // 0000 1010 mov cx
    // 0000 0000
    // 0000 abcd mov cx, 0xabcd
    arg1 = 0xabcd; // change Arg type size to u16

    // set the program pointer to the beginning of the stack memory
    Program *prog = vm->m;
    copy((u8 *)prog, (u8 *)instr1, 1);
    prog++; // move pointer forward by 1

    // copy((u8 *)prog, (u8 *)&arg1, 1); //causes error since arg is 16bit
    copy((u8 *)prog, (u8 *)&arg1, arg_size);
    prog += arg_size; // move pointer to the nex point in the stack

    instr2->o = nop;
    copy((u8 *)prog, (u8 *)instr2, 1);

    prog++; // move pointer by 1 for nop instr
    instr3->o = hlt;
    copy((u8 *)prog, (u8 *)instr3, 1);

    // assign to the instruction pointer register, the first instruction i.e beginning of the stact mem
    vm->c.r.ip = *(Reg *)(vm->m);
    // point stack pointer to the end of the memory since stack builds upward from end
    vm->c.r.sp = (Reg)-1;

    // set break point at the end of instructions to separate instruction mem from stack
    vm->b = (instr_size1 + instr_size2 * 2 + arg_size);

    free(instr1);
    free(instr2);
    free(instr3);

    // return (Program *)&(vm->m); // pointer to the beginning of memory where we
    return (Program *)(vm->m); // pointer to the beginning of memory where we
}

void exec_instr(VM *vm, Instruction *instr)
{
    Args a1 = 0;
    Args a2 = 0;
    u16 instr_size = map(instr->o);
    u16 arg_size = instr_size - 1;
    // u16 size = map(instr->o) - 1;
    printf("in execinstr, opcode:%d, arguments:%.04hx\n", instr->o, instr->a[0]);
    printf("executing args of size:%d\n", arg_size);
    switch (arg_size)
    {
    case 0:
        break;
    case 1:
        //        a1 = instr->a[0];
        break;
    case 2:
        a1 = instr->a[0];
        printf("in case argsize==2, arg1:%d\n", instr->a[0]);
        break;
    case 4:
        a1 = instr->a[0];
        a2 = instr->a[1];
        printf("in case argsize==5, arg1:%d, arg2:%d,\n", instr->a[0], instr->a[1]);
        break;

    default:
        printf("could not find instruction of size %d, returning segfault\n", arg_size);
        segfault(vm);
        break;
    }

    switch (instr->o)
    {
    case mov:
        exec_mov(vm, instr->o, a1, a2);
        break;
    case 0x09 ... 0x0f:
        exec_mov(vm, instr->o, a1, a2);
        break;
    case nop:
        break;
    case hlt:
        vm_error(vm, SysHlt);
        break;
    default:
        // fprintf(stderr, "%s\n", "VM operation not recognized");
        printf("%s\n", "VM operation not recognized");
    }
}

void execute_instr_jb(VM *vm, Program *prog)
{
    Args arg1 = 0;
    Args arg2 = 0;
    u16 arg_size = map((Opcode)*prog) - 1;

    switch (arg_size)
    {
    case 0:
        //       a1 = *(prog + 1);
        break;

    case 2:
        arg1 = *(u16 *)(prog + 1);
        // a1 = *(prog + 1);
        printf("in case argsize==2, arg1:%.04hx\n", arg1);
        break;

    case 4:
        // a1 = *(prog + 1);
        // a2 = *(prog + 3);
        arg1 = *(u16 *)(prog + 1);
        arg2 = *(u16 *)(prog + 3);
        printf("in case argsize==5, arg1:%d, arg2:%d,\n", arg1, arg2);
        break;
        break;

    default:
        printf("could not find instruction of size %d, returning segfault\n", arg_size);
        segfault(vm);
        break;
    }

    switch (*prog)
    {
    case mov:
        exec_mov(vm, (Opcode)*prog, arg1, arg2);
        break;
    case 0x09 ... 0x0f:
        exec_mov(vm, (Opcode)*prog, arg1, arg2);
        break;
    case nop:
        break;
    case hlt:
        vm_error(vm, SysHlt);
        break;
    default:
        // fprintf(stderr, "%s\n", "VM operation not recognized");
        printf("%s\n", "VM operation not recognized");
    }
}
