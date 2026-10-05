#include <stdio.h>
#include <string.h>

#include "ipc.h"
#include "cpu.h"
#include "memory.h"
#include "stack.h"
#include "queue.h"

int execute_command(const char *command)
{
    char operation[20];
    int a, b;
    int result;

    if (sscanf(command, "%19s %d %d", operation, &a, &b) >= 1)
    {
        if (strcmp(operation, "ADD") == 0)
        {
            result = cpu_add(a, b);
            printf("CPU: %d\n", result);
            return 1;
        }

        if (strcmp(operation, "SUB") == 0)
        {
            result = cpu_sub(a, b);
            printf("CPU: %d\n", result);
            return 1;
        }

        if (strcmp(operation, "MUL") == 0)
        {
            result = cpu_mul(a, b);
            printf("CPU: %d\n", result);
            return 1;
        }

        if (strcmp(operation, "DIV") == 0)
        {
            if (b == 0)
            {
                printf("ERROR: Division by zero\n");
                return 0;
            }

            result = cpu_div(a, b);
            printf("CPU: %d\n", result);
            return 1;
        }

        if (strcmp(operation, "STORE") == 0)
        {
            memory_store(a, b);
            printf("Memory: stored %d at address %d\n", b, a);
            return 1;
        }

        if (strcmp(operation, "LOAD") == 0)
        {
            result = memory_load(a);
            printf("Memory: address %d = %d\n", a, result);
            return 1;
        }

        if (strcmp(operation, "PUSH") == 0)
        {
            if (stack_push(a))
            {
                printf("Stack: pushed %d\n", a);
                return 1;
            }

            printf("ERROR: Stack is full\n");
            return 0;
        }

        if (strcmp(operation, "POP") == 0)
        {
            if (stack_pop(&result))
            {
                printf("Stack: popped %d\n", result);
                return 1;
            }

            printf("ERROR: Stack is empty\n");
            return 0;
        }

        if (strcmp(operation, "PEEK") == 0)
        {
            if (stack_peek(&result))
            {
                printf("Stack: top = %d\n", result);
                return 1;
            }

            printf("ERROR: Stack is empty\n");
            return 0;
        }

        if (strcmp(operation, "ENQUEUE") == 0)
        {
            if (queue_enqueue(a))
            {
                printf("Queue: added %d\n", a);
                return 1;
            }

            printf("ERROR: Queue is full\n");
            return 0;
        }

        if (strcmp(operation, "DEQUEUE") == 0)
        {
            if (queue_dequeue(&result))
            {
                printf("Queue: removed %d\n", result);
                return 1;
            }

            printf("ERROR: Queue is empty\n");
            return 0;
        }

        if (strcmp(operation, "QPEEK") == 0)
        {
            if (queue_peek(&result))
            {
                printf("Queue: front = %d\n", result);
                return 1;
            }

            printf("ERROR: Queue is empty\n");
            return 0;
        }
    }

    printf("ERROR: Unknown command\n");
    return 0;
}

int main(void)
{
    char command[256];

    memory_init();
    stack_init();
    queue_init();

    printf("Core Process Started\n");

    if (!ipc_open_queues())
    {
        printf("Failed to open IPC queues.\n");
        return 1;
    }

    printf("Waiting for commands from UI...\n");

    while (1)
    {
        if (!ipc_receive(command))
        {
            break;
        }

        printf("Received: %s\n", command);

        if (strcmp(command, "EXIT") == 0)
        {
            ipc_send_log("Core Process Stopped");
            break;
        }

        execute_command(command);
    }

    ipc_close_queues();

    return 0;
}
