#include "data_structures.h"
#include "proc_parser.h"
#include "cpu_monitor.h"
#include "flatten_util.h"

#include <stdio.h>

// orchestrator layer or the monitor loop that calls the backend data collector functions/apis
int main() {
    printf("Total Processes: %d\n", count_processes(PROC_INCLUDE_ALL));
    printf("Total Thread for Process 1: %d\n", count_threads_of_process("3689"));
    printf("Complete uptime: %f\n", read_uptime());

    process_info proc;
    flat_process_info flat_data;

    proc = parse_process_info("3689", false, false);
    traverse_threads_for_a_process("3689", &proc);

    flatten_process_info(&flat_data, &proc);
    // display_process_info(&proc);
    
    // TEST TO CHECK FLATTENED DATA BY PRINTING 
    for (int i=0; i < (int) flat_data.thread_count; i++) {
        if (proc.threads[i].thread_id != 0) {
            printf("Iterating through this thread_info struct TID: %d T_NAME: %s\n", proc.threads[i].thread_id, proc.threads[i].thread_name);
            display_thread_info(&proc.threads[i]);
        }
    }
    
    free(proc.threads);
    proc.threads = NULL;

}