#include "stack.h"

static int stack[STACK_SIZE];
static int top;

void stack_init(void)
{
    top = -1;
}

int stack_push(int value)
{
    if (stack_is_full())
    {
        return 0;
    }

    top++;
    stack[top] = value;

    return 1;
}

int stack_pop(int *value)
{
    if (stack_is_empty())
    {
        return 0;
    }

    *value = stack[top];
    top--;

    return 1;
}

int stack_peek(int *value)
{
    if (stack_is_empty())
    {
        return 0;
    }

    *value = stack[top];

    return 1;
}

int stack_is_empty(void)
{
    return top == -1;
}

int stack_is_full(void)
{
    return top == STACK_SIZE - 1;
}

