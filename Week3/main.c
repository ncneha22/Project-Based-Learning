
#include <stdio.h>
#include "process.h"

#define MAX_PROCESSES 100

typedef void (*Scheduler)(Process[], int, ScheduleResult *);

static void copy_processes(Process dest[], Process src[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        dest[i] = src[i];
        dest[i].remaining_time = src[i].burst_time;
        dest[i].completion_time = 0;
        dest[i].turnaround_time = 0;
        dest[i].waiting_time = 0;
        dest[i].response_time = -1;
        dest[i].start_time = -1;
    }
}

static int enter_processes(Process p[], int *n)
{
    int i;

    printf("\nEnter number of processes (1-100): ");

    if (scanf("%d", n) != 1 || *n < 1 || *n > MAX_PROCESSES) {
        printf("Invalid number of processes!\n");
        return 0;
    }

    for (i = 0; i < *n; i++) {
        printf("\nProcess %d\n", i + 1);

        printf("Enter PID: ");
        if (scanf("%d", &p[i].pid) != 1)
            return 0;

        printf("Enter Arrival Time: ");
        if (scanf("%d", &p[i].arrival_time) != 1)
            return 0;

        printf("Enter Burst Time: ");
        if (scanf("%d", &p[i].burst_time) != 1)
            return 0;

        printf("Enter Priority: ");
        if (scanf("%d", &p[i].priority) != 1)
            return 0;

        if (p[i].arrival_time < 0 || p[i].burst_time <= 0) {
            printf("Arrival time must be >= 0 and burst time > 0.\n");
            return 0;
        }
    }

    return 1;
}

static void compare_all(Process original[], int n, int quantum)
{
    Process working[MAX_PROCESSES];
    ScheduleResult result;

    Scheduler algorithms[] = {
        fcfs,
        sjf_non_preemptive,
        sjf_preemptive,
        priority_non_preemptive,
        priority_preemptive
    };

    const char *names[] = {
        "FCFS",
        "SJF Non-Preemptive",
        "SJF Preemptive (SRTF)",
        "Priority Non-Preemptive",
        "Priority Preemptive"
    };

    int i, j;
    int total_burst, total_elapsed;
    float avg_wt, avg_tat, avg_rt, cpu_util;

    printf("\n========== ALGORITHM COMPARISON ==========\n");
    printf("Round Robin Time Quantum: %d\n\n", quantum);

    printf("%-27s %10s %10s %10s %12s\n",
           "Algorithm", "Avg WT", "Avg TAT", "Avg RT", "CPU Util %");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < 6; i++) {
        copy_processes(working, original, n);
        result.segment_count = 0;

        if (i < 5)
            algorithms[i](working, n, &result);
        else
            round_robin(working, n, quantum, &result);

        total_burst = 0;
        avg_wt = 0;
        avg_tat = 0;
        avg_rt = 0;

        for (j = 0; j < n; j++) {
            total_burst += working[j].burst_time;
            avg_wt += working[j].waiting_time;
            avg_tat += working[j].turnaround_time;
            avg_rt += working[j].response_time;
        }

        avg_wt /= n;
        avg_tat /= n;
        avg_rt /= n;

        total_elapsed = 0;
        cpu_util = 0;

        if (result.segment_count > 0) {
            total_elapsed =
                result.segments[result.segment_count - 1].end_time;

            if (total_elapsed > 0)
                cpu_util = ((float)total_burst / total_elapsed) * 100.0f;
        }

        if (i < 5) {
            printf("%-27s %10.2f %10.2f %10.2f %11.2f%%\n",
                   names[i], avg_wt, avg_tat, avg_rt, cpu_util);
        } else {
            printf("%-27s %10.2f %10.2f %10.2f %11.2f%%\n",
                   "Round Robin", avg_wt, avg_tat, avg_rt, cpu_util);
        }
    }

    printf("--------------------------------------------------------------------------\n");
}

int main(void)
{
    Process original[MAX_PROCESSES];
    Process working[MAX_PROCESSES];
    ScheduleResult result;

    int n, choice, quantum;
    int new_test_case = 1;
    int exit_program = 0;

    printf("CPU SCHEDULING SIMULATOR\n");
    printf("=======================\n");

    while (!exit_program) {
        if (new_test_case) {
            printf("\n========== NEW TEST CASE ==========\n");

            if (!enter_processes(original, &n)) {
                printf("Invalid input. Please restart the program.\n");
                return 1;
            }

            new_test_case = 0;
        }

        printf("\n========== SCHEDULING MENU ==========\n");
        printf("1. FCFS\n");
        printf("2. SJF Non-Preemptive\n");
        printf("3. SJF Preemptive (SRTF)\n");
        printf("4. Priority Non-Preemptive\n");
        printf("5. Priority Preemptive\n");
        printf("6. Round Robin\n");
        printf("7. Compare All Algorithms\n");
        printf("8. New Test Case\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        if (choice == 0) {
            printf("Exiting simulator.\n");
            exit_program = 1;
            continue;
        }

        if (choice == 8) {
            new_test_case = 1;
            continue;
        }

        if (choice < 1 || choice > 8) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        if (choice == 7) {
            printf("Enter time quantum for Round Robin comparison: ");

            if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                printf("Invalid time quantum!\n");
                continue;
            }

            compare_all(original, n, quantum);

            /* Automatically request a new test case after comparison. */
            new_test_case = 1;
            continue;
        }

        quantum = 0;

        if (choice == 6) {
            printf("Enter time quantum: ");

            if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                printf("Invalid time quantum!\n");
                continue;
            }
        }

        copy_processes(working, original, n);
        result.segment_count = 0;

        switch (choice) {
            case 1:
                printf("\nFCFS SCHEDULING\n");
                fcfs(working, n, &result);
                break;

            case 2:
                printf("\nSJF NON-PREEMPTIVE SCHEDULING\n");
                sjf_non_preemptive(working, n, &result);
                break;

            case 3:
                printf("\nSJF PREEMPTIVE (SRTF) SCHEDULING\n");
                sjf_preemptive(working, n, &result);
                break;

            case 4:
                printf("\nPRIORITY NON-PREEMPTIVE SCHEDULING\n");
                priority_non_preemptive(working, n, &result);
                break;

            case 5:
                printf("\nPRIORITY PREEMPTIVE SCHEDULING\n");
                priority_preemptive(working, n, &result);
                break;

            case 6:
                printf("\nROUND ROBIN SCHEDULING\n");
                round_robin(working, n, quantum, &result);
                break;
        }

        display_gantt_chart(&result);
        display_results(working, n, &result);
    }

    return 0;
}
