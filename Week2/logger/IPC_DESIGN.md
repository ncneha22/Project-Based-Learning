# Logger IPC Design

## 1. Logger Responsibility

The Logger process is responsible for recording:

- Successful process execution messages
- Error messages

The Logger receives these messages from the Core process
through an IPC mechanism.

## 2. IPC Mechanism

The Logger uses a POSIX named pipe (FIFO).

FIFO path:

/tmp/ipc_logger_fifo

The FIFO allows the Core process and Logger process to
communicate with each other.

## 3. Message Format

The Core process will send messages using two formats.

For execution messages:

INFO|message

For error messages:

ERROR|message

Example:

INFO|Process execution completed.

Example:

ERROR|Invalid instruction received.

## 4. Message Flow

Core Process
     |
     | INFO| or ERROR|
     v
POSIX FIFO
     |
     v
Logger Process
     |
     +-------------> execution.log
     |
     +-------------> error.log

## 5. Log Files

INFO messages are written to:

logs/execution.log

ERROR messages are written to:

logs/error.log

## 6. Logger Responsibilities

The Logger will:

1. Create the FIFO if it does not exist.
2. Wait for messages from the Core process.
3. Read incoming messages.
4. Identify INFO and ERROR messages.
5. Write INFO messages to execution.log.
6. Write ERROR messages to error.log.
7. Continue waiting for more messages.

## 7. Why FIFO?

A POSIX named pipe provides a simple way for separate
processes to communicate in Linux.

The Core process can send logging information through the
FIFO and the Logger process can receive and record it.