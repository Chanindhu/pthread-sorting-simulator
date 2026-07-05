#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

static void print_sorted_array(const SharedData *data) {
    printf("\nSorted Array A = (");
    for (int i = 0; i < data->count; i++) {
        printf("%d", data->numbers[i]);
        if (i < data->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

static int load_numbers(const char *file_name, SharedData *data) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("File open failed");
        return 1;
    }

    char line[4096];
    int index = 0;

    if (fgets(line, sizeof(line), fp) != NULL) {
        char *token = strtok(line, " \t\r\n");
        while (token != NULL) {
            if (!valid_integer(token)) {
                printf("Error: Invalid input '%s'. Only integers are allowed.\n", token);
                fclose(fp);
                return 1;
            }

            if (index >= MAX_N) {
                printf("Error: Input contains more than %d integers.\n", MAX_N);
                fclose(fp);
                return 1;
            }

            data->numbers[index++] = atoi(token);
            token = strtok(NULL, " \t\r\n");
        }
    }

    fclose(fp);

    if (index == 0) {
        printf("Error: No integers found in input.\n");
        return 1;
    }

    data->count = index;
    return 0;
}

static void initialize_shared_data(SharedData *data) {
    data->total_swaps = 0;
    data->current_thread = 0;
    data->t1_no_swap = false;
    data->t2_no_swap = false;
    data->done = false;
    data->t1_printed = false;

    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->condition_t1, NULL);
    pthread_cond_init(&data->condition_t2, NULL);
    pthread_cond_init(&data->condition_main, NULL);
    pthread_cond_init(&data->condition_t2_print, NULL);
}

static void destroy_shared_data(SharedData *data) {
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->condition_t1);
    pthread_cond_destroy(&data->condition_t2);
    pthread_cond_destroy(&data->condition_main);
    pthread_cond_destroy(&data->condition_t2_print);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s ToSort\n", argv[0]);
        return 1;
    }

    SharedData data;
    if (load_numbers(argv[1], &data) != 0) {
        return 1;
    }

    initialize_shared_data(&data);

    pthread_t tid1;
    pthread_t tid2;
    SortArgs args1 = {&data, 0};
    SortArgs args2 = {&data, 1};

    pthread_create(&tid1, NULL, sort, &args1);
    pthread_create(&tid2, NULL, sort, &args2);

    pthread_mutex_lock(&data.mutex);
    while (!data.done) {
        pthread_cond_wait(&data.condition_main, &data.mutex);
    }
    pthread_mutex_unlock(&data.mutex);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    print_sorted_array(&data);
    printf("Total number of swaps to sort array A = %d.\n", data.total_swaps);

    destroy_shared_data(&data);
    return 0;
}
