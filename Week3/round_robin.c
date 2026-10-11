#include <stdlib.h>
#include "process.h"

void round_robin(Process p[], int n, int quantum, ScheduleResult *result)
{
    if (p == NULL || result == NULL || n <= 0 || quantum <= 0)
        return;

    int *queue = malloc(n * sizeof(int));
    int *order = malloc(n * sizeof(int));

    if (queue == NULL || order == NULL)
    {
        free(queue);
        free(order);
        return;
    }

    result->segment_count = 0;

    for (int i = 0; i < n; i++)
    {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].turnaround_time = 0;
        p[i].waiting_time = 0;
        p[i].response_time = -1;
        p[i].start_time = -1;
        order[i] = i;
    }

    /* Sort process indices by arrival time. */
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (p[order[j]].arrival_time < p[order[i]].arrival_time)
            {
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    int front = 0, rear = 0, count = 0;
    int next = 0, completed = 0, time = 0;

    while (completed < n)
    {
        /* Enqueue processes that have arrived. */
        while (next < n && p[order[next]].arrival_time <= time)
        {
            int idx = order[next++];
            if (p[idx].burst_time <= 0)
            {
                p[idx].completion_time = p[idx].arrival_time;
                p[idx].turnaround_time = 0;
                p[idx].waiting_time = 0;
                p[idx].response_time = 0;
                completed++;
            }
            else
            {
                queue[rear] = idx;
                rear = (rear + 1) % n;
                count++;
            }
        }

        if (count == 0)
        {
            if (next < n)
            {
                int idle_start = time;
                time = p[order[next]].arrival_time;

                if (result->segment_count < MAX_SEGMENTS)
                {
                    int s = result->segment_count++;
                    result->segments[s].pid = -1;
                    result->segments[s].start_time = idle_start;
                    result->segments[s].end_time = time;
                }
                continue;
            }
            break;
        }

        int idx = queue[front];
        front = (front + 1) % n;
        count--;

        if (p[idx].start_time == -1)
        {
            p[idx].start_time = time;
            p[idx].response_time = time - p[idx].arrival_time;
        }

        int start = time;
        int run = p[idx].remaining_time < quantum
                      ? p[idx].remaining_time : quantum;

        time += run;
        p[idx].remaining_time -= run;

        if (result->segment_count < MAX_SEGMENTS)
        {
            int s = result->segment_count++;
            result->segments[s].pid = p[idx].pid;
            result->segments[s].start_time = start;
            result->segments[s].end_time = time;
        }

        /* New arrivals join before the unfinished process is requeued. */
        while (next < n && p[order[next]].arrival_time <= time)
        {
            int new_idx = order[next++];
            if (p[new_idx].burst_time <= 0)
            {
                p[new_idx].completion_time = p[new_idx].arrival_time;
                p[new_idx].turnaround_time = 0;
                p[new_idx].waiting_time = 0;
                p[new_idx].response_time = 0;
                completed++;
            }
            else
            {
                queue[rear] = new_idx;
                rear = (rear + 1) % n;
                count++;
            }
        }

        if (p[idx].remaining_time == 0)
        {
            p[idx].completion_time = time;
            p[idx].turnaround_time = time - p[idx].arrival_time;
            p[idx].waiting_time =
                p[idx].turnaround_time - p[idx].burst_time;
            completed++;
        }
        else
        {
            queue[rear] = idx;
            rear = (rear + 1) % n;
            count++;
        }
    }

    free(queue);
    free(order);
}
