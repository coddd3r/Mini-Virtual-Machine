#ifndef EXECUTE_FUNCS
#define EXECUTE_FUNCS
#include "helpers.h"
#include "mini_vm.h"
#include <stdio.h>
void execute(VM *vm)
{
    assert(vm && *vm->m);
    Program *prog;
    Instruction instr;
    // u16 instr_size1 = map(mov);
    u16 size = 0;
    // prog = (Program *)&vm->m;
    prog = (Program *)vm->m;
    instr.o = (Opcode)*prog;
    size = map(instr.o);
    printf("after first copy before while instr opcode:%d\n", instr.o);
    printf("before while loop instruction size:%d\n", size);
    // while (instr.o != hlt && prog < &vm->b)
    while (instr.o != hlt && (prog - (Program *)(vm->m)) < vm->b)
    {
        instr.o = (Opcode)*prog;
        printf("in while loop opcode:%d\n", instr.o);
        size = map(instr.o);

        if (instr.o == hlt)
        {
            printf("found HALT\n");
            printf("AFTER executing value of flags:%d\n", vm->c.r.flags);
        }
        printf("in while loop instruction size:%d\n", size);
        printf("in while loop instruction opcode:%d\n", instr.o);

        // copy arguments into the instruction pointer
        printf("arguments in prog before copy: %.04hx\n", *(prog + 1));
        int offset_size = (int)(sizeof(instr.o));
        u8 *arg1_mem = ((u8 *)(&instr)) + offset_size; // position of the arg1 in Instruction memory, after skipping the size used for opcode
        zero_out((u8 *)(arg1_mem), 2);                 // avoid spill over from previous arguments
        copy(arg1_mem, (u8 *)(prog + 2), (size - 1));  // WORKS -SORT OF
        printf("arguments in instr after copy: %.04hx\n", instr.a[0]);

        // move to the next instruction
        // exec_instr(vm, &instr);
        execute_instr_jb(vm, prog);
        vm->c.r.ip += size;
        prog += size;
    }

    return;
}

/*
 * we can avoid the mem allocations in the function below by using setting instruction.o to the beginning of cpu.memory
 * this works because of how structs are laid out in mem with first element coming first
 * */
// void execute2(VM *vm)
//{
//     assert(vm && *vm->m);
//     Program *prog;
//     Instruction *instr;
//     u16 instr_size1 = map(mov);
//     instr = (Instruction *)calloc(1, (int)1);
//     u16 size = 0;
//     // prog = (Program *)&vm->m;
//     prog = vm->m;
//     copy((u8 *)instr, (u8 *)prog, 1);
//     size = map(instr->o);
//     printf("after first copy before while instr opcode:%d\n", instr->o);
//     printf("before while loop instruction size:%d\n", size);
//     while (instr->o != hlt && (prog - (vm->m)) < vm->b)
//     {
//         copy((u8 *)instr, (u8 *)prog, 1);
//         size = map(instr->o);
//         if (instr->o == hlt)
//             printf("found HALT\n");
//         printf("in while loop instruction size:%d\n", size);
//         printf("in while loop instruction opcode:%d\n", instr->o);
//         exec_instr(vm, instr);
//
//         // move to the next instruction
//         vm->c.r.ip += size;
//         prog += size;
//     }
//
//     free(instr);
//     return;
// }

// void execute1(VM *vm)
//{
//     Program *prog;
//     // Instruction *instr;
//     Instruction instr;
//     u16 size = 0;
//     // check if we have a n allocated vm and that the memory section is not 0
//     assert(vm && *vm->m);
//     // prog = vm->m;
//     prog = (Program *)&vm->m;
//     printf("at the beginning of memory size of vm->m %d\n", (int)sizeof(vm->m));
//     printf("at the beginning of memory instruction %d\n", ((Instruction *)vm->m)->o);
//
//     /* instr1 args instr2 instr3 */
//     /* mov ax  0x05; nop;  hlt */
//     int stop = 10;
//     // while ((*prog != (Opcode)hlt) && (prog <= (Program *)&(vm->b)) && stop)
//     // while ((((Instruction *)prog)->o != hlt) && (prog <= (Program *)&(vm->b)))
//     // while ((((Instruction *)prog)->o != hlt))
//     // while ((*prog != (Opcode)hlt) && (prog <= (Program *)(vm->b)))
//     // while ((*prog != (Opcode)hlt)  && ((u16)prog <= (u16)(vm->b)))
//
//     u16 brkaddr = ((u16)vm->m + vm->b);
//     while ((*prog != (Opcode)hlt) && (u16)prog < (u16)brkaddr)
//     {
//         // instr = (Instruction *)prog;
//         copy((u8 *)&instr, (u8 *)prog, 1);
//         size = map(instr.o);
//         printf("in while loop instruction size:%d\n", size);
//         printf("in while loop instruction opcode:%d\n", instr.o);
//         exec_instr(vm, &instr);
//
//         // move to the next instruction
//         vm->c.r.ip += size;
//         prog += size;
//         stop -= 1;
//     }
//
//     // if (prog > (Program *)&(vm->b))
//     // if ((prog - (vm->m)) > (int)(vm->b))
//     // if (prog > (Program *)(vm->b))
//     // if ((u16)prog > (u16)(vm->b))
//     if ((u16)&prog > (u16)brkaddr)
//     {
//         printf("Went beyond break \n");
//         segfault(vm);
//     }
// }

#endif
