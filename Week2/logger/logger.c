#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/ipc_logger_fifo"
#define BUFFER_SIZE 1024

void log_execution(const char *message)
{
    FILE *file = fopen("logs/execution.log", "a");

    if (file == NULL)
    {
        perror("Could not open execution.log");
        return;
    }

    fprintf(file, "[INFO] %s\n", message);
    fclose(file);
}

void log_error(const char *message)
{
    FILE *file = fopen("logs/error.log", "a");

    if (file == NULL)
    {
        perror("Could not open error.log");
        return;
    }

    fprintf(file, "[ERROR] %s\n", message);
    fclose(file);
}

int main()
{
    char buffer[BUFFER_SIZE];

    printf("Logger process started.\n");

    /*
     * Create the named pipe if it does not already exist.
     */
    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        /*
         * The FIFO may already exist.
         * That is okay, so we continue.
         */
    }

    printf("Waiting for messages from Core...\n");

    /*
     * Open the FIFO for reading.
     */
    int fd = open(FIFO_PATH, O_RDONLY);

    if (fd == -1)
    {
        perror("Could not open FIFO");
        return 1;
    }

    /*
     * Keep receiving messages.
     */
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            printf("Received: %s\n", buffer);
            fflush(stdout);

            /*
             * INFO messages go to execution.log.
             */
            if (strncmp(buffer, "INFO|", 5) == 0)
            {
                log_execution(buffer + 5);
            }

            /*
             * ERROR messages go to error.log.
             */
            else if (strncmp(buffer, "ERROR|", 6) == 0)
            {
                log_error(buffer + 6);
            }

            /*
             * Unknown message type.
             */
            else
            {
                log_error("Unknown message received.");
            }
        }
    }

    close(fd);

    return 0;
}
