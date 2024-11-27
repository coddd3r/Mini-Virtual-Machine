#include "mini_vm.h"

// int main(int argc, char *argv[])
int main()
{
    VM *vm = virtualmachine();
    printf("vm address = %p, size:%ld\n", vm, (size_t)sizeof(VM));
    Program *new_program = example_program(vm);
    printf("program address = %p\n", new_program);

    print_hex((u8 *)new_program, (map(mov) + map(nop)), ' ');
}

// map opcode in the instruction map and return the size of the instruction map
u16 map(Opcode o)
{
    u16 n;
    IM *im;
    u16 ret = 0;
    for (n = IM_SIZE, im = instruction_map; n > 0; n--, im++)
    {
        if (im->o == o)
        {
            ret = im->size;
            break;
        }
    }
    // 0 if it fails
    return ret;
}

Program *example_program(VM *vm)
{
    Args *arg1;
    u16 instr_size1 = map(mov);
    u16 instr_size2 = map(nop);
    Instruction *instr1 = (Instruction *)calloc(1, (int)instr_size1);
    Instruction *instr2 = (Instruction *)calloc(1, (int)instr_size2);
    assert(instr1 && instr2);

    instr1->o = mov;
    u16 arg_size = (instr_size1 - 1); // instructions is 1 byte
    u16 hex_size = (map(mov) + map(nop));
    if (arg_size)
    {
        arg1 = (Args *)calloc(1, (int)arg_size);
        assert(arg1);

        // arguments
        // 0000 0001 mov eax
        // 0000 0000
        // 0000 0005 mov eax, 0x05
        *arg1 = 0x00;       // first byte representation of 'mov eax'
        *(arg1 + 1) = 0x05; // next byte, location
    }

    // set the program pointer to the beginning of the stack memory
    Program *prog = vm->m;
    copy((u8 *)prog, (u8 *)instr1, 1);
    prog++; // move pointer forward by 1

    if (arg1)
    {
        copy((u8 *)prog, (u8 *)arg1, arg_size);
        prog += arg_size; // move pointer to the nex point in the stack
    }

    instr2->o = nop;
    // set break point at the end of instructions to separate instruction mem from stack
    vm->b = (instr_size1 + instr_size2 + arg_size);

    copy((u8 *)prog, (u8 *)instr2, 1);
    free(instr1);
    free(instr2);

    return (Program *)vm->m; // pointer to the beginning of memory where we started writing the program instructions
}

// generate a vm shell
VM *virtualmachine()
{
    VM *new_vm;
    u16 size;

    size = (u16)sizeof(VM);
    new_vm = (VM *)calloc(1, (int)size);

    if (!new_vm)
    {
        errno = ErrMem;
    }
    return new_vm;
}