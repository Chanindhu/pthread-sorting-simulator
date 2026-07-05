# Thread Synchronization Notes

The program uses two worker threads to perform an odd-even style sorting simulation.

## Thread responsibilities

- `T1` compares pairs at even indexes: `A[0]` and `A[1]`, `A[2]` and `A[3]`, and so on.
- `T2` compares pairs at odd/even boundaries: `A[1]` and `A[2]`, `A[3]` and `A[4]`, and so on.

## Shared data

The shared structure stores:

- The array of numbers.
- The number of values read from the input file.
- The total swap count.
- Flags showing whether each thread completed a pass with no swaps.
- The current thread turn.
- Mutex and condition variables used for synchronization.

## Synchronization approach

A `pthread_mutex_t` protects the shared array, swap counters, turn flag, and completion flags.
Condition variables are used to make the threads alternate in the required order:

1. `T1` performs its pass.
2. `T1` signals `T2`.
3. `T2` performs its pass.
4. `T2` signals `T1`.
5. The process continues until both threads complete consecutive no-swap passes.

When both no-swap flags are true, the sorting is complete. The worker thread signals the main thread, and the main thread prints the final sorted array and total swap count.
