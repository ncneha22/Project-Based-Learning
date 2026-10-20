#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

/* UI sends commands to Core over a UNIX domain socket */
#define CORE_SOCKET_PATH "/tmp/ipc_core_socket"

/* UI receives replies from Core on a POSIX message queue */
#define CORE_TO_UI "/core_to_ui"

#define MAX_MESSAGE_SIZE 256
#define MAX_MESSAGES 10

void send_command(int sock, const char *command)
{
    char buffer[MAX_MESSAGE_SIZE + 1];

    /* Newline marks the end of each command on the socket stream */
    snprintf(buffer, sizeof(buffer), "%s\n", command);

    if (write(sock, buffer, strlen(buffer)) == -1)
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

    mqd_t core_to_ui = mq_open(
        CORE_TO_UI,
        O_CREAT | O_RDONLY,
        0666,
        &attributes
    );

    if (core_to_ui == (mqd_t)-1)
    {
        perror("Error opening Core to UI queue");
        return 1;
    }

    struct sockaddr_un address;

    int ui_to_core = socket(AF_UNIX, SOCK_STREAM, 0);

    if (ui_to_core == -1)
    {
        perror("Error creating socket");
        mq_close(core_to_ui);
        return 1;
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, CORE_SOCKET_PATH, sizeof(address.sun_path) - 1);

    if (connect(ui_to_core, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("Error connecting to Core (is Core running?)");
        close(ui_to_core);
        mq_close(core_to_ui);
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

    close(ui_to_core);
    mq_close(core_to_ui);

    mq_unlink(CORE_TO_UI);

    printf("\nExiting simulator...\n");

    return 0;
}
