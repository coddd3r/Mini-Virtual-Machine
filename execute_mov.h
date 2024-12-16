#ifndef EXEC_MOV_H
#define EXEC_MOV_H

#include "mini_vm.h"
void exec_mov(VM *vm, Opcode opcode, Args arg1, Args arg2)
{
    u16 dst = (u16)arg1;
    switch (opcode)
    {
        /*mov ax; bin:1000*/
    case mov:
        vm->c.r.ax = (Reg)dst;
        break;
        /*mov bx; bin:1001*/
    case 0x09:
        vm->c.r.bx = (Reg)dst;
        break;
        /*mov cx; bin:1010*/
    case 0x0a:
        vm->c.r.cx = (Reg)dst;
        break;
        /*mov dx; bin:1011*/
    case 0x0b:
        vm->c.r.dx = (Reg)dst;
        break;
        /*mov sp; bin:1100*/
    case 0x0c:
        vm->c.r.sp = (Reg)dst;
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
