#include <stdio.h>
#include "memory.h"

int main(void)
{
    memory_init();

    memory_store(0, 100);
    memory_store(1, 200);
    memory_store(2, 300);

    printf("Memory[0]: %d\n", memory_load(0));
    printf("Memory[1]: %d\n", memory_load(1));
    printf("Memory[2]: %d\n", memory_load(2));

    return 0;
}
