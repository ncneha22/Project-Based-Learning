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
- **Expected page faults:** 3
- **Expected page hits:** 3
- **Purpose:** Verify that repeated references are hits when all pages fit in memory.

## Test Case 3: Repeated Pages with Replacement

- **Page reference string:** `1 2 1 3 1 2`
- **Number of frames:** 2
- **Purpose:** Check how each algorithm handles repeated pages and replacement.
- **Expected behavior:** Each algorithm should report its page hits and page faults according to its replacement policy.

## Test Case 4: One Frame

- **Page reference string:** `1 2 1 2`
- **Number of frames:** 1
- **Expected page faults:** 4
- **Expected page hits:** 0
- **Purpose:** Verify replacement behavior with a single frame.

## Test Case 5: Invalid Empty Input

- **Page reference string:** Empty
- **Number of frames:** 3
- **Expected behavior:** Display an input validation error.

## Test Case 6: Invalid Frame Count

- **Page reference string:** `1 2 3`
- **Number of frames:** `0`
- **Expected behavior:** Display an input validation error.

## Test Case 7: Invalid Page Reference

- **Page reference string:** `1 x 3`
- **Number of frames:** 2
- **Expected behavior:** Display an input validation error.

## Test Case 8: Maximum Allowed Frames

- **Page reference string:** `1 2 3 4`
- **Number of frames:** 20
- **Purpose:** Check that the program handles the maximum supported frame count.
- **Expected behavior:** Process the input without exceeding supported limits.
