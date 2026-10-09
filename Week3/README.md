# Week 3 — Process Scheduling Simulator

## Project Overview
A CPU Process Scheduling Simulator implemented in C to simulate scheduling algorithms and analyze their performance.

## Scheduling Algorithms
- First Come First Serve (FCFS)
- Shortest Job First (SJF) — Non-Preemptive and Preemptive
- Priority Scheduling — Non-Preemptive and Preemptive
- Round Robin — Configurable Time Quantum

## Performance Metrics
- Completion Time (CT)
- Turnaround Time (TAT)
- Waiting Time (WT)
- Response Time (RT)
- Average Waiting Time
- Average Turnaround Time

## Project Structure
- `main.c` — Main program and integration
- `process.h` — Shared process structure and declarations
- `fcfs_sjf.c` — FCFS and SJF algorithms
- `priority.c` — Priority scheduling algorithms
- `round_robin.c` — Round Robin algorithm
- `tests/` — Test cases and sample outputs
- `docs/` — Project report and performance analysis

## Team Work
Each team member will implement their assigned scheduling algorithms. The team lead will integrate the modules, test the complete program, and coordinate the final documentation.

## Language
C
