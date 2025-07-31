#include "data_structures.h"
#include "proc_parser.h"
#include "cpu_monitor.h"
#include "flatten_util.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


void populate_cpu_data() {

    int num_cores;
    CPUStatsArray prev, curr;
    prev = get_cpu_stats_array();
    num_cores = prev.count;
    usleep(1000000);
    curr = get_cpu_stats_array();
    cpu_display_info info[num_cores - 1];

    for (int core = 0; core < num_cores; core++) {
        int core_number = 0;
        if (strcmp(prev.entries[core].id, "cpu") == 0) {
            double cpu_total = compute_cpu_usage1(prev.entries[core], curr.entries[core], false);
            printf("\nCPU Total Usage %.2f%%\n", cpu_total);
            continue;
        } else {
            sscanf(prev.entries[core].id, "cpu%d", &core_number);
            printf("Core found %d\n", core_number);
        }
        
        double usage = compute_cpu_usage1(prev.entries[core], curr.entries[core], false);
        info[core_number].index = core_number;
        info[core_number].usage = usage;
        // printf("Core %d Usage %.2lf%%\n", core_number, usage);
    }
    display_cpu_info(info, num_cores-1);
    free(prev.entries);
    free(curr.entries);

    // cpu_total_stats prev_total, curr_total;
    // cpu_core_stats prev_cores[4], curr_cores[4];

    // read_cpu_stats(&prev_total, prev_cores);
    // usleep(100000);
    // read_cpu_stats(&curr_total, curr_cores);
    
    // int num_cores = read_cpu_stats(prev_cores, false);
    // usleep(100000);    
    // read_cpu_stats(curr_cores, false);
    
    // cpu_display_info info[4];
    // for (int core = 0; core < 4; core++) {
    //     info[core].index = core;
    //     printf("check now %llu %llu\n", prev_cores[0].system,curr_cores[1].system);
    //     info[core].usage = compute_core_cpu_usage(prev_cores[core], curr_cores[core], false);
    //     // display_core_info(&info[core]);
    // }
    
    // // display_cpu_info(info, num_cores);
    // double cpu_total = compute_total_cpu_usage(prev_total, curr_total, false);
    // printf("\nCPU Total Usage %.2f%%\n", cpu_total);
}

// orchestrator layer or the monitor loop that calls the backend data collector functions/apis
int main() {
    printf("Total Processes: %d\n", count_processes(PROC_INCLUDE_ALL));
    printf("Total Thread for Process 1: %d\n", count_threads_of_process("1"));
    printf("Complete uptime: %f\n", read_uptime());

    // process_info proc;
    // flat_process_info flat_data;

    // process_info *procArray;
    int proc_count = 0;
    parse_all_processes( &proc_count, false, false, true);

    // proc = parse_process_info("1502", false, false);
    // traverse_threads_for_a_process("1502", &proc);

    // flatten_process_info(&flat_data, &proc);
    // display_process_info(&proc);
    
    // // TEST TO CHECK FLATTENED DATA BY PRINTING 
    // for (int i=0; i < (int) flat_data.thread_count; i++) {
    //     if (proc.threads[i].thread_id != 0) {
    //         printf("Iterating through this thread_info struct TID: %d T_NAME: %s\n", proc.threads[i].thread_id, proc.threads[i].thread_name);
    //         // display_thread_info(&proc.threads[i]);
    //     }
    // }
    // free(proc.threads);
    // proc.threads = NULL;

    populate_cpu_data();

}
