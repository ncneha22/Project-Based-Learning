
# Deadlock Simulation System

## 1. Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[Initialize Processes and Resources]
    B --> C[Enter Allocation, Maximum and Request Matrices]
    C --> D[Calculate Need and Available Resources]
    D --> E{Choose Operation}

    E --> F[Banker's Algorithm]
    E --> G[Deadlock Detection]
    E --> H[Resource Request]
    E --> I[Deadlock Recovery]
    E --> M[Show Resource Allocation Graph]
    E --> R[Reset Simulation]

    F --> J[Display Results]
    G --> N{Deadlock Detected?}
    N -- Yes --> O[Display Deadlocked Processes and Waiting Chain]
    O --> P[Highlight Deadlocked Processes and Relevant Graph Edges]
    P --> J
    N -- No --> J
    H --> J
    I --> J
    M --> J

    J --> K{Continue Simulation?}
    K -- Yes --> E
    K -- No --> L([End])

    R --> S{Confirm Reset?}
    S -- Yes --> T[Reload Initial State]
    T --> B
    S -- No --> E
```

## 2. Flowchart Explanation

1. **Start:** Begin the Deadlock Simulation System.
2. **Initialize:** Set up the processes and resource types.
3. **Input:** Enter the Allocation Matrix, Maximum Requirement Matrix, resource requests, and total resources as required.
4. **Calculate:** Calculate the Need Matrix and Available Resource Vector.
5. **Choose Operation:** Select Banker's Algorithm, deadlock detection, resource request, recovery, Resource Allocation Graph display, or Reset Simulation.
6. **Analyze:** Execute the selected operation and determine the system state.
7. **Detect Deadlock:** Check whether processes can finish using the available resources and outstanding requests.
8. **Display Results:** Show the analysis results, safe sequence where applicable, deadlocked processes, and waiting chain when a deadlock is detected.
9. **Update Graph:** If the Resource Allocation Graph has been displayed, refresh it to highlight deadlocked processes and relevant edges.
10. **Reset Simulation:** When Reset Simulation is selected, display a confirmation popup. If the user confirms, reload the page and return to the initial simulation state. If the user cancels, return to the operation menu without resetting.
11. **Continue or End:** Continue with another operation or end the simulation.
```
