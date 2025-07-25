#ifndef SORTING_UTILS_H
#define SORTING_UTILS_H
#include <stdlib.h>
#include "data_structures.h"

typedef enum {
    SORT_BY_PID,
    SORT_BY_CPU,
    SORT_BY_MEM,
    SORT_BY_UPTIME
} SortKey;

int compare_by_pid(const void* a, const void *b);

int compare_by_cpu(const void* a, const void *b);

int compare_by_mem(const void* a, const void *b);

int compare_by_uptime(const void* a, const void *b);

void sort_processes(process_info *list, size_t count, SortKey key);

#endif