# Week 3 CPU Scheduling Simulator - Testing Report

## 1. Project Overview

The CPU Scheduling Simulator is implemented in C. It demonstrates six CPU scheduling algorithms and displays Gantt charts, process completion times, turnaround times, waiting times, response times, average metrics, CPU utilization, and idle time.

## 2. Algorithms Tested

1. First Come First Serve (FCFS)
2. Shortest Job First (SJF) Non-Preemptive
3. Shortest Remaining Time First (SRTF)
4. Priority Scheduling Non-Preemptive
5. Priority Scheduling Preemptive
6. Round Robin (RR), with time quantum = 2

## 3. Test Cases

### Test Case 1: All Processes Arrive at Time 0

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 5 | 2 |
| P2 | 0 | 3 | 1 |
| P3 | 0 | 1 | 3 |

### Test Case 2: Processes Arrive at Different Times

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 4 | 2 |
| P2 | 2 | 3 | 1 |
| P3 | 4 | 2 | 3 |

### Test Case 3: CPU Idle Time

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 3 | 2 |
| P2 | 5 | 2 | 1 |

## 4. Testing Results

All six scheduling algorithms were manually tested with all three test cases, giving 18 algorithm-test combinations. The schedules and calculated metrics were checked against expected results.

### Test Case 1 Results

| Algorithm | Gantt Chart Schedule | Avg. TAT | Avg. WT | Avg. RT |
|---|---|---:|---:|---:|
| FCFS | P1 (0-5), P2 (5-8), P3 (8-9) | 7.33 | 4.33 | 4.33 |
| SJF Non-Preemptive | P3 (0-1), P2 (1-4), P1 (4-9) | 4.67 | 1.67 | 1.67 |
| SRTF | P3 (0-1), P2 (1-4), P1 (4-9) | 4.67 | 1.67 | 1.67 |
| Priority Non-Preemptive | P2 (0-3), P1 (3-8), P3 (8-9) | 6.67 | 3.67 | 3.67 |
| Priority Preemptive | P2 (0-3), P1 (3-8), P3 (8-9) | 6.67 | 3.67 | 3.67 |
| Round Robin | P1 (0-2), P2 (2-4), P3 (4-5), P1 (5-7), P2 (7-8), P1 (8-9) | 7.33 | 4.33 | 2.00 |

CPU utilization: 100%. Idle time: 0.

### Test Case 2 Results

| Algorithm | Gantt Chart Schedule | Avg. TAT | Avg. WT | Avg. RT |
|---|---|---:|---:|---:|
| FCFS | P1 (0-4), P2 (4-7), P3 (7-9) | 4.67 | 1.67 | 1.67 |
| SJF Non-Preemptive | P1 (0-4), P3 (4-6), P2 (6-9) | 4.33 | 1.33 | 1.33 |
| SRTF | P1 (0-4), P3 (4-6), P2 (6-9) | 4.33 | 1.33 | 1.33 |
| Priority Non-Preemptive | P1 (0-4), P2 (4-7), P3 (7-9) | 4.67 | 1.67 | 1.67 |
| Priority Preemptive | P1 (0-2), P2 (2-5), P1 (5-7), P3 (7-9) | 5.00 | 2.00 | 1.00 |
| Round Robin | P1 (0-2), P2 (2-4), P1 (4-6), P3 (6-8), P2 (8-9) | 5.67 | 2.67 | 0.67 |

CPU utilization: 100%. Idle time: 0.

### Test Case 3 Results

| Algorithm | Gantt Chart Schedule | Avg. TAT | Avg. WT | Avg. RT |
|---|---|---:|---:|---:|
| FCFS | P1 (0-3), IDLE (3-5), P2 (5-7) | 2.50 | 0.00 | 0.00 |
| SJF Non-Preemptive | P1 (0-3), IDLE (3-5), P2 (5-7) | 2.50 | 0.00 | 0.00 |
| SRTF | P1 (0-3), IDLE (3-5), P2 (5-7) | 2.50 | 0.00 | 0.00 |
| Priority Non-Preemptive | P1 (0-3), IDLE (3-5), P2 (5-7) | 2.50 | 0.00 | 0.00 |
| Priority Preemptive | P1 (0-3), IDLE (3-5), P2 (5-7) | 2.50 | 0.00 | 0.00 |
| Round Robin | P1 (0-2), P1 (2-3), IDLE (3-5), P2 (5-7) | 2.50 | 0.00 | 0.00 |

CPU utilization: 71.43%. Idle time: 2 time units.

## 5. Performance Metrics Checked

The following metrics were checked:

- Completion Time (CT)
- Turnaround Time (TAT)
- Waiting Time (WT)
- Response Time (RT)
- Average TAT, WT, and RT
- CPU utilization and idle time

### Metric Definitions

- TAT = Completion Time - Arrival Time
- WT = Turnaround Time - Burst Time
- RT = First Start Time - Arrival Time
- CPU Utilization = (Busy Time / Total Elapsed Time) x 100

## 6. Additional Test Program

The additional test program is located at `tests/member2_test.c`. Its compilation and execution should be verified separately before claiming that it passed.

## 7. Conclusion

The CPU Scheduling Simulator compiled successfully using GCC with the `-Wall` and `-Wextra` warning options. The six scheduling algorithms were manually tested using three test cases covering simultaneous arrivals, different arrival times, and CPU idle time. The observed schedules and performance metrics were checked against expected results. The screenshots in the `screenshots` directory provide visual evidence of selected test runs.
