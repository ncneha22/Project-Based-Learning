/*
 * optimal.c - Optimal Page Replacement Algorithm
 *
 * Idea: when a page fault happens and every frame is full, look ahead
 * in the reference string and throw out the page that will be needed
 * again furthest in the future (or never again).
 *
 * Results are stored in the PageResult structure from page_replacement.h:
 *   result->frames[i][f]  frame f's page after request i (-1 = empty)
 *   result->hit[i]        1 if request i was a hit, 0 if it was a fault
 */

#include "page_replacement.h"

/* Return the frame index holding page, or -1 if it is not in memory. */
static int find_page(int frames[], int frameCount, int page)
{
    for (int f = 0; f < frameCount; f++)
    {
        if (frames[f] == page)
        {
            return f;
        }
    }

    return -1;
}

/*
 * Choose which frame to replace.
 * For each page in memory, find when it is next used after position
 * current. The page used furthest away (or never) is the victim.
 */
static int find_victim(int pages[], int referenceCount, int current,
                       int frames[], int frameCount)
{
    int victim = 0;
    int farthest = -1;

    for (int f = 0; f < frameCount; f++)
    {
        /* Assume the page is never used again. */
        int nextUse = referenceCount;

        for (int k = current + 1; k < referenceCount; k++)
        {
            if (pages[k] == frames[f])
            {
                nextUse = k;
                break;
            }
        }

        /* Never used again: this is the best choice, stop looking. */
        if (nextUse == referenceCount)
        {
            return f;
        }

        if (nextUse > farthest)
        {
            farthest = nextUse;
            victim = f;
        }
    }

    return victim;
}

void run_optimal(int pages[], int referenceCount,
                 int frameCount, PageResult *result)
{
    int frames[MAX_FRAMES];

    if (result == 0)
    {
        return;
    }

    /* Keep the counts inside the array limits from the header. */
    if (referenceCount < 0)
    {
        referenceCount = 0;
    }
    if (referenceCount > MAX_PAGES)
    {
        referenceCount = MAX_PAGES;
    }
    if (frameCount < 0)
    {
        frameCount = 0;
    }
    if (frameCount > MAX_FRAMES)
    {
        frameCount = MAX_FRAMES;
    }

    /* Step 1: initialise the result. */
    result->pageFaults = 0;
    result->pageHits = 0;
    result->referenceCount = referenceCount;
    result->frameCount = frameCount;

    for (int i = 0; i < MAX_PAGES; i++)
    {
        result->hit[i] = 0;

        for (int f = 0; f < MAX_FRAMES; f++)
        {
            result->frames[i][f] = -1;
        }
    }

    if (pages == 0 || frameCount == 0)
    {
        return;
    }

    /* Step 2: all frames start empty. */
    for (int f = 0; f < frameCount; f++)
    {
        frames[f] = -1;
    }

    /* Step 3: process each page request in order. */
    for (int i = 0; i < referenceCount; i++)
    {
        int page = pages[i];

        if (find_page(frames, frameCount, page) != -1)
        {
            /* Page is already in memory: hit. */
            result->pageHits++;
            result->hit[i] = 1;
        }
        else
        {
            /* Page is not in memory: fault. */
            result->pageFaults++;
            result->hit[i] = 0;

            /* Use an empty frame if there is one. */
            int empty = find_page(frames, frameCount, -1);

            if (empty != -1)
            {
                frames[empty] = page;
            }
            else
            {
                int victim = find_victim(pages, referenceCount, i,
                                         frames, frameCount);
                frames[victim] = page;
            }
        }

        /* Step 4: save a snapshot of the frames after this request. */
        for (int f = 0; f < frameCount; f++)
        {
            result->frames[i][f] = frames[f];
        }
    }
}
