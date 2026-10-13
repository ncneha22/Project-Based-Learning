
#include <stdio.h>
#include "process.h"

#define MAX_PROCESSES 100

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
        printf("7. New Test Case\n");
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

        if (choice == 7) {
            new_test_case = 1;
            continue;
        }

        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please try again.\n");
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
