# IPC Design

## 1. Objective

To separate the simulator into three independent processes and
enable communication using appropriate Inter-Process
Communication (IPC) mechanisms.

## 2. Processes

### 2.1 UI Process

The UI process accepts commands and input from the user,
sends commands to Core, and displays the responses received.

### 2.2 Core Process

The Core process performs CPU, memory, stack, and queue
operations. It sends results to UI and log messages to Logger.

### 2.3 Logger Process

The Logger process receives messages from Core and records
execution and error information in log files.

## 3. IPC Mechanisms Used

### 3.1 UI to Core: UNIX Domain Socket

- The UI sends commands to Core through a UNIX domain stream socket.
- Socket path: `/tmp/ipc_core_socket`
- Commands are sent as text messages.

### 3.2 Core to UI: POSIX Message Queue

- Core sends operation results and error responses to UI.
- Message queue name: `/core_to_ui`
- The UI receives the messages and displays them to the user.

### 3.3 Core to Logger: Named FIFO

- Core sends log messages through a named FIFO.
- FIFO path: `/tmp/ipc_logger_fifo`
- Logger reads the messages and records them in log files.
- Execution messages are recorded in `logger/logs/execution.log`.
- Error messages are recorded in `logger/logs/error.log`.

## 4. Communication Flow

```text
User
 |
 v
+----------------+
|   UI Process   |
+----------------+
 |
 | UNIX Domain Socket
 | Commands
 v
+----------------+
|  Core Process  |
| CPU / Memory   |
| Stack / Queue  |
+----------------+
 |             |
 | Results     | Log messages
 | POSIX MQ    | Named FIFO
 v             v
+----------------+    +----------------+
|   UI Process   |    | Logger Process |
| Displays result|    | Writes log files|
+----------------+    +----------------+

```

## 5. Reason for Selecting These Mechanisms

A UNIX domain socket provides local communication between UI
and Core. A POSIX message queue transfers Core responses to UI.
A named FIFO provides a simple channel for sending log messages
from Core to Logger.

## 6. Verification

The following behaviours were manually tested:

- Arithmetic operations: ADD, SUBTRACT, MULTIPLY, and DIVIDE.
- Memory operations: STORE and LOAD.
- Stack operations: PUSH, POP, and PEEK.
- Queue operations: ENQUEUE, DEQUEUE, and QUEUE PEEK.
- UI received responses from Core.
- Logger received and recorded messages from Core.
- Division by zero produced an error response.
- The division-by-zero error was recorded in `error.log`.

## 7. Conclusion

The simulator uses three IPC mechanisms: a UNIX domain socket
for UI-to-Core commands, a POSIX message queue for Core-to-UI
responses, and a named FIFO for Core-to-Logger log messages.
