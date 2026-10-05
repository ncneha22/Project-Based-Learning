#include <stdio.h>
#include "queue.h"

int main(void)
{
    int value;

    queue_init();

    queue_enqueue(10);
    queue_enqueue(20);
    queue_enqueue(30);

    if (queue_peek(&value))
    {
        printf("PEEK: %d\n", value);
    }

    if (queue_dequeue(&value))
    {
        printf("DEQUEUE: %d\n", value);
    }

    if (queue_dequeue(&value))
    {
        printf("DEQUEUE: %d\n", value);
    }

    if (queue_dequeue(&value))
    {
        printf("DEQUEUE: %d\n", value);
    }

    return 0;
}
