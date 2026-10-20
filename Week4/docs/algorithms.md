# Deadlock Simulation System — Algorithms

## 1. Banker's Algorithm

**Purpose:** To determine whether the system is in a safe state.

Steps:
1. Calculate the Need Matrix using Maximum − Allocation.
2. Set Work equal to Available.
3. Find a process whose Need is less than or equal to Work.
4. Assume that process finishes and releases its allocated resources.
5. Add those resources to Work and mark the process as finished.
6. Repeat until all processes finish or no suitable process can be found.
7. If all processes finish, the system is SAFE and a safe sequence exists. Otherwise, the state is UNSAFE.

## 2. Deadlock Detection

**Purpose:** To identify processes that cannot proceed because of resource dependencies.

Steps:
1. Examine the Available Resource Vector and outstanding requests.
2. Check whether a process's outstanding request can be satisfied.
3. If it can, assume the process finishes and releases its allocated resources.
4. Repeat the check for the remaining processes.
5. Identify processes that remain unfinished as potential deadlocked processes, according to the detection method used.

## 3. Resource Request Algorithm

**Purpose:** To decide whether a resource request can be granted safely.

Steps:
1. Receive a request from a process.
2. Check that the request does not exceed the process's remaining Need.
3. Check that sufficient resources are Available.
4. Temporarily allocate the requested resources.
5. Run the safety check.
6. Grant the request if the resulting state is safe.
7. Otherwise, restore the previous state and deny the request.

## 4. Deadlock Recovery

**Purpose:** To help the system recover after a deadlock is detected.

Steps:
1. Identify the deadlocked processes.
2. Select a process for recovery.
3. Release its allocated resources.
4. Update the Available Resource Vector and system state.
5. Run deadlock detection again to check whether the deadlock has been resolved.
