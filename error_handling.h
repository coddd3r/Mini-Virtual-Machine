#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H
#include "mini_vm.h"

void vm_error(VM *vm, ErrorCode e)
{
    u8 exit_code = -1;
    switch (e)
    {
    case ErrSegv:
        fprintf(stderr, "%s\n", "VM Segmentation fault");
        break;

    case ErrInstr:
        fprintf(stderr, "%s\n", "VM got illegal instruction");
        break;

    case SysHlt:
        fprintf(stderr, "%s\n", "VM System halted ");
        printf("cx register: %.04hx\n", vm->c.r.cx);
        exit_code = 0;
        if equal (vm)
            printf("eflag set\n");
        if gt (vm)
            printf("gt flag set\n");
        if higher (vm)
            printf("higher half flag set\n");
        break;

    default:
        break;
    }
    if (vm)
        free(vm);
    exit((int)exit_code);
}

#endif
