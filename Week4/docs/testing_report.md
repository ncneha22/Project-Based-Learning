# Testing Report — Interactive Deadlock Simulation System

## 1. Purpose
This report records checks for the process and resource interface, safety analysis, resource requests, deadlock detection, recovery, and system status.

## 2. Test Environment
- Application: Interactive Deadlock Simulation System
- Interface: HTML and CSS
- Logic: JavaScript
- Execution: Local web browser using Python's HTTP server

## 3. Recorded Local Test Scenario
The following scenario was previously tested locally.

Total resources:
- R0 = 10
- R1 = 5
- R2 = 7

Allocation Matrix:

| Process | R0 | R1 | R2 |
|---|---:|---:|---:|
| P0 | 0 | 1 | 0 |
| P1 | 2 | 0 | 0 |
| P2 | 3 | 0 | 2 |
| P3 | 2 | 1 | 1 |
| P4 | 0 | 0 | 2 |

Maximum Matrix:

| Process | R0 | R1 | R2 |
|---|---:|---:|---:|
| P0 | 7 | 5 | 3 |
| P1 | 3 | 2 | 2 |
| P2 | 9 | 0 | 2 |
| P3 | 2 | 2 | 2 |
| P4 | 4 | 3 | 3 |

The expected Need Matrix is calculated as Maximum minus Allocation. The resulting Available Vector is [3, 3, 2].

Recorded results:
- Banker's Algorithm reported a safe state.
- Safe sequence: P1 → P3 → P4 → P0 → P2.
- Deadlock detection reported no deadlocked processes.
- A request by P1 for [1, 0, 2] was granted.
- System status reported a safe state with five processes and three resource types.
- Recovery reported that no deadlocked process was available to recover, and instructed the user to run detection first.

## 4. Additional Test Plan

| Test | Check | Result |
|---|---|---|
| Page loading | Open the main webpage | Verify after final integration |
| Process setup | Create and display processes | Verify after final integration |
| Resource setup | Create resource types and totals | Verify after final integration |
| Matrix input | Enter allocation and maximum values | Verify after final integration |
| Need Matrix | Confirm Maximum minus Allocation | Verify after final integration |
| Safety check | Run Banker's Algorithm | Safe scenario recorded above |
| Resource graph | Display process-resource relationships | Verify after final integration |
| Resource request | Check valid, unavailable, and unsafe requests | One granted request recorded above |
| Deadlock detection | Run detection and display results | No-deadlock scenario recorded above |
| Recovery | Attempt recovery after detection | No-deadlock response recorded above |
| System status | Display current system state | Safe status recorded above |

## 5. Conclusion
The recorded local scenario demonstrated a safe resource-allocation state and a successful resource request. The remaining test-plan items must be rechecked after the final GitHub version is synchronized and tested.
