#include "cpu.h"

int cpu_add(int a, int b)
{
    return a + b;
}

int cpu_sub(int a, int b)
{
    return a - b;
}

int cpu_mul(int a, int b)
{
    return a * b;
}

int cpu_div(int a, int b)
{
    if (b == 0)
    {
        return 0;
    }

    return a / b;
}

