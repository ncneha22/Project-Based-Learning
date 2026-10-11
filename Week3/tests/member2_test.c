
#include <stdio.h>
#include "../process.h"

void fcfs(Process p[], int n, ScheduleResult *result);
void sjf_non_preemptive(Process p[], int n, ScheduleResult *result);
void sjf_preemptive(Process p[], int n, ScheduleResult *result);

void display_gantt_chart(ScheduleResult *result);
void display_results(Process p[], int n, ScheduleResult *result);

static void run_test(
    const char *name,
    void (*algorithm)(Process[], int, ScheduleResult *))
{
    Process p[] = {
        {.pid = 1, .arrival_time = 0, .burst_time = 7, .priority = 2},
        {.pid = 2, .arrival_time = 2, .burst_time = 4, .priority = 1},
        {.pid = 3, .arrival_time = 4, .burst_time = 1, .priority = 3}
    };

    int n = (int)(sizeof(p) / sizeof(p[0]));
    ScheduleResult result = {0};

    printf("\n========== %s ==========\n", name);

    algorithm(p, n, &result);
    display_gantt_chart(&result);
    display_results(p, n, &result);
}

int main(void)
{
    run_test("FCFS", fcfs);
    run_test("SJF Non-Preemptive", sjf_non_preemptive);
    run_test("SJF Preemptive (SRTF)", sjf_preemptive);

    return 0;
}
