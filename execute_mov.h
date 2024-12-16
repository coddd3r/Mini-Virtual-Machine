#ifndef EXEC_MOV_H
#define EXEC_MOV_H

#include "mini_vm.h"

void set_reg(VM *vm, Reg reg_n, u16 dst, u16 hi, u16 lo)
{
    // convert the register to a pointer, cast to Reg pointer then access fields using pointer arithmetic
    if (higher(vm))
        *(((Reg *)&vm->c.r) + reg_n) = (Reg)hi;
    else if (lower(vm))
        *(((Reg *)&vm->c.r) + reg_n) = (Reg)lo;
    else
        *(((Reg *)&vm->c.r) + reg_n) = (Reg)dst;
}

void exec_mov(VM *vm, Opcode opcode, Args arg1, Args arg2)
{
    u16 dst = (u16)arg1;
    // use bitwise AND to get the last 2 bits and store value of lower half
    u16 dst_lower = ((u16)arg1) & 0x03;
    // use bitwise AND to get the last 2 bits and store value of higher  half
    // right shift to convert to and 8bit val
    u16 dst_higher = (((u16)arg1) & 0x0c) >> 2;
    switch (opcode)
    {
        /*mov ax; bin:1000*/
    case 0x08:
        set_reg(vm, 1, dst, dst_higher, dst_lower);
        break;
    /*mov bx; bin:1001*/
    case 0x09:
        set_reg(vm, 2, dst, dst_higher, dst_lower);
        break;

        /*mov cx; bin:1010*/
    case 0x0a:
        set_reg(vm, 3, dst, dst_higher, dst_lower);
        break;

        /*mov dx; bin:1011*/
    case 0x0b:
        set_reg(vm, 4, dst, dst_higher, dst_lower);
        break;

        /*mov sp; bin:1100*/
    case 0x0c:
        set_reg(vm, 5, dst, dst_higher, dst_lower);
        break;
        /*mov ax; bin:1101*/
        // case 0x0d:
        //   vm->c.r.sp = (Reg)dst;
        // break;
        /*mov ax; bin:1110*/
    // case 0x0e:
    //     vm->c.r.ax = (Reg)dst;
    //     break;
    //     /*mov [addr]; bin:1111*/
    // case 0x0f:
    // TODO!
    //     break;
    default:
        vm_error(vm, ErrInstr);
        break;
    }

    return;
}

#endif
