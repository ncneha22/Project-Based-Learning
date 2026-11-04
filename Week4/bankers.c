#include <stdio.h>
#include "deadlock.h"

/* Calculate Need Matrix */
void calculate_need(SystemState *state)
{
    int i, j;

    for (i = 0; i < state->process_count; i++)
    {
        for (j = 0; j < state->resource_count; j++)
        {
            state->need[i][j] =
                state->maximum[i][j] -
                state->allocation[i][j];
        }
    }
}

/* Check whether the system is in a safe state */
int banker_is_safe(SystemState *state,
                   int safe_sequence[],
                   int *sequence_length)
{
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int i, j, found;
    int count = 0;

    *sequence_length = 0;

    if (state->process_count < 1 ||
        state->process_count > MAX_PROCESSES ||
        state->resource_count < 1 ||
        state->resource_count > MAX_RESOURCES)
    {
        return 0;
    }

    /* Copy available resources into Work */
    for (j = 0; j < state->resource_count; j++)
    {
        work[j] = state->available[j];
    }

    /* Mark inactive processes as finished */
    for (i = 0; i < state->process_count; i++)
    {
        if (!state->active[i])
        {
            finish[i] = 1;
        }
    }

    /* Find a safe sequence */
    while (count < state->process_count)
    {
        found = 0;

        for (i = 0; i < state->process_count; i++)
        {
            if (finish[i] == 0)
            {
                for (j = 0; j < state->resource_count; j++)
                {
                    if (state->need[i][j] > work[j])
                    {
                        break;
                    }
                }

                /* Check whether all required resources are available */
                if (j == state->resource_count)
                {
                    for (j = 0; j < state->resource_count; j++)
                    {
                        work[j] += state->allocation[i][j];
                    }

                    safe_sequence[count] = i;
                    count++;

                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        /* No process can proceed */
        if (!found)
        {
            break;
        }
    }

    *sequence_length = count;

    /* Return 1 if all active processes can finish */
    for (i = 0; i < state->process_count; i++)
    {
        if (!finish[i])
        {
            return 0;
        }
    }

    return 1;
}

