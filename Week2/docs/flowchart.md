# Week 2 - IPC Flowchart

## Multi-Process Simulator

```mermaid
flowchart TD
    A([Start]) --> B[Start Logger Process]
    B --> C[Start Core Process]
    C --> D[Start UI Process]

    D --> E[User Selects Operation]
    E --> F[UI Sends Command to Core]

    F -->|UNIX Domain Socket| G[Core Process]
    G --> H{Process Operation}

    H --> I[CPU Operations]
    H --> J[Memory Operations]
    H --> K[Stack Operations]
    H --> L[Queue Operations]

    I --> M[Prepare Result]
    J --> M
    K --> M
    L --> M

    M -->|POSIX Message Queue| N[Send Result to UI]
    N --> O[Display Result to User]

    G -->|Named FIFO| P[Logger Process]
    P --> Q{Message Type}
    Q -->|Normal Message| R[Write to execution.log]
    Q -->|Error Message| S[Write to error.log]

    O --> T{Exit Selected?}
    T -->|No| E
    T -->|Yes| U[Stop UI and Core]
    U --> V[Stop Logger]
    V --> W([End])
```

## IPC Communication Summary

1. **UI to Core:** UNIX domain socket at `/tmp/ipc_core_socket`.
2. **Core to UI:** POSIX message queue named `/core_to_ui`.
3. **Core to Logger:** Named FIFO at `/tmp/ipc_logger_fifo`.

## Execution Order

1. Start Logger.
2. Start Core and wait for the UI connection.
3. Start UI and perform operations.
4. Select Exit to stop the UI and Core.
5. Stop Logger separately.
