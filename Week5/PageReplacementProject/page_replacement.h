
#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#define MAX_PAGES 100
#define MAX_FRAMES 20

/* Stores the result of one algorithm */
typedef struct
{
    int frames[MAX_PAGES][MAX_FRAMES];
    int hit[MAX_PAGES];

    int pageFaults;
    int pageHits;

    int referenceCount;
    int frameCount;
} PageResult;

/* FIFO algorithm */
void run_fifo(int pages[], int referenceCount,
              int frameCount, PageResult *result);

/* LRU algorithm */
void run_lru(int pages[], int referenceCount,
             int frameCount, PageResult *result);

/* Optimal algorithm */
void run_optimal(int pages[], int referenceCount,
                 int frameCount, PageResult *result);

#endif
