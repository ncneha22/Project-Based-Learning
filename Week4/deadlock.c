#include <stdio.h>
#include "deadlock.h"

/* Deadlock Detection Algorithm */
int detect_deadlock(SystemState *state, int deadlocked[])
{
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int i, j, found;
    int deadlock_count = 0;

    /* Initialize Work with Available resources */
    for (j = 0; j < state->resource_count; j++)
    {
        work[j] = state->available[j];
    }

    /* Initialize Finish array */
    for (i = 0; i < state->process_count; i++)
    {
        int allocated = 0;

        for (j = 0; j < state->resource_count; j++)
        {
            if (state->allocation[i][j] > 0)
            {
                allocated = 1;
                break;
            }
        }

        if (!state->active[i] || !allocated)
        {
            finish[i] = 1;
        }
    }

    /* Search for processes that can complete */
    do
    {
        found = 0;

        for (i = 0; i < state->process_count; i++)
        {
            if (finish[i] == 0)
            {
                for (j = 0; j < state->resource_count; j++)
                {
                    if (state->request[i][j] > work[j])
                    {
                        break;
                    }
                }

                if (j == state->resource_count)
                {
                    for (j = 0; j < state->resource_count; j++)
                    {
                        work[j] += state->allocation[i][j];
                    }

                    finish[i] = 1;
                    found = 1;
                }
            }
        }

    } while (found);

    /* Identify deadlocked processes */
    for (i = 0; i < state->process_count; i++)
    {
        deadlocked[i] = 0;

        if (finish[i] == 0)
        {
            deadlocked[i] = 1;
            deadlock_count++;
        }
    }

    return deadlock_count;
}
