# Week 3 CPU Scheduling Simulator - Testing Report

## 1. Project Overview
The CPU Scheduling Simulator is implemented in C. It supports six scheduling algorithms and displays Gantt charts and scheduling performance metrics.

## 2. Algorithms Tested
1. First Come First Serve (FCFS)
2. Shortest Job First (SJF) Non-Preemptive
3. Shortest Remaining Time First (SRTF)
4. Priority Scheduling Non-Preemptive
5. Priority Scheduling Preemptive
6. Round Robin

## 3. Test Cases

### Test Case 1: All processes arrive at time 0
- P1: Arrival Time = 0, Burst Time = 5, Priority = 2
- P2: Arrival Time = 0, Burst Time = 3, Priority = 1
- P3: Arrival Time = 0, Burst Time = 1, Priority = 3

### Test Case 2: Processes arrive at different times
- P1: Arrival Time = 0, Burst Time = 4, Priority = 2
- P2: Arrival Time = 2, Burst Time = 3, Priority = 1
- P3: Arrival Time = 4, Burst Time = 2, Priority = 3

### Test Case 3: CPU idle time
- P1: Arrival Time = 0, Burst Time = 3, Priority = 2
- P2: Arrival Time = 5, Burst Time = 2, Priority = 1

## 4. Testing Results
All six algorithms were manually tested with all three test cases, making 18 test runs. The displayed scheduling results were checked against expected calculations.

- Test Case 1: CPU utilization = 100%; idle time = 0.
- Test Case 2: CPU utilization = 100%; idle time = 0.
- Test Case 3: CPU utilization = 71.43%; idle time = 2 time units.

Round Robin was tested with a time quantum of 2 for these test cases.

## 5. Performance Metrics Checked
- Completion Time (CT)
- Turnaround Time (TAT)
- Waiting Time (WT)
- Response Time (RT)
- Average TAT, WT and RT
- CPU utilization and idle time

## 6. Additional Test Program
The program in tests/member2_test.c was compiled and executed. Its FCFS, SJF Non-Preemptive and SRTF results were manually checked.

## 7. Conclusion
The simulator compiled and ran successfully. The results from the documented manual tests matched the expected calculations. The tests described here were manually verified; they are not an automated PASS/FAIL test suite.
