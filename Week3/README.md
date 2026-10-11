# Week 3 — Process Scheduling Simulator

## Project Overview
A CPU Process Scheduling Simulator implemented in C to simulate operating system process scheduling algorithms. The simulator accepts process details from the user and displays the scheduling order, Gantt chart, and performance metrics.

## Scheduling Algorithms
- First Come First Serve (FCFS)
- Shortest Job First (SJF) — Non-Preemptive
- Shortest Remaining Time First (SRTF) — Preemptive SJF
- Priority Scheduling — Non-Preemptive
- Priority Scheduling — Preemptive
- Round Robin — Configurable Time Quantum

## Performance Metrics
- Completion Time (CT)
- Turnaround Time (TAT)
- Waiting Time (WT)
- Response Time (RT)
- Average Turnaround Time
- Average Waiting Time
- Average Response Time
- CPU Utilization
- CPU Idle Time

## Project Structure
- `main.c` — Main program and integration
- `process.h` — Shared process structure and function declarations
- `fcfs_sjf.c` — FCFS and SJF scheduling algorithms
- `priority.c` — Priority scheduling algorithms
- `round_robin.c` — Round Robin scheduling algorithm
- `display.c` — Gantt chart and performance metrics display
- `tests/` — Test cases and testing files
- `docs/` — Project documentation and performance analysis

## Testing
Three test cases are provided:
- Test Case 1: All processes arrive at time 0.
- Test Case 2: Processes arrive at different times.
- Test Case 3: A process arrives after the CPU becomes idle.

All six scheduling algorithms have been manually tested using these three test cases. The results were checked against expected calculations. Detailed results are documented in docs/testing_report.md.

## Compilation
Compile the complete simulator after all six algorithm implementations are available:

```bash
gcc -Wall -Wextra main.c fcfs_sjf.c priority.c round_robin.c display.c -o scheduler
```

Run the simulator using:

```bash
./scheduler
```

## Team Work
Each team member implements their assigned components. The team lead integrates the components, tests the complete simulator, and verifies the scheduling results.

## Language
C
