#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define MAX_THR_NAME_LEN 16
#define MAX_PROC_NAME_LEN 256
#define MAX_THREADS_IN_PROC 64
/***************************************
  Data Structure for Thread Level Info
***************************************/

typedef struct thread_info {
    int thread_id;
    char thread_name[MAX_THR_NAME_LEN];
    char thread_state;
    long cpu_usage_secs;  // total cpu time consumed since inception by this thread
    float cpu_usage_ticks;
    float cpu_usage;    // cpu usage percentage currently being used by this thread
    int vm_size;
    int vm_res_mem;
    int sched_priority;
    int sched_policy;
} thread_info;

/***************************************
  Data Structure for Process Level Info
***************************************/

typedef struct process_info {
    int process_id;
    int parent_pid;
    char process_name[MAX_PROC_NAME_LEN];
    char process_state;
    float vm_size;  // in kb
    float vm_res_mem; // in kb
    float cpuUsage;   // current cpu percentage being used by this process
    size_t thread_count;
    size_t thread_capacity;
    char command[2048];    // the userspace command responsible for launching this process
    int is_kernel_thread;
    int process_sched_prio;
    int process_sched_policy;
    unsigned long uptime;  // how long the process has been running

    thread_info* threads;   // a pointer to start of a dynamic thread_info array

} process_info;

/************************************************
  Flattened Data Structure for Process Level Info
*************************************************/

typedef struct flat_process_info {
    int process_id;
    int parent_pid;
    char process_name[MAX_PROC_NAME_LEN];
    char process_state;
    float vm_size;  // in kb
    float vm_res_mem; // in kb
    float cpuUsage;   // current cpu percentage being used by this process
    size_t thread_count;
    size_t thread_capacity;
    char command[2048];    // the userspace command responsible for launching this process
    int is_kernel_thread;
    int process_sched_prio;
    int process_sched_policy;
    unsigned long uptime;  // how long the process has been running

    thread_info threads[MAX_THREADS_IN_PROC];   // a pointer to start of a dynamic thread_info array

} flat_process_info;


/***************************************
  Data Structure for System Level Info
***************************************/

typedef struct process_list {
    size_t count;
    size_t capacity;
    process_info* processes;    // a pointer to start of a dynamic process_info array
} process_list;

typedef struct cpu_stats {
  int index;
  unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} cpu_stats;

typedef struct cpu_display_info {
  int index;
  double usage;
} cpu_display_info;

void init_process_list(process_list *list);
void display_process_info(const process_info* proc);
void display_thread_info(const thread_info* thread);
void display_core_info(const cpu_display_info *info);
void display_cpu_info(const cpu_display_info* info, int num_cores);
void display_cpu_struct(const cpu_stats* stats, int num_cores);


#endif
