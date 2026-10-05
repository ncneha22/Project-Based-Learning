#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>

#define UI_TO_CORE "/ui_to_core"
#define CORE_TO_UI "/core_to_ui"

#define MAX_MESSAGE_SIZE 256
#define MAX_MESSAGES 10

void send_command(mqd_t queue, const char *command)
{
    if (mq_send(queue, command, strlen(command) + 1, 0) == -1)
    {
        perror("Error sending command");
    }
}

void receive_response(mqd_t queue)
{
    char response[MAX_MESSAGE_SIZE];

    ssize_t bytes_received = mq_receive(
        queue,
        response,
        MAX_MESSAGE_SIZE,
        NULL
    );

    if (bytes_received == -1)
    {
        perror("Error receiving response");
        return;
    }

    response[bytes_received] = '\0';

    printf("\nCore Response: %s\n", response);
}

int main()
{
    struct mq_attr attributes;

    attributes.mq_flags = 0;
    attributes.mq_maxmsg = MAX_MESSAGES;
    attributes.mq_msgsize = MAX_MESSAGE_SIZE;
    attributes.mq_curmsgs = 0;

    mqd_t ui_to_core = mq_open(
        UI_TO_CORE,
        O_CREAT | O_WRONLY,
        0666,
        &attributes
    );

    if (ui_to_core == (mqd_t)-1)
    {
        perror("Error opening UI to Core queue");
        return 1;
    }

    mqd_t core_to_ui = mq_open(
        CORE_TO_UI,
        O_CREAT | O_RDONLY,
        0666,
        &attributes
    );

    if (core_to_ui == (mqd_t)-1)
    {
        perror("Error opening Core to UI queue");
        mq_close(ui_to_core);
        return 1;
    }

    int choice;
    int a, b;
    char command[MAX_MESSAGE_SIZE];

    while (1)
    {
        printf("\n====================================\n");
        printf("       MULTI-PROCESS SIMULATOR\n");
        printf("====================================\n");

        printf("\n1. ADD\n");
        printf("2. SUBTRACT\n");
        printf("3. MULTIPLY\n");
        printf("4. DIVIDE\n");
        printf("5. STORE\n");
        printf("6. LOAD\n");
        printf("7. PUSH\n");
        printf("8. POP\n");
        printf("9. PEEK\n");
        printf("10. ENQUEUE\n");
        printf("11. DEQUEUE\n");
        printf("12. QUEUE PEEK\n");
        printf("13. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 13)
        {
            send_command(ui_to_core, "EXIT");
            receive_response(core_to_ui);
            break;
        }

        switch (choice)
        {
            case 1:
                printf("Enter two numbers: ");
                scanf("%d %d", &a, &b);
                snprintf(command, sizeof(command), "ADD %d %d", a, b);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 2:
                printf("Enter two numbers: ");
                scanf("%d %d", &a, &b);
                snprintf(command, sizeof(command), "SUB %d %d", a, b);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 3:
                printf("Enter two numbers: ");
                scanf("%d %d", &a, &b);
                snprintf(command, sizeof(command), "MUL %d %d", a, b);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 4:
                printf("Enter two numbers: ");
                scanf("%d %d", &a, &b);
                snprintf(command, sizeof(command), "DIV %d %d", a, b);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 5:
                printf("Enter address and value: ");
                scanf("%d %d", &a, &b);
                snprintf(command, sizeof(command), "STORE %d %d", a, b);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 6:
                printf("Enter address: ");
                scanf("%d", &a);
                snprintf(command, sizeof(command), "LOAD %d", a);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 7:
                printf("Enter value: ");
                scanf("%d", &a);
                snprintf(command, sizeof(command), "PUSH %d", a);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 8:
                send_command(ui_to_core, "POP");
                receive_response(core_to_ui);
                break;

            case 9:
                send_command(ui_to_core, "PEEK");
                receive_response(core_to_ui);
                break;

            case 10:
                printf("Enter value: ");
                scanf("%d", &a);
                snprintf(command, sizeof(command), "ENQUEUE %d", a);
                send_command(ui_to_core, command);
                receive_response(core_to_ui);
                break;

            case 11:
                send_command(ui_to_core, "DEQUEUE");
                receive_response(core_to_ui);
                break;

            case 12:
                send_command(ui_to_core, "QPEEK");
                receive_response(core_to_ui);
                break;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    mq_close(ui_to_core);
    mq_close(core_to_ui);

    mq_unlink(UI_TO_CORE);
    mq_unlink(CORE_TO_UI);

    printf("\nExiting simulator...\n");

    return 0;
}
