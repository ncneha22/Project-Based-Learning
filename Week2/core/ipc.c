#include "ipc.h"

#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * One IPC mechanism per receiving process:
 *   UI -> Core      UNIX domain socket   (Core receives on a socket)
 *   Core -> UI      POSIX message queue  (UI receives on a message queue)
 *   Core -> Logger  named FIFO (pipe)    (Logger receives on a pipe)
 */

static int server_fd = -1;
static int client_fd = -1;
static mqd_t core_to_ui_queue;

int ipc_open_queues(void)
{
    struct mq_attr attributes;
    struct sockaddr_un address;

    attributes.mq_flags = 0;
    attributes.mq_maxmsg = MAX_MESSAGES;
    attributes.mq_msgsize = MAX_MESSAGE_SIZE;
    attributes.mq_curmsgs = 0;

    core_to_ui_queue = mq_open(
        CORE_TO_UI,
        O_CREAT | O_WRONLY,
        0666,
        &attributes
    );

    if (core_to_ui_queue == (mqd_t)-1)
    {
        perror("Error opening Core to UI queue");
        return 0;
    }

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("Error creating socket");
        mq_close(core_to_ui_queue);
        return 0;
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, CORE_SOCKET_PATH, sizeof(address.sun_path) - 1);

    /* Remove a socket file left over from an earlier run */
    unlink(CORE_SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("Error binding socket");
        close(server_fd);
        mq_close(core_to_ui_queue);
        return 0;
    }

    if (listen(server_fd, 1) == -1)
    {
        perror("Error listening on socket");
        close(server_fd);
        mq_close(core_to_ui_queue);
        return 0;
    }

    printf("Waiting for UI to connect on %s...\n", CORE_SOCKET_PATH);

    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd == -1)
    {
        perror("Error accepting UI connection");
        close(server_fd);
        mq_close(core_to_ui_queue);
        return 0;
    }

    printf("UI connected.\n");

    return 1;
}

int ipc_receive(char *message)
{
    int length = 0;
    char c;
    ssize_t bytes_received;

    /*
     * A stream socket has no message boundaries, so the UI ends every
     * command with '\n' and we read one byte at a time up to it.
     */
    while (length < MAX_MESSAGE_SIZE - 1)
    {
        bytes_received = read(client_fd, &c, 1);

        if (bytes_received == -1)
        {
            perror("Error receiving message");
            return 0;
        }

        if (bytes_received == 0)
        {
            printf("UI disconnected.\n");
            return 0;
        }

        if (c == '\n')
        {
            break;
        }

        message[length] = c;
        length++;
    }

    message[length] = '\0';

    return 1;
}

int ipc_send_ui(const char *message)
{
    if (mq_send(
            core_to_ui_queue,
            message,
            strlen(message) + 1,
            0) == -1)
    {
        perror("Error sending message to UI");
        return 0;
    }

    return 1;
}

int ipc_send_log(const char *message)
{
    int fd;
    char buffer[MAX_MESSAGE_SIZE];

    fd = open(LOGGER_FIFO, O_WRONLY);

    if (fd == -1)
    {
        perror("Error opening logger FIFO");
        return 0;
    }

    if (strncmp(message, "ERROR:", 6) == 0)
    {
        snprintf(buffer, sizeof(buffer), "ERROR|%s", message);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "INFO|%s", message);
    }

    if (write(fd, buffer, strlen(buffer)) == -1)
    {
        perror("Error writing to logger FIFO");
        close(fd);
        return 0;
    }

    close(fd);

    return 1;
}

void ipc_close_queues(void)
{
    close(client_fd);
    close(server_fd);
    mq_close(core_to_ui_queue);

    /* core.c never calls ipc_cleanup(), so remove the socket file here */
    unlink(CORE_SOCKET_PATH);
}

void ipc_cleanup(void)
{
    unlink(CORE_SOCKET_PATH);
    mq_unlink(CORE_TO_UI);
}
