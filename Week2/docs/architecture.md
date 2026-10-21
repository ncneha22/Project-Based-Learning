# Multi-Process Simulator Architecture

## 1. Architecture Overview

The Multi-Process Simulator consists of three independent processes:
UI, Core, and Logger.

Each process communicates using a specific Inter-Process
Communication (IPC) mechanism.

## 2. Architecture Diagram

```text
+----------------------+
|      UI Process      |
|   User Interaction   |
+----------------------+
           |
           | UI Commands
           | UNIX Domain Socket
           v
+----------------------+
|     Core Process     |
|                      |
|  +----------------+  |
|  |      CPU       |  |
|  +----------------+  |
|  |     Memory     |  |
|  +----------------+  |
|  |     Stack      |  |
|  +----------------+  |
|  |     Queue      |  |
|  +----------------+  |
+----------------------+
       |          |
       |          |
       |          +----------------------+
       |                                 |
       | Results                         | Execution and Error Logs
       | POSIX Message Queue             | Named FIFO
       v                                 v
+----------------------+       +----------------------+
|      UI Process      |       |    Logger Process    |
|  Displays Results    |       |    Records Logs      |
+----------------------+       +----------------------+
```

## 3. IPC Mechanisms

### 3.1 UI to Core - UNIX Domain Socket

The UI sends user commands to the Core process through a UNIX domain stream socket.

Socket path: `/tmp/ipc_core_socket`

### 3.2 Core to UI - POSIX Message Queue

The Core process sends operation results and error responses to the UI through a POSIX message queue.

Message queue name: `/core_to_ui`

### 3.3 Core to Logger - Named FIFO

The Core process sends execution messages and errors to the Logger process through a named FIFO.

FIFO path: `/tmp/ipc_logger_fifo`

The Logger records messages in `logger/logs/execution.log` and `logger/logs/error.log`.

## 4. Core Components

- CPU: Performs arithmetic operations.
- Memory: Stores and retrieves values using memory addresses.
- Stack: Supports PUSH, POP, and PEEK operations.
- Queue: Supports ENQUEUE, DEQUEUE, and QUEUE PEEK operations.

## 5. Process Startup Order

1. Start the Logger process.
2. Start the Core process.
3. Start the UI process.

The Logger must be running before the Core sends messages through the named FIFO.

## 6. Summary

The simulator uses three IPC mechanisms: UNIX domain sockets for UI-to-Core commands, POSIX message queues for Core-to-UI responses, and a named FIFO for Core-to-Logger messages.
