#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "ipc.h"

/*
 * Test UI: sends typed commands to Core over the UI -> Core socket.
 * Core's replies go to the /core_to_ui message queue and are not read here;
 * watch Core's terminal output instead.
 */
int main(void)
{
    int sock;
    struct sockaddr_un address;
    char message[256];
    char buffer[258];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock == -1)
    {
        perror("Error creating socket");
        return 1;
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, CORE_SOCKET_PATH, sizeof(address.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("Error connecting to Core (is Core running?)");
        close(sock);
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

        snprintf(buffer, sizeof(buffer), "%s\n", message);

        if (write(sock, buffer, strlen(buffer)) == -1)
        {
            perror("Error sending message");
            break;
        }

        if (strcmp(message, "EXIT") == 0)
        {
            break;
        }
    }

    close(sock);

    return 0;
}
