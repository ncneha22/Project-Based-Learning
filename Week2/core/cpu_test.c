#include <stdio.h>
#include "cpu.h"

int main()
{
    printf("ADD: %d\n", cpu_add(10, 20));
    printf("SUB: %d\n", cpu_sub(20, 5));
    printf("MUL: %d\n", cpu_mul(5, 4));
    printf("DIV: %d\n", cpu_div(20, 5));

    return 0;
}

