#include "page_replacement.h"

void run_lru(int pages[], int referenceCount,
             int frameCount, PageResult *result)
{
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int i, j;
    int found, emptyFrame, replaceFrame;

    if (pages == 0 || result == 0 ||
        referenceCount < 0 || referenceCount > MAX_PAGES ||
        frameCount < 1 || frameCount > MAX_FRAMES)
    {
        return;
    }

    result->pageFaults = 0;
    result->pageHits = 0;
    result->referenceCount = referenceCount;
    result->frameCount = frameCount;

    for (j = 0; j < frameCount; j++)
    {
        frames[j] = -1;
        lastUsed[j] = -1;
    }

    for (i = 0; i < referenceCount; i++)
    {
        found = 0;
        emptyFrame = -1;

        for (j = 0; j < frameCount; j++)
        {
            if (frames[j] == pages[i])
            {
                found = 1;
                lastUsed[j] = i;
                break;
            }

            if (frames[j] == -1 && emptyFrame == -1)
            {
                emptyFrame = j;
            }
        }

        if (found)
        {
            result->hit[i] = 1;
            result->pageHits++;
        }
        else
        {
            result->hit[i] = 0;
            result->pageFaults++;

            if (emptyFrame != -1)
            {
                replaceFrame = emptyFrame;
            }
            else
            {
                replaceFrame = 0;

                for (j = 1; j < frameCount; j++)
                {
                    if (lastUsed[j] < lastUsed[replaceFrame])
                    {
                        replaceFrame = j;
                    }
                }
            }

            frames[replaceFrame] = pages[i];
            lastUsed[replaceFrame] = i;
        }

        for (j = 0; j < MAX_FRAMES; j++)
        {
            result->frames[i][j] = -1;
        }

        for (j = 0; j < frameCount; j++)
        {
            result->frames[i][j] = frames[j];
        }
    }
}
