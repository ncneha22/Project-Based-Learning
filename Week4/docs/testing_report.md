# Testing Report — Interactive Deadlock Simulation System

## 1. Purpose

This report documents the testing status of the Interactive Deadlock Simulation System, including resource allocation, Banker's Algorithm, resource requests, deadlock detection, recovery, and resource utilization analysis.

## 2. Test Environment

- **Application:** Interactive Deadlock Simulation System
- **Interface:** HTML and CSS
- **Logic:** JavaScript
- **Execution:** Local web browser using Python's HTTP server
- **Unit Test Command:** `cd Week4 && node tests/bankers_test.js`
- **Banker's Algorithm Unit Tests:** 18/18 passed, as previously recorded.

## 3. Recorded Local Test Scenario

The following scenario was previously tested locally.

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
- System status reported a safe state with five processes and three resource types.
- Recovery reported that no deadlocked process was available to recover because the system was not deadlocked.

## 4. Additional Test Plan and Current Status

| Test | Expected Result | Status |
|---|---|---|
| Page loading | Main webpage loads correctly | Pending final verification |
| Process setup | Processes can be created and displayed | Pending final verification |
| Resource setup | Resource types and totals are displayed correctly | Pending final verification |
| Matrix input | Allocation and maximum matrices are entered correctly | Pending final verification |
| Need Matrix | Need = Maximum − Allocation | Pending final verification |
| Safe state | Banker's Algorithm returns the expected safe sequence | Previously recorded |
| Unsafe state | Available [1, 0, 0] is reported as unsafe | Pending verification |
| Granted request | P1 requests [1, 0, 2]; available becomes [2, 3, 0] | Previously recorded as granted; available value pending verification |
| Unsafe request | P0 requests [0, 2, 0]; request is denied and recorded as waiting | Pending verification |
| Resource Allocation Graph | Graph displays process-resource relationships and highlights a deadlock cycle in red | Pending verification |
| Deadlock detection | P0 and P1 are identified in the specified deadlock scenario | Pending verification |
| Deadlock recovery | One process is terminated, the other request is granted, and deadlock is resolved | Pending verification |
| Resource utilization | Allocation totals, available resources, total resources, utilization percentages, waiting processes, and deadlocked processes are displayed | Implementation added; browser verification pending |
| System status | Status includes the resource utilization summary | Implementation added; browser verification pending |

## 5. Resource Utilization Verification

For the recorded safe scenario, the expected resource utilization values are:

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

These are expected values calculated from the recorded matrices. The utilization panel still requires browser verification.

## 6. Banker's Algorithm Unit Tests

The following command is used to execute the Banker's Algorithm unit tests:

`cd Week4 && node tests/bankers_test.js`

Previously recorded result: **18/18 tests passed.**

This result applies to the Banker's Algorithm unit tests and does not confirm that every integrated GUI feature has passed testing.

## 7. Conclusion

The recorded local scenario demonstrated a safe resource-allocation state with the expected safe sequence. The resource utilization panel has been added to the interface code, and the JavaScript syntax check passed.

Additional scenarios involving unsafe states, waiting requests, deadlock detection, recovery, and resource graph visualization remain pending verification after final integration. Browser testing is required before these cases can be marked as passed.
