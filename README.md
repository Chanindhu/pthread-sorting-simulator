# pthread Sorting Simulator

A C command-line program that simulates a simple two-thread sorting algorithm using POSIX threads. The program reads integers from a file, coordinates two worker threads with mutexes and condition variables, and prints the sorted array with per-thread and total swap counts.

## Overview

The simulator uses two threads:

- **T1** checks adjacent pairs starting at even indexes: `A[0]`/`A[1]`, `A[2]`/`A[3]`, etc.
- **T2** checks adjacent pairs starting at odd indexes: `A[1]`/`A[2]`, `A[3]`/`A[4]`, etc.

The threads alternate using `pthread_cond_wait()` and `pthread_cond_signal()`. Sorting stops when both threads complete consecutive passes without making a swap.

## Features

- C implementation using POSIX threads.
- Thread synchronization with mutexes and condition variables.
- Shared swap counter protected by a mutex.
- Input validation for integer-only files.
- Supports up to 200 integers.
- Includes sample input and output files.
- Simple `Makefile` for build and run commands.

## Project Structure

```text
pthread-sorting-simulator/
├── Makefile
├── README.md
├── include/
│   └── sort.h
├── src/
│   ├── main.c
│   └── sort.c
├── data/
│   ├── ToSort
│   ├── example-input.txt
│   └── small-input.txt
└── docs/
    ├── sample-output.txt
    └── thread-synchronization.md
```

## Requirements

- GCC or Clang
- POSIX thread support
- macOS, Linux, or WSL on Windows

## Build

```bash
make
```

This creates the executable:

```text
sss
```

You can also compile manually:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude src/main.c src/sort.c -pthread -o sss
```

## Run

```bash
./sss data/example-input.txt
```

Or use:

```bash
make run
```

## Example Input

```text
5 3 11 2 1 4 5 1 10 11 21 17 25 16 6
```

## Example Output

```text
Thread ID1: total number of swaps = 16
Thread ID2: total number of swaps = 13

Sorted Array A = (1, 1, 2, 3, 4, 5, 5, 6, 10, 11, 11, 16, 17, 21, 25)
Total number of swaps to sort array A = 29.
```

## Notes

This project was originally developed as an Operating Systems coursework project and later cleaned, documented, and prepared for portfolio presentation.
