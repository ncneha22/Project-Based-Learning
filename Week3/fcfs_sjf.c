
#include <stdio.h>
#include "process.h"

/* Add a segment to the Gantt chart.
   Combine adjacent segments belonging to the same process. */
static void add_segment(ScheduleResult *result, int pid,
                        int start, int end)
{
    if (result == NULL || start >= end)
        return;

    if (result->segment_count > 0) {
        ExecutionSegment *last =
            &result->segments[result->segment_count - 1];

        if (last->pid == pid && last->end_time == start) {
            last->end_time = end;
            return;
        }
    }

    if (result->segment_count >= MAX_SEGMENTS)
        return;

    ExecutionSegment *segment =
        &result->segments[result->segment_count++];

    segment->pid = pid;
    segment->start_time = start;
    segment->end_time = end;
}

/* Reset metrics before running an algorithm. */
static void reset_processes(Process p[], int n,
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

/* Calculate final metrics for a completed process. */
static void calculate_metrics(Process *p)
{
    p->turnaround_time =
        p->completion_time - p->arrival_time;

    p->waiting_time =
        p->turnaround_time - p->burst_time;
}

/* FCFS: execute processes in arrival order. */
void fcfs(Process p[], int n, ScheduleResult *result)
{
    int done[n > 0 ? n : 1];
    int completed = 0;
    int time = 0;
    int i;

    if (p == NULL || result == NULL || n <= 0)
        return;

    reset_processes(p, n, result);

    for (i = 0; i < n; i++)
        done[i] = 0;

    while (completed < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (done[i])
                continue;

            if (selected == -1 ||
                p[i].arrival_time < p[selected].arrival_time ||
                (p[i].arrival_time == p[selected].arrival_time &&
                 p[i].pid < p[selected].pid)) {
                selected = i;
            }
        }

        if (selected == -1)
            break;

        if (time < p[selected].arrival_time) {
            add_segment(result, -1, time,
                        p[selected].arrival_time);
            time = p[selected].arrival_time;
        }

        p[selected].start_time = time;
        p[selected].response_time =
            time - p[selected].arrival_time;

        add_segment(result, p[selected].pid,
                    time, time + p[selected].burst_time);

        time += p[selected].burst_time;
        p[selected].completion_time = time;

        calculate_metrics(&p[selected]);

        done[selected] = 1;
        completed++;
    }
}

/* SJF Non-Preemptive: choose the shortest arrived job.
   Once started, a process runs until completion. */
void sjf_non_preemptive(Process p[], int n,
                        ScheduleResult *result)
{
    int done[n > 0 ? n : 1];
    int completed = 0;
    int time = 0;
    int i;

    if (p == NULL || result == NULL || n <= 0)
        return;

    reset_processes(p, n, result);

    for (i = 0; i < n; i++)
        done[i] = 0;

    while (completed < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (done[i] || p[i].arrival_time > time)
                continue;

            if (selected == -1 ||
                p[i].burst_time < p[selected].burst_time ||
                (p[i].burst_time == p[selected].burst_time &&
                 p[i].arrival_time < p[selected].arrival_time) ||
                (p[i].burst_time == p[selected].burst_time &&
                 p[i].arrival_time == p[selected].arrival_time &&
                 p[i].pid < p[selected].pid)) {
                selected = i;
            }
        }

        /* If nothing has arrived, advance to the next arrival. */
        if (selected == -1) {
            int next_arrival = -1;

            for (i = 0; i < n; i++) {
                if (!done[i] &&
                    (next_arrival == -1 ||
                     p[i].arrival_time < next_arrival)) {
                    next_arrival = p[i].arrival_time;
                }
            }

            if (next_arrival == -1)
                break;

            add_segment(result, -1, time, next_arrival);
            time = next_arrival;
            continue;
        }

        p[selected].start_time = time;
        p[selected].response_time =
            time - p[selected].arrival_time;

        add_segment(result, p[selected].pid,
                    time, time + p[selected].burst_time);

        time += p[selected].burst_time;
        p[selected].completion_time = time;

        calculate_metrics(&p[selected]);

        done[selected] = 1;
        completed++;
    }
}

/* SJF Preemptive / SRTF:
   At every time unit, run the arrived process with the
   shortest remaining burst time. */
void sjf_preemptive(Process p[], int n,
                    ScheduleResult *result)
{
    int completed = 0;
    int time = 0;
    int i;

    if (p == NULL || result == NULL || n <= 0)
        return;

    reset_processes(p, n, result);

    while (completed < n) {
        int selected = -1;

        for (i = 0; i < n; i++) {
            if (p[i].remaining_time <= 0 ||
                p[i].arrival_time > time) {
                continue;
            }

            if (selected == -1 ||
                p[i].remaining_time <
                    p[selected].remaining_time ||
                (p[i].remaining_time ==
                    p[selected].remaining_time &&
                 p[i].arrival_time < p[selected].arrival_time) ||
                (p[i].remaining_time ==
                    p[selected].remaining_time &&
                 p[i].arrival_time == p[selected].arrival_time &&
                 p[i].pid < p[selected].pid)) {
                selected = i;
            }
        }

        /* No process is available: record CPU idle time. */
        if (selected == -1) {
            add_segment(result, -1, time, time + 1);
            time++;
            continue;
        }

        if (p[selected].start_time == -1) {
            p[selected].start_time = time;
            p[selected].response_time =
                time - p[selected].arrival_time;
        }

        add_segment(result, p[selected].pid, time, time + 1);

        p[selected].remaining_time--;
        time++;

        if (p[selected].remaining_time == 0) {
            p[selected].completion_time = time;

            calculate_metrics(&p[selected]);

            completed++;
        }
    }
}
