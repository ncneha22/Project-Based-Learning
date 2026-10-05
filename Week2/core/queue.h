#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 100

void queue_init(void);
int queue_enqueue(int value);
int queue_dequeue(int *value);
int queue_peek(int *value);
int queue_is_empty(void);
int queue_is_full(void);

#endif

