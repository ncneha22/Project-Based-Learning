#include <stdio.h>
#include <mqueue.h>
#include <string.h>

#include "ipc.h"

int main(void)
{
    mqd_t queue;
    char message[MAX_MESSAGE_SIZE];

    queue = mq_open(CORE_TO_LOGGER, O_RDONLY);

    if (queue == (mqd_t)-1)
    {
        perror("Error opening Core to Logger queue");
        return 1;
    }

    printf("Test Logger started.\n");
    printf("Waiting for logs...\n");

    while (1)
    {
        ssize_t bytes_received;

        bytes_received = mq_receive(
            queue,
            message,
            MAX_MESSAGE_SIZE,
            NULL
        );

        if (bytes_received == -1)
        {
            perror("Error receiving log");
            break;
        }

        message[bytes_received] = '\0';

        printf("LOGGER: %s\n", message);

        if (strcmp(message, "Core Process Stopped") == 0)
        {
            break;
        }
    }

    mq_close(queue);

    return 0;
}

