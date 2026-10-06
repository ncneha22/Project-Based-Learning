# IPC Design

## Objective

To separate the simulator into three independent processes and enable communication between them using POSIX IPC mechanisms.

## Processes

### 1. UI Process

The UI process interacts with the user. It accepts commands and input from the user and sends them to the Core process.

### 2. Core Process

The Core process performs the main simulator operations such as CPU operations, memory operations, stack operations, and queue operations.

### 3. Logger Process

The Logger process receives messages from the Core process and records execution and error information in log files.

## IPC Mechanisms Used

### UI ↔ Core: POSIX Message Queue

A POSIX Message Queue is used for communication between the UI and Core processes.

- UI sends commands to Core using `/ui_to_core`.
- Core sends responses back to UI using `/core_to_ui`.
- This provides a structured way to exchange messages between the two processes.

### Core → Logger: Named FIFO

A Named FIFO is used for communication between Core and Logger.

- Core sends log messages through `/tmp/ipc_logger_fifo`.
- Logger reads these messages from the FIFO.
- Logger records the received information in log files.

## Communication Flow

```text
                 User
                   |
                   v
              +---------+
              |   UI    |
              +---------+
                   |
                   | POSIX Message Queue
                   v
              +---------+
              |  Core   |
              +---------+
               /       \
              /         \
             v           v
       CPU/Memory/    Named FIFO
       Stack/Queue        |
                          v
                    +-----------+
                    |  Logger   |
                    +-----------+
                          |
                          v
                       Log Files
Reason for Selecting These IPC Mechanisms
POSIX Message Queues were selected for UI-Core communication because they provide structured message-based communication between independent processes.
A Named FIFO was selected for Core-Logger communication because logging messages can be sent as a simple stream from Core to Logger.

IPC Verification
The communication was tested using all simulator operations.
- UI successfully sent commands to Core.
- Core successfully processed the commands.
- Core successfully sent responses back to UI.
- Core successfully sent log messages to Logger.
- Logger successfully received and recorded the messages.

Conclusion
The simulator was successfully separated into UI, Core, and Logger processes. POSIX Message Queues were used for UI-Core communication, while a Named FIFO was used for Core-Logger communication. The IPC mechanisms successfully enabled communication between the independent processes.
