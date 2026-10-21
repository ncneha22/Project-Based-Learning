# IPC Test Cases

## 1. Objective

To verify communication between the UI, Core, and Logger
processes and test the simulator operations.

## 2. Functional Test Cases

| Test Case | Operation | Input | Observed Result | Status |
|---|---|---|---|---|
| TC01 | ADD | 10, 20 | CPU: 30 | PASS |
| TC02 | SUBTRACT | 20, 5 | CPU: 15 | PASS |
| TC03 | MULTIPLY | 5, 4 | CPU: 20 | PASS |
| TC04 | DIVIDE | 20, 5 | CPU: 4 | PASS |
| TC05 | DIVIDE BY ZERO | 20, 0 | ERROR: Division by zero | PASS |
| TC06 | STORE | Address 10, Value 50 | Memory: stored 50 at address 10 | PASS |
| TC07 | LOAD | Address 10 | Memory: address 10 = 50 | PASS |
| TC08 | PUSH | 100 | Stack: pushed 100 | PASS |
| TC09 | POP | After PUSH 100 | Stack: popped 100 | PASS |
| TC10 | PEEK | Empty stack | ERROR: Stack is empty | PASS |
| TC11 | ENQUEUE | 300 | Queue: added 300 | PASS |
| TC12 | DEQUEUE | After ENQUEUE 300 | Queue: removed 300 | PASS |
| TC13 | QUEUE PEEK | Empty queue | ERROR: Queue is empty | PASS |
| TC14 | EXIT | — | UI and Core exited | PASS |

## 3. IPC Verification

- UI sends commands to Core through a UNIX domain socket.
- Core sends responses to UI through a POSIX message queue.
- Core sends log messages to Logger through a named FIFO.
- Logger received and recorded execution messages.
- The division-by-zero error was recorded in `logger/logs/error.log`.

## 4. Test Environment

The programs were compiled using GCC with `-Wall -Wextra`.
The UI, Core, and Logger were run as separate processes.

## 5. Conclusion

The listed functional operations and IPC behaviours were
manually tested. The UI and Core exchanged commands and
responses, and the Logger recorded execution and error messages.
