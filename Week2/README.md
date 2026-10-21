# Multi-Process Simulator - Week 2

## 1. Project Overview

The Multi-Process Simulator demonstrates inter-process
communication (IPC) using three independent processes:
UI, Core, and Logger.

The Core process supports arithmetic, memory, stack, and
queue operations.

## 2. IPC Architecture

The project uses three IPC mechanisms:

- **UI to Core:** UNIX domain socket
  - Path: `/tmp/ipc_core_socket`
- **Core to UI:** POSIX message queue
  - Name: `/core_to_ui`
- **Core to Logger:** Named FIFO
  - Path: `/tmp/ipc_logger_fifo`

The Logger records execution messages in
`logger/logs/execution.log` and error messages in
`logger/logs/error.log`.

For details, see `docs/architecture.md` and
`docs/IPC_DESIGN.md`.

## 3. Requirements

- Linux or WSL with Ubuntu
- GCC compiler
- POSIX message queue support
- Three terminal sessions to run the processes

## 4. Compile the Programs

Run these commands from the Week2 directory:

```bash
mkdir -p bin

gcc -Wall -Wextra core/core.c core/ipc.c core/cpu.c \
core/memory.c core/stack.c core/queue.c -o bin/core -lrt

gcc -Wall -Wextra ui/ui.c -o bin/ui -lrt

gcc -Wall -Wextra logger/logger.c -o bin/logger
```

## 5. Run the Simulator

Use three separate Ubuntu terminals.

### Terminal 1: Start Logger

Run Logger from its directory so its log files are stored correctly.

```bash
cd ~/Project-Based-Learning/Week2/logger
../bin/logger
```

### Terminal 2: Start Core

```bash
cd ~/Project-Based-Learning/Week2
./bin/core
```

Wait until Core displays that it is waiting for UI to connect.

### Terminal 3: Start UI

```bash
cd ~/Project-Based-Learning/Week2
./bin/ui
```

Use the displayed menu to perform operations. Choose option
13 to exit the UI and Core processes.

The Logger can be stopped with Ctrl+C in its terminal.

## 6. Available Operations

1. ADD
2. SUBTRACT
3. MULTIPLY
4. DIVIDE
5. STORE
6. LOAD
7. PUSH
8. POP
9. PEEK
10. ENQUEUE
11. DEQUEUE
12. QUEUE PEEK
13. Exit

## 7. Testing

Functional operations and IPC communication were manually
tested during integration.

Test cases are documented in `tests/IPC_TEST_CASES.md`.
Benchmark information is documented in
`benchmark/BENCHMARK_RESULTS.md`.

## 8. Important Notes

- Start Logger before Core because Core sends messages through
  the named FIFO.
- Start Core before UI because Core waits for UI to connect.
- The benchmark measurements are historical and have not
  been reproduced during the current integration testing.
- Log files are stored under `logger/logs/`.

## 9. Project Structure

- `ui/` - User interface process
- `core/` - Core operations and IPC implementation
- `logger/` - Logging process
- `docs/` - Architecture and IPC documentation
- `tests/` - Test cases
- `benchmark/` - Benchmark source files and results
