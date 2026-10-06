#include "memory.h"
#include <stdio.h>

static int memory[MEMORY_SIZE];

void memory_init(void)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i] = 0;
    }
}

void memory_store(int address, int value)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("ERROR: Invalid memory address\n");
        return;
    }

    memory[address] = value;
}

int memory_load(int address)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("ERROR: Invalid memory address\n");
        return 0;
    }

    return memory[address];
}
