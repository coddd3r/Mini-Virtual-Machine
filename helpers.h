#ifndef HELPERS_H
#define HELPERS_H
#include "mini_vm.h"
void zero_out(u8 *str, u16 size)
{
    u8 *p;
    u16 n;
    for (n = 0, p = str; n < size; p++, n++)
        *p = 0;
}

void copy(u8 *destination, u8 *source, u16 size)
{
    u8 *dst;
    u8 *src;
    u16 n;
    for (n = size, dst = destination, src = source; n > 0; n--, src++, dst++)
        *dst = *src;
    return;
}

void print_hex(u8 *text, u16 size, u8 delimiter)
{
    u8 *p;
    u16 n;
    for (p = text, n = size; n > 0; n--, p++)
    {
        printf("%.02x", *p);
        if (delimiter)
            printf("%c", delimiter);
        fflush(stdout);
    }
    printf("\n");
    return;
}

#endif
