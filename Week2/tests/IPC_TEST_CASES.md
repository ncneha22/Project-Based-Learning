# IPC Test Cases

## Objective
To verify the communication between the UI, Core, and Logger processes and confirm that the simulator operations work correctly.

## Test Cases

| Test Case | Operation | Input | Expected Result | Actual Result | Status |
|---|---|---|---|---|---|
| TC01 | ADD | 10, 20 | CPU: 30 | CPU: 30 | PASS |
| TC02 | SUBTRACT | 20, 5 | CPU: 15 | CPU: 15 | PASS |
| TC03 | MULTIPLY | 5, 4 | CPU: 20 | CPU: 20 | PASS |
| TC04 | DIVIDE | 20, 5 | CPU: 4 | CPU: 4 | PASS |
| TC05 | DIVIDE BY ZERO | 20, 0 | Error message | ERROR: Division by zero | PASS |
| TC06 | STORE | Address 10, Value 50 | Value stored | Value stored successfully | PASS |
| TC07 | LOAD | Address 10 | Value 50 | Memory: address 10 = 50 | PASS |
| TC08 | PUSH | 100/200/500 | Value pushed | Value pushed successfully | PASS |
| TC09 | POP | — | Top value removed | Value popped successfully | PASS |
| TC10 | PEEK | — | Top value displayed | Top value displayed | PASS |
| TC11 | ENQUEUE | 300 | Value added | Queue: added 300 | PASS |
| TC12 | DEQUEUE | — | Front value removed | Removed 300 | PASS |
| TC13 | QPEEK | Empty queue | Error message | ERROR: Queue is empty | PASS |
| TC14 | EXIT | — | Processes terminate | Core stopped and Logger received message | PASS |

## IPC Verification

- UI sends commands to Core using POSIX Message Queues.
- Core sends responses back to UI using POSIX Message Queues.
- Core sends log messages to Logger using a Named FIFO.
- Logger receives messages and records them in log files.
- All tested operations successfully communicated between the processes.

## Conclusion

All functional test cases were successfully completed. The UI, Core, and Logger processes communicated correctly using the selected IPC mechanisms.
