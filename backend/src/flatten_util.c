#include "../include/flatten_util.h"
#include "../include/data_structures.h"


void flatten_process_info(flat_process_info *output, const process_info *input){
    // output;
    output->process_id = input->process_id;
    output->parent_pid = input->parent_pid;
    memcpy(output->process_name, input->process_name, sizeof(output->process_name));
    output->process_state = input->process_state;
    output->vm_size = input->vm_size;
    output->vm_res_mem = input->vm_res_mem;
    output->cpuUsage= input->cpuUsage;
    output->thread_count = input->thread_count;
    output->thread_capacity = input->thread_capacity;
    memcpy(output->command, input->command, sizeof(output->command));
    output->is_kernel_thread = input->is_kernel_thread;
    output->process_sched_prio = input->process_sched_prio;
    output->process_sched_policy = input->process_sched_policy;
    output->uptime = input->uptime;

    int thread_limit = (input->thread_count > MAX_THREADS_IN_PROC) ? MAX_THREADS_IN_PROC : input->thread_count;
    // flatten the dynamically malloc'ed thread_info threads[i] struct
    for (int i = 0; i < thread_limit; i++) {
        output->threads[i] = input->threads[i];
    }

    if (thread_limit > MAX_THREADS_IN_PROC) {
        // log silently that process has more threads than can be exported 
        // fprintf(stderr, "Warning: Thread Count for process %d exceeded max_thread_count %d", input->process_id, MAX_THREADS_IN_PROC);
        printf("Warning: Thread Count for process %d exceeded max_thread_count %d", input->process_id, MAX_THREADS_IN_PROC);
    }
}