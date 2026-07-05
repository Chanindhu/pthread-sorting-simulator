#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

bool valid_integer(const char *str) {
    if (*str == '-' || *str == '+') {
        str++;
    }

    if (*str == '\0') {
        return false;
    }

    while (*str != '\0') {
        if (!isdigit((unsigned char)*str)) {
            return false;
        }
        str++;
    }

    return true;
}

static int compare_even_pairs(SharedData *data) {
    int swaps = 0;

    for (int i = 0; i < data->count / 2; i++) {
        int left = 2 * i;
        int right = left + 1;

        if (data->numbers[left] > data->numbers[right]) {
            int temp = data->numbers[left];
            data->numbers[left] = data->numbers[right];
            data->numbers[right] = temp;
            swaps++;
        }
    }

    return swaps;
}

static int compare_odd_pairs(SharedData *data) {
    int swaps = 0;

    for (int i = 1; i <= (data->count - 1) / 2; i++) {
        int left = 2 * i - 1;
        int right = 2 * i;

        if (data->numbers[left] > data->numbers[right]) {
            int temp = data->numbers[left];
            data->numbers[left] = data->numbers[right];
            data->numbers[right] = temp;
            swaps++;
        }
    }

    return swaps;
}

static void wait_for_turn(SharedData *data, int thread_id) {
    while (data->current_thread != thread_id && !data->done) {
        if (thread_id == 0) {
            pthread_cond_wait(&data->condition_t1, &data->mutex);
        } else {
            pthread_cond_wait(&data->condition_t2, &data->mutex);
        }
    }
}

static void signal_next_thread(SharedData *data, int thread_id) {
    data->current_thread = (thread_id == 0) ? 1 : 0;

    if (thread_id == 0) {
        pthread_cond_signal(&data->condition_t2);
    } else {
        pthread_cond_signal(&data->condition_t1);
    }
}

static void print_thread_swaps(SharedData *data, int thread_id, int cumulative_swaps) {
    pthread_mutex_lock(&data->mutex);

    if (thread_id == 0) {
        printf("Thread ID1: total number of swaps = %d\n", cumulative_swaps);
        data->t1_printed = true;
        pthread_cond_signal(&data->condition_t2_print);
    } else {
        while (!data->t1_printed) {
            pthread_cond_wait(&data->condition_t2_print, &data->mutex);
        }
        printf("Thread ID2: total number of swaps = %d\n", cumulative_swaps);
    }

    pthread_mutex_unlock(&data->mutex);
}

void *sort(void *arg) {
    SortArgs *args = (SortArgs *)arg;
    SharedData *data = args->data;
    int thread_id = args->thread_id;
    int cumulative_swaps = 0;

    while (true) {
        pthread_mutex_lock(&data->mutex);
        wait_for_turn(data, thread_id);

        if (data->done) {
            pthread_mutex_unlock(&data->mutex);
            break;
        }

        int local_swaps;
        if (thread_id == 0) {
            local_swaps = compare_even_pairs(data);
            data->t1_no_swap = (local_swaps == 0);
        } else {
            local_swaps = compare_odd_pairs(data);
            data->t2_no_swap = (local_swaps == 0);
        }

        data->total_swaps += local_swaps;
        cumulative_swaps += local_swaps;

        if (data->t1_no_swap && data->t2_no_swap) {
            data->done = true;
            pthread_cond_signal(&data->condition_main);
        }

        signal_next_thread(data, thread_id);
        pthread_mutex_unlock(&data->mutex);
    }

    print_thread_swaps(data, thread_id, cumulative_swaps);
    return NULL;
}
