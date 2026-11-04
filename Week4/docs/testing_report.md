# Testing Report

## Project
Interactive Deadlock Simulation System

## Testing Environment
- Operating System: Ubuntu (WSL)
- Programming Language: C
- GUI Library: GTK 3
- Compiler: GCC

## Test Cases and Results

| No. | Test Case | Expected Result | Status |
|---|---|---|---|
| 1 | Load Classic Example | System matrices are loaded correctly | Passed |
| 2 | Run Banker's Algorithm | Safe state and safe sequence are displayed | Passed |
| 3 | Detect Deadlock | System safety status is displayed | Passed |
| 4 | View Resource Allocation Graph | Process and resource nodes with allocation arrows are displayed | Passed |
| 5 | Request Available Resources | Resources are granted when the request is valid and safe | Passed |
| 6 | Request Unavailable Resources | Process is shown as waiting with a pending request | Passed |
| 7 | Release Resources | Released resources are returned to the available pool | Passed |
| 8 | Recover Process | Process resources are released and pending requests are cleared | Passed |
| 9 | Invalid Process ID | An error message is displayed | Passed |
| 10 | Negative Resource Values | Invalid resource values are rejected | Passed |
| 11 | Excessive Resource Request | Invalid resource request is rejected | Passed |
| 12 | Reset System | Current system data and graph are cleared | Passed |

## Banker's Algorithm Result

The classic example produced the following safe sequence:

P1 -> P3 -> P4 -> P0 -> P2

The system was reported as safe, with no deadlock detected.

## Conclusion

The GTK desktop application was compiled and tested successfully. The main features, including resource allocation, Banker's Algorithm, deadlock detection, graph visualization, resource requests, release, recovery, and reset, worked as expected during testing.
