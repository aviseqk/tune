#include "../include/sorting_utils.h"
#include "../include/data_structures.h"

int compare_by_cpu(const void *a, const void *b) {
    const process_info* p1 = a;
    const process_info* p2 = b;
    return ( p2->cpuUsage > p1->cpuUsage ) - (p2->cpuUsage < p1->cpuUsage);
}

int compare_by_pid(const void *a, const void *b) {
    const process_info* p1 = a;
    const process_info* p2 = b;
    return p1->process_id - p2->process_id;
}

int compare_by_mem(const void *a, const void *b) {
    const process_info* p1 = a;
    const process_info* p2 = b;
    return ( p2->vm_res_mem > p1->vm_res_mem ) - (p2->vm_res_mem < p1->vm_res_mem);

}

int compare_by_uptime(const void *a, const void *b) {
    const process_info* p1 = a;
    const process_info* p2 = b;
    return ( p2->uptime > p1->uptime ) - (p2->uptime < p1->uptime);
}

/* dispatcher function for sorting functions */
int (*get_sort_function(SortKey key))(const void* a, const void* b) {
    switch (key) {
        case SORT_BY_CPU:
            return compare_by_cpu;
        case SORT_BY_PID:
            return compare_by_pid;
        case SORT_BY_MEM:
            return compare_by_mem; 
        case SORT_BY_UPTIME:
            return compare_by_uptime;
    }
}

/* wrapper function for sorting */
void sort_processes(process_info *list, size_t count, SortKey key) {
    qsort(list, count, sizeof(process_info),get_sort_function(key));
}