#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 100

void stack_init(void);
int stack_push(int value);
int stack_pop(int *value);
int stack_peek(int *value);
int stack_is_empty(void);
int stack_is_full(void);

#endif
