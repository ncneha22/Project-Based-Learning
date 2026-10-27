#include <stdio.h>
#include "page_replacement.h"

void test_lru(int pages[], int count, int frameCount, int testNumber)
{
    PageResult result = {0};

    run_lru(pages, count, frameCount, &result);

    printf("\nTest Case %d\n", testNumber);
    printf("Pages: ");

    for (int i = 0; i < count; i++)
        printf("%d ", pages[i]);

    printf("\nFrames: %d\n", frameCount);
    printf("Page Faults: %d\n", result.pageFaults);
    printf("Page Hits: %d\n", result.pageHits);
    printf("Frame contents after each request:\n");

    for (int i = 0; i < count; i++)
    {
        printf("Page %d: ", pages[i]);

        for (int j = 0; j < frameCount; j++)
        {
            if (result.frames[i][j] == -1)
                printf("- ");
            else
                printf("%d ", result.frames[i][j]);
        }

        printf("(%s)\n", result.hit[i] ? "Hit" : "Fault");
    }
}

int main(void)
{
    int pages1[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    int pages2[] = {1, 2, 3, 1, 2, 3};
    int pages3[] = {1, 2, 1, 3, 1, 2};
    int pages4[] = {1, 2, 1, 2};
    int pages5[] = {1, 2, 3, 4};

    test_lru(pages1, 10, 3, 1);
    test_lru(pages2, 6, 3, 2);
    test_lru(pages3, 6, 2, 3);
    test_lru(pages4, 4, 1, 4);
    test_lru(pages5, 4, 20, 8);

    return 0;
}
