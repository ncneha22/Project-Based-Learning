# Multi-Process Simulator Architecture

## Architecture Diagram

                 ┌─────────────────┐
                 │    UI Process   │
                 │ User Interaction│
                 └────────┬────────┘
                          │
                    User Commands
                          │
                          ▼
              ┌──────────────────────┐
              │ POSIX Message Queue  │
              └──────────┬───────────┘
                         │
                         ▼
                 ┌───────────────┐
                 │ Core Process  │
                 │               │
                 │ CPU           │
                 │ Memory        │
                 │ Stack         │
                 │ Queue         │
                 └───────┬───────┘
                         │
                 Execution / Error
                         │
                         ▼
              ┌──────────────────────┐
              │ POSIX Message Queue  │
              └──────────┬───────────┘
                         │
                         ▼
                 ┌───────────────┐
                 │Logger Process │
                 │Execution and  │
                 │Error Logging  │
                 └───────────────┘

                 Core Process
                      │
                    Result
                      │
                      ▼
                 UI Process

## IPC Mechanism

The project uses POSIX Message Queues for communication between the independent processes.

POSIX Message Queues provide simple, organized and asynchronous communication between the UI, Core and Logging processes.
