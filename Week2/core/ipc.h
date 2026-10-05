#ifndef IPC_H
#define IPC_H

#define UI_TO_CORE "/ui_to_core"
#define CORE_TO_UI "/core_to_ui"

#define LOGGER_FIFO "/tmp/ipc_logger_fifo"

#define MAX_MESSAGE_SIZE 256
#define MAX_MESSAGES 10

int ipc_open_queues(void);

int ipc_receive(char *message);

int ipc_send_ui(const char *message);

int ipc_send_log(const char *message);

void ipc_close_queues(void);

void ipc_cleanup(void);

#endif
