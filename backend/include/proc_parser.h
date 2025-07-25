#ifndef PROC_THREAD_PARSER_H
#define PROC_THREAD_PARSER_H
#include "data_structures.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>


typedef enum ProcFilterFlags {
    PROC_INCLUDE_ALL = 0,      // NO EXCLUSIONS
    PROC_EXCLUDE_KERNEL = 1 << 1,
    PROC_EXCLUDE_ZOMBIE = 1 << 2,
} ProcFilerFlags;

thread_info parse_thread_info(char pid[], char tid[]);

void get_utime_stime(char pid[], unsigned long *utime, unsigned long *stime);

process_info parse_process_info(char pid[], bool cpu_filter, bool mem_filter);

process_info* parse_all_processes(int* count, bool zombie_pid_filter, bool kthread_pid_filter, bool log);

// void parse_all_processes(int* count);

int count_processes(ProcFilerFlags flags);
int count_threads_of_process(char pid[]);
bool is_zombie(char pid[]);
int is_kernel_thread(char pid[]);

void traverse_threads_for_a_process(char pid[], process_info* process_detail);

#endif