# Testing Report — Interactive Deadlock Simulation System

## 1. Purpose

This report documents the testing status of the Interactive Deadlock Simulation System, including resource allocation, Banker's Algorithm, resource requests, deadlock detection, recovery, Resource Allocation Graph visualization, resource utilization analysis, and simulation reset functionality.

## 2. Test Environment

- **Application:** Interactive Deadlock Simulation System
- **Interface:** HTML and CSS
- **Logic:** JavaScript
- **Graph Visualization:** SVG
- **Execution:** Local web browser using Python's HTTP server
- **Unit Test Command:** `node tests/bankers_test.js`
- **Banker's Algorithm Unit Tests:** 18/18 passed, as previously recorded.

## 3. Recorded Local Test Scenario

The following safe-state scenario was previously recorded.

**Total Resources:** R0 = 10, R1 = 5, R2 = 7

### Allocation Matrix

| Process | R0 | R1 | R2 |
|---|---:|---:|---:|
| P0 | 0 | 1 | 0 |
| P1 | 2 | 0 | 0 |
| P2 | 3 | 0 | 2 |
| P3 | 2 | 1 | 1 |
| P4 | 0 | 0 | 2 |

### Maximum Matrix

| Process | R0 | R1 | R2 |
|---|---:|---:|---:|
| P0 | 7 | 5 | 3 |
| P1 | 3 | 2 | 2 |
| P2 | 9 | 0 | 2 |
| P3 | 2 | 2 | 2 |
| P4 | 4 | 3 | 3 |

### Expected Results

- Available resources: [3, 3, 2]
- Need Matrix: Maximum − Allocation
- Banker's Algorithm: SAFE
- Safe sequence: P1 → P3 → P4 → P0 → P2
- Deadlock detection: No deadlocked processes

### Previously Recorded Results

- Banker's Algorithm reported a safe state.
- The safe sequence was P1 → P3 → P4 → P0 → P2.
- Deadlock detection reported no deadlocked processes.
- A request by P1 for [1, 0, 2] was recorded as granted.
- Recovery reported that no deadlocked process was available to recover because the system was not deadlocked.
- The request result was recorded previously; its final available-resource value still requires verification.

## 4. Additional Test Plan and Current Status

| Test | Expected Result | Status |
|---|---|---|
| Page loading | Main webpage loads correctly | Pending final verification |
| Process setup | Processes can be created and displayed | Pending final verification |
| Resource setup | Resource types and totals are displayed correctly | Pending final verification |
| Matrix input | Allocation and maximum matrices are entered correctly | Pending final verification |
| Need Matrix | Need = Maximum − Allocation | Pending final verification |
| Safe state | Banker's Algorithm returns the expected safe sequence | Previously recorded |
| Unsafe state | An unsafe configuration is correctly reported as unsafe | Pending verification |
| Granted request | A valid request is granted when the resulting state is safe | Previously recorded; available-resource value pending verification |
| Unsafe request | An unsafe request is denied or marked as waiting, according to the implementation | Pending verification |
| Resource Allocation Graph | Process and resource nodes, allocation arrows, request arrows, and legend are displayed | Verified in the tested scenario |
| Deadlock detection | P0 and P1 are identified in the specified deadlock scenario | Passed for the tested scenario |
| Waiting-chain output | The waiting relationships between P0 and P1 are displayed | Passed for the tested scenario |
| Deadlock highlighting | Deadlocked processes and relevant graph edges are highlighted in red | Passed for the tested scenario |
| Graph refresh | An already displayed graph refreshes after deadlock detection | Tested as part of the deadlock scenario |
| Deadlock recovery | Recovery resolves a deadlock and updates the system state correctly | Pending verification |
| Resource utilization | Allocation totals, available resources, total resources, utilization percentages, waiting processes, and deadlocked processes are displayed correctly | Pending browser verification |
| System status | Status is updated consistently after simulation operations | Pending verification |
| Reset Simulation | Confirmation popup appears; clicking OK reloads the initial state | Popup and reset tested successfully |
| Reset cancellation | Clicking Cancel closes the popup and preserves the current simulation | Pending verification |

## 5. Deadlock Detection and Graph Visualization Test

### Test Configuration

**Number of processes:** 2

**Number of resource types:** 2

**Total resources:** R0 = 1, R1 = 1

### Allocation Matrix

| Process | R0 | R1 |
|---|---:|---:|
| P0 | 1 | 0 |
| P1 | 0 | 1 |

### Maximum Matrix

| Process | R0 | R1 |
|---|---:|---:|
| P0 | 1 | 1 |
| P1 | 1 | 1 |

### Outstanding Request Matrix

| Process | R0 | R1 |
|---|---:|---:|
| P0 | 0 | 1 |
| P1 | 1 | 0 |

### Expected Results

- P0 holds R0 and waits for R1.
- P1 holds R1 and waits for R0.
- Neither outstanding request can be satisfied with the currently available resources.
- Deadlock detection identifies P0 and P1.
- The waiting chain identifies P0 waiting for R1 held by P1, and P1 waiting for R0 held by P0.
- The Resource Allocation Graph highlights the deadlocked processes and relevant edges in red.

### Observed Results

- The application displayed both processes as deadlocked.
- The waiting-chain message showed: P0 waits for R1 held by P1; P1 waits for R0 held by P0.
- The graph displayed process circles and resource rectangles.
- Allocation arrows and dashed request arrows were displayed with labels.
- P0 and P1 and the relevant edges were highlighted in red.
- The graph displayed a legend explaining the visual elements.

**Test status:** Passed for this two-process, two-resource scenario.

## 6. Resource Utilization Verification

For the previously recorded safe scenario, the expected resource utilization values are:

| Resource | Allocated | Available | Total | Utilization |
|---|---:|---:|---:|---:|
| R0 | 7 | 3 | 10 | 70.0% |
| R1 | 2 | 3 | 5 | 40.0% |
| R2 | 5 | 2 | 7 | 71.4% |

- Total allocated instances: 14
- Total resource instances: 22
- Overall utilization: 63.6%
- Waiting processes: 0, provided no pending requests exist
- Deadlocked processes: 0 in the recorded safe scenario

These values are calculated from the recorded matrices. The utilization panel requires separate browser verification before it can be marked as passed.

## 7. Banker's Algorithm Unit Tests

The following command was previously recorded for executing the Banker's Algorithm unit tests:

`node tests/bankers_test.js`

Previously recorded result: **18/18 tests passed.**

This result applies to the Banker's Algorithm unit tests only. It does not confirm that every integrated GUI feature has passed testing.

## 8. Reset Simulation Test

### Test Procedure

1. Open the Deadlock Simulation System in the browser.
2. Enter the required simulation data.
3. Click the Reset Simulation button.
4. Check whether a confirmation popup appears.
5. Click OK and verify that the page reloads to the initial simulation state.

### Expected Results

- A confirmation popup appears when Reset Simulation is clicked.
- Clicking OK reloads the application and returns it to the initial state.
- Clicking Cancel should close the popup without resetting the current simulation.

### Observed Results

- The confirmation popup appeared.
- After confirmation, the simulation reset successfully.

**Test status:** Popup and reset behavior tested successfully. Cancel behavior requires separate verification.

## 9. Conclusion

The previously recorded safe-state scenario demonstrated a safe resource-allocation state with the expected safe sequence.

The updated deadlock detection and Resource Allocation Graph features were tested using a two-process, two-resource scenario. The application identified P0 and P1 as deadlocked, displayed the waiting chain, and highlighted the deadlocked processes and relevant graph edges in red.

The Reset Simulation button was also tested. Its confirmation popup appeared, and confirming the reset returned the application to its initial state.

Other scenarios involving unsafe states, resource requests, recovery, resource utilization, reset cancellation, and complete application integration require further verification. Test results should be marked as passed only after the corresponding behavior has been checked.
