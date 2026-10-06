#include <stdio.h>
#include "cpu.h"
#include "memory.h"
#include "stack.h"
#include "queue.h"

int main()
{
    int i;
    int result;
    int value;

    memory_init();
    stack_init();
    queue_init();

    /* CPU operations */
    for (i = 0; i < 1000000; i++)
    {
        result = cpu_add(10, 20);
        result = cpu_sub(20, 5);
        result = cpu_mul(5, 4);
        result = cpu_div(20, 5);
    }

    /* Memory operations */
    for (i = 0; i < 1000000; i++)
    {
        memory_store(10, 50);
        value = memory_load(10);
    }

    /* Stack operations */
    for (i = 0; i < 100000; i++)
    {
        stack_push(100);
        stack_pop(&value);
    }

    /* Queue operations */
    for (i = 0; i < 100000; i++)
    {
        queue_enqueue(300);
        queue_dequeue(&value);
    }

    printf("Standalone simulator completed.\n");

    return 0;
}
