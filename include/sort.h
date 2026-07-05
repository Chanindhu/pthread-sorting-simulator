#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <pthread.h>

#define MAX_N 200

/*
 * Shared state used by the two sorting threads.
 * Access to this structure is coordinated with the mutex and condition
 * variables declared inside the structure.
 */
typedef struct {
    int numbers[MAX_N];
    int count;
    int total_swaps;

    pthread_mutex_t mutex;
    pthread_cond_t condition_t1;
    pthread_cond_t condition_t2;
    pthread_cond_t condition_main;
    pthread_cond_t condition_t2_print;

    int current_thread;
    bool t1_no_swap;
    bool t2_no_swap;
    bool done;
    bool t1_printed;
} SharedData;

typedef struct {
    SharedData *data;
    int thread_id;
} SortArgs;

bool valid_integer(const char *str);
void *sort(void *arg);

#endif
