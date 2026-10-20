# Interactive Deadlock Simulation System

## Project Overview
The Interactive Deadlock Simulation System is a web-based educational project for understanding resource allocation, safe states, deadlock detection, and recovery in operating systems.

## Objectives
- Represent processes and resource types.
- Maintain allocation and maximum requirement matrices.
- Calculate the Need Matrix.
- Apply Banker's Algorithm to check system safety.
- Display a Resource Allocation Graph.
- Evaluate resource requests.
- Detect deadlocks and demonstrate recovery.

## Technologies Used
- HTML for the interface.
- CSS for styling.
- JavaScript for simulation logic and interaction.

## Project Structure
- `index.html` — main webpage.
- `style.css` — page styling.
- `js/main.js` — interface and event handling.
- `js/resourceManager.js` — process and resource state management.
- `js/bankers.js` — Banker's Algorithm.
- `js/deadlock.js` — deadlock detection.
- `js/rag.js` — Resource Allocation Graph.
- `js/recovery.js` — recovery operations.
- `docs/algorithms.md` — algorithm descriptions.
- `docs/flowchart.md` — system flowchart.
- `docs/testing_report.md` — testing records and test plan.

## How to Run
1. Open a terminal in the project directory.
2. Run `cd Week4`.
3. Start a local server using `python3 -m http.server 8000`.
4. Open `http://localhost:8000` in a browser.
5. Use the webpage to configure processes and resources and run the available simulations.

## Core Concepts
### Banker's Algorithm
Checks whether resource allocation leaves the system in a safe state.

### Deadlock Detection
Examines outstanding resource requirements to identify processes that cannot proceed.

### Resource Requests
Checks request limits and resource availability before evaluating whether granting a request preserves safety.

### Deadlock Recovery
Demonstrates releasing resources from a selected process after a deadlock is identified.

## Documentation
See `algorithms.md`, `flowchart.md`, and `testing_report.md` in this directory for further information.
