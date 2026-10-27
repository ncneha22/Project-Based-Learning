# Page Replacement Algorithm Test Cases

## Test Case 1: Standard Reference String

- **Page reference string:** `7 0 1 2 0 3 0 4 2 3`
- **Number of frames:** 3
- **Purpose:** Compare page hits and page faults for all three algorithms.

Expected and observed results:

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 1 | 9 |
| LRU | 2 | 8 |
| Optimal | 4 | 6 |

**Status:** Tested successfully.

## Test Case 2: All Pages Fit in Frames

- **Page reference string:** `1 2 3 1 2 3`
- **Number of frames:** 3
- **Purpose:** Verify that repeated references are hits when all pages fit in memory.

Observed results:

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 3 | 3 |
| LRU | 3 | 3 |
| Optimal | 3 | 3 |

**Status:** Tested successfully.

## Test Case 3: Repeated Pages with Replacement

- **Page reference string:** `1 2 1 3 1 2`
- **Number of frames:** 2
- **Purpose:** Check how each algorithm handles repeated pages and replacement.

Observed results:

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 1 | 5 |
| LRU | 2 | 4 |
| Optimal | 2 | 4 |

**Status:** Tested successfully.

## Test Case 4: One Frame

- **Page reference string:** `1 2 1 2`
- **Number of frames:** 1
- **Purpose:** Verify replacement behavior with a single frame.

Observed results:

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 0 | 4 |
| LRU | 0 | 4 |
| Optimal | 0 | 4 |

**Status:** Tested successfully.

## Test Case 5: Invalid Empty Input

- **Page reference string:** Empty
- **Number of frames:** 3
- **Expected behavior:** Display an input validation error.
- **Observed behavior:** Displayed "Invalid page-reference string. Enter 1 to 100 non-negative page numbers, separated by spaces or commas."

**Status:** Tested successfully.

## Test Case 6: Invalid Frame Count

- **Page reference string:** `1 2 3`
- **Number of frames:** `0`
- **Expected behavior:** Display an input validation error.
- **Observed behavior:** Displayed "Invalid frame count. Enter a number from 1 to 20."

**Status:** Tested successfully.

## Test Case 7: Invalid Page Reference

- **Page reference string:** `1 x 3`
- **Number of frames:** 2
- **Expected behavior:** Display an input validation error.
- **Observed behavior:** Displayed "Invalid page-reference string. Enter 1 to 100 non-negative page numbers, separated by spaces or commas."

**Status:** Tested successfully.

## Test Case 8: Maximum Allowed Frames

- **Page reference string:** `1 2 3 4`
- **Number of frames:** 20
- **Purpose:** Check that the program handles the maximum supported frame count.

Observed results:

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 0 | 4 |
| LRU | 0 | 4 |
| Optimal | 0 | 4 |

**Status:** Tested successfully.
