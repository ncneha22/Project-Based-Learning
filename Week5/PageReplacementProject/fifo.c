#include <stddef.h>
#include "page_replacement.h"

void run_fifo(int pages[], int referenceCount,
              int frameCount, PageResult *result)
{
    int frame[MAX_FRAMES];
    int i, j;
    int nextReplace = 0;

    if (result == NULL)
        return;

    /* Initialize result counters. */
    result->pageFaults = 0;
    result->pageHits = 0;
    result->referenceCount = 0;
    result->frameCount = 0;

    /* Validate input limits. */
    if (pages == NULL ||
        referenceCount <= 0 ||
        referenceCount > MAX_PAGES ||
        frameCount <= 0 ||
        frameCount > MAX_FRAMES)
    {
        return;
    }

    result->referenceCount = referenceCount;
    result->frameCount = frameCount;

    /* Initialize frames as empty. */
    for (i = 0; i < frameCount; i++)
    {
        frame[i] = -1;
    }

    /* Process each page request. */
    for (i = 0; i < referenceCount; i++)
    {
        int found = 0;

        /* Check whether the page is already in memory. */
        for (j = 0; j < frameCount; j++)
        {
            if (frame[j] == pages[i])
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            /* Page hit. */
            result->hit[i] = 1;
            result->pageHits++;
        }
        else
        {
            /* Page fault: insert or replace a page. */
            result->hit[i] = 0;
            result->pageFaults++;

            frame[nextReplace] = pages[i];

            /* Move to the next frame in FIFO order. */
            nextReplace = (nextReplace + 1) % frameCount;
        }

        /* Save frame contents after this request. */
        for (j = 0; j < frameCount; j++)
        {
            result->frames[i][j] = frame[j];
        }
    }
}