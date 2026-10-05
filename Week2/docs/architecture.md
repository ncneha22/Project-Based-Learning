# Multi-Process Simulator Architecture

## Processes

UI Process
    |
    | User commands
    v
POSIX Message Queue
    |
    v
Core Process
    |
    | Execution / Error information
    v
POSIX Message Queue
    |
    v
Logging Process

Core Process
    |
    | Result
    v
UI Process

# Multi-Process Simulator Architecture

## Processes

UI Process
    |
    | User commands
    v
POSIX Message Queue
    |
    v
Core Process
    |
    | Execution / Error information
    v
POSIX Message Queue
    |
    v
Logging Process

Core Process
    |
    | Result
    v
UI Process

## Core Components

The Core Process contains:
- CPU
- Memory
- Stack
- Queue

## IPC Mechanism

The project uses POSIX Message Queues for communication between the independent processes.

POSIX Message Queues provide simple, organized and asynchronous communication between the UI, Core and Logging processes.
