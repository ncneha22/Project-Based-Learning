# Simulation and Comparative Analysis of Page Replacement Algorithms

## 1. Project Overview

This project simulates and compares three page replacement algorithms used in Operating Systems:

- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal Page Replacement

The application is developed in C using GTK 3 for the graphical user interface and Cairo for chart drawing.

## 2. Technologies Used

- Programming Language: C
- GUI Toolkit: GTK 3
- Chart Library: Cairo
- Data Structures: Arrays
- Development Environment: Ubuntu on WSL

## 3. Project Files

| File | Purpose |
|---|---|
| `main.c` | Starts the GTK application |
| `gui.c` | Creates the GUI, handles input, and displays results |
| `page_replacement.h` | Shared result structure and function declarations |
| `fifo.c` | FIFO algorithm implementation |
| `lru.c` | LRU algorithm implementation |
| `optimal.c` | Optimal algorithm implementation |
| `chart.c` | Draws the comparison chart |
| `chart.h` | Chart function declaration |
| `test_lru.c` | LRU testing source file |
| `test_cases.md` | Documents test cases |

## 4. Features

- Enter a page-reference string.
- Specify the number of frames.
- Select FIFO, LRU, Optimal, or Compare All.
- Display frame contents after each page request.
- Identify page hits and page faults.
- Calculate hit and fault ratios.
- Compare the results of all three algorithms.
- Display a Cairo chart showing page hits and page faults for FIFO, LRU, and Optimal.

## 5. Installation Requirements

On Ubuntu, install the required compiler and libraries:

```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-3-dev libcairo2-dev
```

## 6. Compilation

Open the terminal in the `PageReplacementProject` directory and run:

```bash
gcc -Wall -Wextra main.c gui.c chart.c fifo.c lru.c optimal.c -o page_replacement $(pkg-config --cflags --libs gtk+-3.0 cairo)
```

## 7. Run the Application

```bash
./page_replacement
```

## 8. Testing

Test cases are documented in `test_cases.md`. They cover standard reference strings, repeated pages, frame limits, and invalid inputs.

The standard reference string has been tested with three frames. Its observed results are:

| Algorithm | Page Hits | Page Faults |
|---|---:|---:|
| FIFO | 1 | 9 |
| LRU | 2 | 8 |
| Optimal | 4 | 6 |

The comparison chart should display these hit and fault counts.

Other test cases should be run to verify their expected behavior.

## 9. Project Status

The GUI, three page replacement algorithms, result display, and comparison chart are implemented. The standard comparison case has been tested. Further test cases should be run to verify input validation and other edge cases.
