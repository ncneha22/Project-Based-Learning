#include <stdio.h>
#include "stack.h"

int main(void)
{
    int value;

    stack_init();

    stack_push(10);
    stack_push(20);
    stack_push(30);

    if (stack_peek(&value))
    {
        printf("PEEK: %d\n", value);
    }

    if (stack_pop(&value))
    {
        printf("POP: %d\n", value);
    }

    if (stack_pop(&value))
    {
        printf("POP: %d\n", value);
    }

    if (stack_pop(&value))
    {
        printf("POP: %d\n", value);
    }

    return 0;
}

