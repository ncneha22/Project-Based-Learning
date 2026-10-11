# CPU Scheduling Simulator Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[Enter number of processes]
    B --> C[Enter PID, arrival time, burst time and priority]
    C --> D[Display scheduling algorithm menu]
    D --> E{Select algorithm}
    E -->|1| F[FCFS]
    E -->|2| G[SJF Non-Preemptive]
    E -->|3| H[SJF Preemptive / SRTF]
    E -->|4| I[Priority Non-Preemptive]
    E -->|5| J[Priority Preemptive]
    E -->|6| K[Round Robin]
    F --> L[Run selected scheduling algorithm]
    G --> L
    H --> L
    I --> L
    J --> L
    K --> L
    L --> M[Generate Gantt chart]
    M --> N[Calculate and display scheduling metrics]
    N --> O{Run another algorithm?}
    O -->|Yes| D
    O -->|No| P([End])

Press **Enter**.

Then type:

```text
