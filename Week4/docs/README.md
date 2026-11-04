# Interactive Deadlock Simulation System

## Project Overview
The Interactive Deadlock Simulation System is a desktop application developed using C and GTK 3. It demonstrates resource allocation, Banker's Algorithm, deadlock detection, Resource Allocation Graph visualization, and deadlock recovery.

## Technologies Used
- C programming language
- GTK 3
- Cairo
- GCC compiler

## Main Features
1. Dashboard for creating, loading, and resetting systems.
2. Matrices for viewing Allocation, Maximum, Need, and Available resources.
3. Banker's Algorithm for checking system safety and displaying a safe sequence.
4. Deadlock Detection for identifying deadlock conditions.
5. Resource Allocation Graph for displaying processes, resources, and request/allocation edges.
6. Requests and Recovery for requesting resources, releasing resources, and recovering processes.

## Project Files
- `main.c` – Initializes GTK and starts the application.
- `app.c` – Handles the graphical interface and user interactions.
- `app.h` – Application declarations.
- `bankers.c` – Banker's Algorithm.
- `deadlock.c` – Deadlock detection.
- `rag.c` – Resource Allocation Graph logic.
- `recovery.c` – Process recovery.
- `resource_manager.c` – Resource requests and releases.
- `deadlock.h` – Shared system structures and declarations.
- `resource_manager.h` – Resource manager declarations.
- `testing_report.md` – Testing report.

## Compilation
Run from the Week4 directory:

    gcc -Wall -Wextra -std=c11 main.c app.c bankers.c deadlock.c rag.c recovery.c resource_manager.c -o deadlock_gui $(pkg-config --cflags --libs gtk+-3.0) -lm

## Execution
After compilation, run:

    ./deadlock_gui

## Testing
Refer to `testing_report.md` for testing details and results.
