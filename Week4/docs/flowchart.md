# Deadlock Simulation System

## 1. Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[Initialize Processes and Resources]
    B --> C[Enter Allocation and Maximum Matrices]
    C --> D[Calculate Need and Available Resources]
    D --> E{Choose Operation}
    E --> F[Banker's Algorithm]
    E --> G[Deadlock Detection]
    E --> H[Resource Request]
    E --> I[Deadlock Recovery]
    F --> J[Display Results]
    G --> J
    H --> J
    I --> J
    J --> K{Continue Simulation?}
    K -- Yes --> E
    K -- No --> L([End])
```
## 2. Flowchart Explanation

1. Start: Begin the Deadlock Simulation System.
2. Initialize: Set up the processes and resource types.
3. Input: Enter the Allocation Matrix, Maximum Requirement Matrix, and total resources.
4. Calculate: Calculate the Need Matrix and Available Resource Vector.
5. Choose Operation: Select Banker's Algorithm, deadlock detection, resource request, or recovery.
6. Analyze: Execute the selected operation and determine the system state.
7. Display Results: Show the analysis results, safe sequence, deadlocked processes, and Resource Allocation Graph where applicable.
8. Continue or End: Continue with another operation or end the simulation.
