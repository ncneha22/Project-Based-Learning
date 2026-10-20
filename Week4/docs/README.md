# Interactive Deadlock Simulation System

## Project Overview

The Interactive Deadlock Simulation System is a web-based educational project for understanding resource allocation, safe states, deadlock detection, and recovery in operating systems. It also provides a Reset Simulation feature that allows users to return the application to its initial state.

## Objectives

- Represent processes and resource types.
- Maintain allocation, maximum requirement, and resource request information.
- Calculate the Need Matrix and Available Resource Vector.
- Apply Banker's Algorithm to check system safety.
- Display a Resource Allocation Graph with process and resource nodes.
- Show resource allocation and request relationships in the graph.
- Detect deadlocks and display the affected processes and waiting chain.
- Highlight deadlocked processes and relevant graph edges.
- Demonstrate resource request evaluation and deadlock recovery.
- Reset the simulation to its initial state using a confirmation popup.

## Technologies Used

- HTML for the interface.
- CSS for styling.
- JavaScript for simulation logic and interaction.
- SVG for drawing the Resource Allocation Graph.

## Project Structure

- `index.html` — main webpage.
- `style.css` — page styling.
- `js/main.js` — interface and event handling.
- `js/resourceManager.js` — process and resource state management.
- `js/bankers.js` — Banker's Algorithm.
- `js/deadlock.js` — deadlock detection and waiting-chain output.
- `js/rag.js` — Resource Allocation Graph visualization.
- `js/recovery.js` — recovery operations.
- `docs/README.md` — project overview and instructions.
- `docs/algorithms.md` — algorithm descriptions.
- `docs/flowchart.md` — system flowchart.
- `docs/testing_report.md` — testing records and test plan.

## How to Run

1. Open a terminal in the project directory.
2. Navigate to the Week4 project folder using `cd Week4`.
3. Start a local server using `python3 -m http.server 8000`.
4. Open `http://localhost:8000` in a browser.
5. Configure processes and resources, enter the required matrices, and run the available simulations.
6. Use the **Reset Simulation** button to restart the simulation when needed. Confirm the popup to reload the application to its initial state.

## Core Concepts

### Banker's Algorithm

Checks whether resource allocation leaves the system in a safe state and determines a safe sequence when one exists.

### Deadlock Detection

Examines available resources, process allocations, and outstanding requests to identify processes that cannot finish under the detection procedure.

### Resource Allocation Graph

Visualizes processes, resource types, resource allocations, and outstanding requests. When a deadlock is detected, the graph can highlight deadlocked processes and relevant edges.

### Resource Requests

Evaluates resource requests against the applicable limits and available resources before checking whether a request can be granted safely.

### Deadlock Recovery

Demonstrates releasing resources from a selected process after a deadlock is identified.

### Reset Simulation

Provides a confirmation popup before reloading the application. When the user confirms, the simulation returns to its initial state. If the user cancels, the current simulation should remain unchanged.

## Documentation

Refer to `algorithms.md`, `flowchart.md`, and `testing_report.md` in this directory for further information.
