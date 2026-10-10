#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;

    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int remaining_time;
    int start_time;
} Process;

/* FCFS and SJF */
void fcfs(Process p[], int n);
void sjf_non_preemptive(Process p[], int n);
void sjf_preemptive(Process p[], int n);

/* Priority Scheduling */
void priority_non_preemptive(Process p[], int n);
void priority_preemptive(Process p[], int n);

/* Round Robin */
void round_robin(Process p[], int n, int quantum);

#endif
