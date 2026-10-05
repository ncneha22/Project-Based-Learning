#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

#include "ipc.h"

int main(void)
{
    mqd_t queue;
    char message[256];

    queue = mq_open(UI_TO_CORE, O_WRONLY);

    if (queue == (mqd_t)-1)
    {
        perror("Error opening UI to Core queue");
        return 1;
    }

    printf("Test UI started.\n");

    while (1)
    {
        printf("UI> ");

        if (fgets(message, sizeof(message), stdin) == NULL)
        {
            break;
        }

        message[strcspn(message, "\n")] = '\0';

        if (mq_send(
                queue,
                message,
                strlen(message) + 1,
                0) == -1)
        {
            perror("Error sending message");
            break;
        }

        if (strcmp(message, "EXIT") == 0)
        {
            break;
        }
    }

    mq_close(queue);

    return 0;
}

