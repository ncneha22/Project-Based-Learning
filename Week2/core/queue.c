#include "queue.h"

static int queue[QUEUE_SIZE];
static int front;
static int rear;
static int count;

void queue_init(void)
{
    front = 0;
    rear = 0;
    count = 0;
}

int queue_enqueue(int value)
{
    if (queue_is_full())
    {
        return 0;
    }

    queue[rear] = value;
    rear = (rear + 1) % QUEUE_SIZE;
    count++;

    return 1;
}

int queue_dequeue(int *value)
{
    if (queue_is_empty())
    {
        return 0;
    }

    *value = queue[front];
    front = (front + 1) % QUEUE_SIZE;
    count--;

    return 1;
}

int queue_peek(int *value)
{
    if (queue_is_empty())
    {
        return 0;
    }

    *value = queue[front];

    return 1;
}

int queue_is_empty(void)
{
    return count == 0;
}

int queue_is_full(void)
{
    return count == QUEUE_SIZE;
}
