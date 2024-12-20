#ifndef EXEC_MOV_H
#define EXEC_MOV_H

#include "mini_vm.h"
// set register using pointer arithmetic
void set_reg(VM *vm, Reg reg_n, u16 dst)
{
    printf("IN SETREG current flags =%d\n", vm->c.r.flags);
    // convert the register to a pointer, cast to Reg pointer then access fields using pointer arithmetic
    // each Reg is 'n' offset from the starting point of the pointer, i.e ax = 0, bx is '1 * size of Reg*' offset   from the beginning, etc.
    Reg *curr_reg = ((Reg *)&vm->c.r) + reg_n;
    if (higher(vm))
    { // use bitwise AND to get the first 2 bits and store value of higher  half
        u16 hi = ((dst << 2)) & 0x0c;
        *curr_reg = (Reg)hi;
        printf("setting higher half\n");
    }
    else if (lower(vm))
    {
        // use bitwise AND to get the last 2 bits and store value of lower half
        Reg lo = (Reg)(dst & 0x03);
        *curr_reg = (Reg)lo;
        printf("setting lower half\n");
    }
    else
    {
        *curr_reg = (Reg)dst;
        printf("setting whole register\n");
    }
}

void exec_mov(VM *vm, Opcode opcode, Args arg1, Args arg2)
{
    // value to be placed in register
    u16 dst = (u16)arg1;
    switch (opcode)
    {
    // each mov to register represented by opcode, has a register with offset (opcode-0x08) times the of Reg*
    // pass the offset an duse pointer arithmetic to assigne values to the registers
    case 0x08 ... 0x0b:
        set_reg(vm, opcode - 0x08, dst);
        break;

        /*mov ax; bin:1000*/
        // case 0x08:
        //     set_reg(vm, 0, dst, dst_higher, dst_lower);
        //     break;

        //    /*mov bx; bin:1001*/
        // case 0x09:
        //    set_reg(vm, 1, dst, dst_higher, dst_lower);
        //    break;

        //    /*mov cx; bin:1010*/
        // case 0x0a:
        //    set_reg(vm, 2, dst, dst_higher, dst_lower);
        //    break;

        //    /*mov dx; bin:1011*/
        // case 0x0b:
        //    set_reg(vm, 3, dst, dst_higher, dst_lower);
        //    break;

        //    /*mov sp; bin:1100*/
    case 0x0c:
        // we dont want higher and lower in stack pointer
        if (higher(vm) || lower(vm))
            vm_error(vm, ErrInstr);
        else
            set_reg(vm, 4, dst);
        break;
    default:
        vm_error(vm, ErrInstr);
        break;
    }

    return;
}
#endif
