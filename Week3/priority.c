
#include <stdio.h>
#include "process.h"

/* Add a Gantt chart segment and combine adjacent segments
   belonging to the same process. */
static void add_priority_segment(ScheduleResult *result, int pid,
                                 int start, int end)
{
    ExecutionSegment *last;

    if (result == NULL || start >= end)
        return;

    if (result->segment_count > 0) {
        last = &result->segments[result->segment_count - 1];

        if (last->pid == pid && last->end_time == start) {
            last->end_time = end;
            return;
        }
    }

    if (result->segment_count >= MAX_SEGMENTS)
        return;

    result->segments[result->segment_count].pid = pid;
    result->segments[result->segment_count].start_time = start;
    result->segments[result->segment_count].end_time = end;
    result->segment_count++;
}

/* Reset process metrics before scheduling. */
static void reset_priority_processes(Process p[], int n,
                                     ScheduleResult *result)
{
    int i;

    result->segment_count = 0;

    for (i = 0; i < n; i++) {
        p[i].completion_time = 0;
        p[i].turnaround_time = 0;
        p[i].waiting_time = 0;
        p[i].response_time = -1;
        p[i].remaining_time = p[i].burst_time;
        p[i].start_time = -1;
    }
}

/* Calculate metrics after a process completes. */
static void calculate_priority_metrics(Process *p)
{
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->burst_time;
}

/* Non-preemptive priority scheduling.
   Smaller priority number means higher priority. */
void priority_non_preemptive(Process p[], int n, ScheduleResult *result)
{
    int completed = 0;
    int time = 0;
    int i;

    if (p == NULL || result == NULL || n <= 0)
        return;

    reset_priority_processes(p, n, result);

    while (completed < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (p[i].remaining_time <= 0 ||
                p[i].arrival_time > time)
                continue;

            if (selected == -1 ||
                p[i].priority < p[selected].priority ||
                (p[i].priority == p[selected].priority &&
                 (p[i].arrival_time < p[selected].arrival_time ||
                  (p[i].arrival_time == p[selected].arrival_time &&
                   p[i].pid < p[selected].pid)))) {
                selected = i;
            }
        }

        if (selected == -1) {
            add_priority_segment(result, -1, time, time + 1);
            time++;
            continue;
        }

        p[selected].start_time = time;
        p[selected].response_time = time - p[selected].arrival_time;

        add_priority_segment(result, p[selected].pid,
                             time, time + p[selected].remaining_time);

        time += p[selected].remaining_time;
        p[selected].remaining_time = 0;
        p[selected].completion_time = time;

        calculate_priority_metrics(&p[selected]);
        completed++;
    }
}

/* Preemptive priority scheduling.
   Re-evaluate the highest-priority ready process every time unit. */
void priority_preemptive(Process p[], int n, ScheduleResult *result)
{
    int completed = 0;
    int time = 0;
    int i;

    if (p == NULL || result == NULL || n <= 0)
        return;

    reset_priority_processes(p, n, result);

    while (completed < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (p[i].remaining_time <= 0 ||
                p[i].arrival_time > time)
                continue;

            if (selected == -1 ||
                p[i].priority < p[selected].priority ||
                (p[i].priority == p[selected].priority &&
                 (p[i].arrival_time < p[selected].arrival_time ||
                  (p[i].arrival_time == p[selected].arrival_time &&
                   p[i].pid < p[selected].pid)))) {
                selected = i;
            }
        }

        if (selected == -1) {
            add_priority_segment(result, -1, time, time + 1);
            time++;
            continue;
        }

        if (p[selected].start_time == -1) {
            p[selected].start_time = time;
            p[selected].response_time = time - p[selected].arrival_time;
        }

        add_priority_segment(result, p[selected].pid, time, time + 1);

        p[selected].remaining_time--;
        time++;

        if (p[selected].remaining_time == 0) {
            p[selected].completion_time = time;
            calculate_priority_metrics(&p[selected]);
            completed++;
        }
    }
}
