#include "../include/data_structures.h"
#include <stdlib.h>

#define INITIAL_PROC_CAPACITY 64;

void init_process_list(process_list *list) {
    list->count = 0;
    list->capacity = INITIAL_PROC_CAPACITY;
    list->processes = calloc(list->capacity, sizeof(process_info));

    if (!list->processes) {
        perror("calloc failed - process_list");
        exit(EXIT_FAILURE);
    }
}


void display_process_info(const process_info* proc) {
    if (proc == NULL) {
        printf("Invalid process_info pointer.\n");
        return;
    }
    printf("========= Process Info =========\n");
    printf("Process ID        : %d\n", proc->process_id);
    printf("Parent Process ID : %d\n", proc->parent_pid);
    printf("Process Name      : %s\n", proc->process_name);
    printf("Process State     : %c\n", proc->process_state);
    printf("Command           : %s\n", proc->command);
    printf("Process Uptime    : %ld secs\n", proc->uptime);
    printf("VM Size (kB)      : %.2f kB\n", proc->vm_size);
    printf("VM Resident Mem   : %.2f kB\n", proc->vm_res_mem);
    printf("CPU Usage (%%)    : %.2f\n", proc->cpuUsage);
    printf("Process Sch Policy: %d\n", proc->process_sched_policy);
    printf("Process Priority  : %d\n", proc->process_sched_prio);
    printf("Thread Count      : %zu\n", proc->thread_count);
    printf("Kernel Thread     : %s\n", proc->is_kernel_thread ? "Yes" : "No");
}

void display_complete_process_info(const process_info* proc) {
    display_process_info(proc);

    for (int i=0; i < (int) proc->thread_count; i++) {
        if (proc->threads[i].thread_id != 0) {
            // printf("Iterating through this thread_info struct TID: %d T_NAME: %s\n", proc->threads[i].thread_id, proc->threads[i].thread_name);
            display_thread_info(&proc->threads[i]);
        }
    }

}

void display_thread_info(const thread_info* thread) {
    if (thread == NULL) {
        printf("Invalid thread_info pointer.\n");
        return;
    }
    printf("========= Thread Info =========\n");
    printf("Thread ID         : %d\n", thread->thread_id);
    printf("Thread Name       : %s\n", thread->thread_name);
    printf("Thread State      : %c\n", thread->thread_state);
    printf("CPU Usage (secs)  : %ld\n", thread->cpu_usage_secs);
    printf("CPU Usage (ticks) : %f\n", thread->cpu_usage_ticks);
    printf("Memory Usage (kB) : %d kB\n", thread->vm_size);
    printf("Memory Usage Res  : %d kB\n", thread->vm_res_mem);
    printf("Sched Priority    : %d\n", thread->sched_priority);
    printf("Sched Policy      : %d\n", thread->sched_policy);
}


void display_core_info(const cpu_display_info* info) {
    if (info == NULL) {
        printf("Invalid core_display_info pointer");
        return;
    }

    printf("--------CPU USAGE EACH CORE--------\n");
    printf("CPU Core  [%d]          : %.2f\n", info->index, info->usage);
}

void display_cpu_info(const cpu_display_info* info, int num_cores) {
    if (info == NULL) {
        printf("Invalid cpu_display_info pointer");
        return;
    }

    printf("-------CPU USAGE PER CORE--------\n");
    for (int j = 0; j < num_cores; j++) {
        printf("CPU Core  [%d]          : %.2f\n", info[j].index, info[j].usage);
    }
}

void display_cpu_struct(const cpu_stats* stats, int num_cores) {
    printf("=== CPU Stats ===\n");
    if (num_cores == 0) {
        printf("CPU %d:\n", stats[0].index);
        printf("  user     : %llu\n", stats[0].user);
        printf("  nice     : %llu\n", stats[0].nice);
        printf("  system   : %llu\n", stats[0].system);
        printf("  idle     : %llu\n", stats[0].idle);
        printf("  iowait   : %llu\n", stats[0].iowait);
        printf("  irq      : %llu\n", stats[0].irq);
        printf("  softirq  : %llu\n", stats[0].softirq);
        printf("  steal    : %llu\n", stats[0].steal);
        printf("----------------------\n");
    } else {
    for (int i = 0; i < num_cores; i++) {
        printf("CPU %d:\n", stats[i].index);
        printf("  user     : %llu\n", stats[i].user);
        printf("  nice     : %llu\n", stats[i].nice);
        printf("  system   : %llu\n", stats[i].system);
        printf("  idle     : %llu\n", stats[i].idle);
        printf("  iowait   : %llu\n", stats[i].iowait);
        printf("  irq      : %llu\n", stats[i].irq);
        printf("  softirq  : %llu\n", stats[i].softirq);
        printf("  steal    : %llu\n", stats[i].steal);
        printf("----------------------\n");
    }
}
}