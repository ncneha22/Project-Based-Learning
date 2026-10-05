#include "ipc.h"

#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <sys/stat.h>

static mqd_t ui_to_core_queue;
static mqd_t core_to_logger_queue;

int ipc_open_queues(void)
{
    struct mq_attr attributes;

    attributes.mq_flags = 0;
    attributes.mq_maxmsg = MAX_MESSAGES;
    attributes.mq_msgsize = MAX_MESSAGE_SIZE;
    attributes.mq_curmsgs = 0;

    ui_to_core_queue = mq_open(
        UI_TO_CORE,
        O_CREAT | O_RDONLY,
        0666,
        &attributes
    );

    if (ui_to_core_queue == (mqd_t)-1)
    {
        perror("Error opening UI to Core queue");
        return 0;
    }

    core_to_logger_queue = mq_open(
        CORE_TO_LOGGER,
        O_CREAT | O_WRONLY,
        0666,
        &attributes
    );

    if (core_to_logger_queue == (mqd_t)-1)
    {
        perror("Error opening Core to Logger queue");
        mq_close(ui_to_core_queue);
        return 0;
    }

    return 1;
}

int ipc_receive(char *message)
{
    ssize_t bytes_received;

    bytes_received = mq_receive(
        ui_to_core_queue,
        message,
        MAX_MESSAGE_SIZE,
        NULL
    );

    if (bytes_received == -1)
    {
        perror("Error receiving message");
        return 0;
    }

    message[bytes_received] = '\0';

    return 1;
}

int ipc_send_log(const char *message)
{
    if (mq_send(
            core_to_logger_queue,
            message,
            strlen(message) + 1,
            0) == -1)
    {
        perror("Error sending message to logger");
        return 0;
    }

    return 1;
}

void ipc_close_queues(void)
{
    mq_close(ui_to_core_queue);
    mq_close(core_to_logger_queue);
}

void ipc_cleanup(void)
{
    mq_unlink(UI_TO_CORE);
    mq_unlink(CORE_TO_LOGGER);
}
