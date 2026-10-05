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
    char response[256];

    if (sscanf(command, "%19s %d %d", operation, &a, &b) >= 1)
    {
        if (strcmp(operation, "ADD") == 0)
        {
            result = cpu_add(a, b);

            snprintf(response, sizeof(response),
                     "CPU: %d", result);

            printf("%s\n", response);
            ipc_send_ui(response);
            ipc_send_log(response);

            return 1;
        }

        if (strcmp(operation, "SUB") == 0)
        {
            result = cpu_sub(a, b);

            snprintf(response, sizeof(response),
                     "CPU: %d", result);

            printf("%s\n", response);
            ipc_send_ui(response);

            return 1;
        }

        if (strcmp(operation, "MUL") == 0)
        {
            result = cpu_mul(a, b);

            snprintf(response, sizeof(response),
                     "CPU: %d", result);

            printf("%s\n", response);
            ipc_send_ui(response);

            return 1;
        }

        if (strcmp(operation, "DIV") == 0)
        {
            if (b == 0)
            {
                snprintf(response, sizeof(response),
                         "ERROR: Division by zero");

                printf("%s\n", response);
                ipc_send_ui(response);

                return 0;
            }

            result = cpu_div(a, b);

            snprintf(response, sizeof(response),
                     "CPU: %d", result);

            printf("%s\n", response);
            ipc_send_ui(response);

            return 1;
        }

        if (strcmp(operation, "STORE") == 0)
        {
            memory_store(a, b);

            snprintf(response, sizeof(response),
                     "Memory: stored %d at address %d", b, a);

            printf("%s\n", response);
            ipc_send_ui(response);

            return 1;
        }

        if (strcmp(operation, "LOAD") == 0)
        {
            result = memory_load(a);

            snprintf(response, sizeof(response),
                     "Memory: address %d = %d", a, result);

            printf("%s\n", response);
            ipc_send_ui(response);

            return 1;
        }

        if (strcmp(operation, "PUSH") == 0)
        {
            if (stack_push(a))
            {
                snprintf(response, sizeof(response),
                         "Stack: pushed %d", a);

                printf("%s\n", response);
                ipc_send_ui(response);

                return 1;
            }

            snprintf(response, sizeof(response),
                     "ERROR: Stack is full");

            printf("%s\n", response);
            ipc_send_ui(response);

            return 0;
        }

        if (strcmp(operation, "POP") == 0)
        {
            if (stack_pop(&result))
            {
                snprintf(response, sizeof(response),
                         "Stack: popped %d", result);

                printf("%s\n", response);
                ipc_send_ui(response);

                return 1;
            }

            snprintf(response, sizeof(response),
                     "ERROR: Stack is empty");

            printf("%s\n", response);
            ipc_send_ui(response);

            return 0;
        }

        if (strcmp(operation, "PEEK") == 0)
        {
            if (stack_peek(&result))
            {
                snprintf(response, sizeof(response),
                         "Stack: top = %d", result);

                printf("%s\n", response);
                ipc_send_ui(response);

                return 1;
            }

            snprintf(response, sizeof(response),
                     "ERROR: Stack is empty");

            printf("%s\n", response);
            ipc_send_ui(response);

            return 0;
        }

        if (strcmp(operation, "ENQUEUE") == 0)
        {
            if (queue_enqueue(a))
            {
                snprintf(response, sizeof(response),
                         "Queue: added %d", a);

                printf("%s\n", response);
                ipc_send_ui(response);

                return 1;
            }

            snprintf(response, sizeof(response),
                     "ERROR: Queue is full");

            printf("%s\n", response);
            ipc_send_ui(response);

            return 0;
        }

        if (strcmp(operation, "DEQUEUE") == 0)
        {
            if (queue_dequeue(&result))
            {
                snprintf(response, sizeof(response),
                         "Queue: removed %d", result);

                printf("%s\n", response);
                ipc_send_ui(response);

                return 1;
            }

            snprintf(response, sizeof(response),
                     "ERROR: Queue is empty");

            printf("%s\n", response);
            ipc_send_ui(response);

            return 0;
        }

        if (strcmp(operation, "QPEEK") == 0)
        {
            if (queue_peek(&result))
            {
                snprintf(response, sizeof(response),
                         "Queue: front = %d", result);

                printf("%s\n", response);
                ipc_send_ui(response);

                return 1;
            }

            snprintf(response, sizeof(response),
                     "ERROR: Queue is empty");

            printf("%s\n", response);
            ipc_send_ui(response);

            return 0;
        }
    }

    snprintf(response, sizeof(response),
             "ERROR: Unknown command");

    printf("%s\n", response);
    ipc_send_ui(response);

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
            ipc_send_ui("Core Process Stopped");
            ipc_send_log("Core Process Stopped");
            break;
        }

        execute_command(command);
    }

    ipc_close_queues();

    return 0;
}
