#include "mini_vm.h"

// int main(int argc, char *argv[])
int main()
{

    VM *vm = virtualmachine();
    printf("vm address = %p\n", vm);
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
    Instruction *instr1 = (Instruction *)malloc((int)instr_size1);
    Instruction *instr2 = (Instruction *)malloc((int)instr_size2);

    assert(instr1 && instr2);
    zero_out((u8 *)instr1, instr_size1);
    zero_out((u8 *)instr2, instr_size2);

    instr1->o = mov;
    u16 arg_size = (instr_size1 - 1); // instructions is 1 byte
    u16 hex_size = (map(mov) + map(nop));
    printf("mov size: %d, nop size:%d\n", map(mov), map(nop));
    if (arg_size)
    {
        printf("WE HAVE ARG SIZE:%d\n", arg_size);
        arg1 = (Args *)malloc((int)arg_size);
        assert(arg1);
        zero_out((u8 *)arg1, arg_size);

        // arguments
        // 0000 0001 mov eax
        // 0000 0000
        // 0000 0005 mov eax, 0x05
        *arg1 = 0x00;       // first byte representation of 'mov eax'
        *(arg1 + 1) = 0x05; // next byte, location
        print_hex((u8 *)arg1, hex_size, ' ');
    }

    printf("memory size = %d\n", (u16)sizeof(*vm->m));
    Program *prog = vm->m; // set the program pointer to the beginning of the stack memory
    copy((u8 *)prog, (u8 *)instr1, 1);
    prog++; // move pointer forward by 1

    if (arg1)
    {
        copy((u8 *)prog, (u8 *)arg1, arg_size);
        prog += arg_size; // move pointer to the nex point in the stack
    }

    instr2->o = nop;
    copy((u8 *)prog, (u8 *)instr2, 1);
    free(instr1);
    free(instr2);
    printf("move + nop size, %d\n", hex_size);
    printf("returning memory:");
    print_hex((u8 *)prog - 10, hex_size, ' ');

    return vm->m; // pointer to the beginning of memory where we started writing te program
}

// generate a vm shell
VM *virtualmachine()
{
    VM *new_vm;
    u16 size;

    // size = (u16)sizeof(struct s_virtualmachine);
    size = (u16)sizeof(VM);
    new_vm = (VM *)malloc((int)size);

    if (!new_vm)
    {
        errno = ErrMem;
    }
    zero_out((u8 *)new_vm, size);

    // prog_space = (Program *)malloc((int)prog_size);
    // if (!prog_space)
    // {
    //     free(new_vm);
    //     errno = ErrMem;
    //     return (VM *)0;
    // }

    // copy(prog_space, prog, prog_size);
    return new_vm;
}