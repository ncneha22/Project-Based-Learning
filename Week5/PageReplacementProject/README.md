# Simulation and Comparative Analysis of Page Replacement Algorithms

## 1. Project Overview

This project simulates and compares three page replacement algorithms used in Operating Systems:

- FIFO (First-In, First-Out)
- LRU (Least Recently Used)
- Optimal Page Replacement

The application is being developed in C using GTK 3 for the graphical user interface.

## 2. Technologies Used

- **Programming Language:** C
- **GUI Toolkit:** GTK 3
- **Data Structures:** Arrays
- **Development Environment:** Visual Studio Code with Ubuntu on WSL
- **Chart Library:** Cairo (planned for comparison visualization)

## 3. Project Files

| File | Purpose |
|---|---|
| `main.c` | Starts the GTK application |
| `gui.c` | Creates the GUI and handles user inputs |
| `page_replacement.h` | Defines the shared result structure and function declarations |
| `fifo.c` | FIFO algorithm implementation |
| `lru.c` | LRU algorithm implementation |
| `optimal.c` | Optimal algorithm implementation |
| `test_cases.md` | Documents test cases |
| `README.md` | Project documentation |

## 4. Planned Features

- Enter a page-reference string.
- Specify the number of frames.
- Select FIFO, LRU, Optimal, or Compare All.
- Display frame contents after each page request.
- Identify page hits and page faults.
- Calculate hit and fault ratios.
- Compare the page-fault counts of the three algorithms.
- Add a comparison chart using Cairo.

## 5. Installation Requirements

On Ubuntu, install the GTK 3 development libraries and compiler:

```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-3-dev libcairo2-dev
```

## 6. Compilation

After all three algorithm implementations are complete, compile the project from the `PageReplacementProject` directory:

```bash
gcc main.c gui.c fifo.c lru.c optimal.c \
    $(pkg-config --cflags --libs gtk+-3.0) \
    -o page_simulator
```

## 7. Run the Application

```bash
./page_simulator
```

## 8. Testing

Test cases, including standard page-reference strings, repeated pages, one-frame scenarios, invalid inputs, and maximum frame counts, are documented in `test_cases.md`.

The algorithm results must be verified before the project is considered complete.

## 9. Project Status

The initial GUI and shared header have been created. The algorithm implementations, full integration, end-to-end testing, and comparison chart remain to be completed and verified.
