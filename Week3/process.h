
#ifndef PROCESS_H
#define PROCESS_H

#define MAX_SEGMENTS 1000

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

typedef struct {
    int pid;
    int start_time;
    int end_time;
} ExecutionSegment;

typedef struct {
    ExecutionSegment segments[MAX_SEGMENTS];
    int segment_count;
} ScheduleResult;

/* FCFS and SJF */
void fcfs(Process p[], int n, ScheduleResult *result);
void sjf_non_preemptive(Process p[], int n, ScheduleResult *result);
void sjf_preemptive(Process p[], int n, ScheduleResult *result);

/* Priority */
void priority_non_preemptive(Process p[], int n, ScheduleResult *result);
void priority_preemptive(Process p[], int n, ScheduleResult *result);

/* Round Robin */
void round_robin(Process p[], int n, int quantum, ScheduleResult *result);

/* Display functions */
void display_gantt_chart(ScheduleResult *result);
void display_results(Process p[], int n, ScheduleResult *result);

#endif
