#include "../include/cpu_monitor.h"
#include "../include/data_structures.h"
#include <stdio.h>

#define CPUS_MAX 8
#define CPU_INTERVAL 500000

typedef unsigned long long my_u64;

// void populate_cores_cpu_stats(cpu_stats *prev_cores, cpu_stats *curr_cores) {

//     int num_cores = read_cpu_stats(prev_cores, false);

//     printf("NUM CORES : %d\n", num_cores);

//     usleep(CPU_INTERVAL);

//     read_cpu_stats(curr_cores, false);

//     cpu_display_info info[num_cores];
//     for (int core = 0; core < num_cores; core++) {
//         info[core].index = core;
//         info[core].usage = compute_cpu_usage(prev_cores[core], curr_cores[core], false);

//         // display_core_info(&info[core]);
//     }

//     display_cpu_info(info, num_cores);
// }

/* returns number of cores */
int read_cpu_stats(cpu_stats *stats, bool total_cpu_flag) {
    FILE *fp = fopen("/proc/stat", "r");

    if (!fp) {
        perror("fopen /proc/stat error");
        exit(EXIT_FAILURE);
    }

    char line[512];
    int core = 0;


    while (fgets(line, sizeof(line), fp)) {
        
        if (strncmp(line, "cpu", 3) != 0) break;
        
        if (strncmp(line, "cpu ", 4) == 0){
            if (total_cpu_flag) {
                printf("read cpu line %s", line);
                int core = 0;
                sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
            &stats[core].user, &stats[core].nice, &stats[core].system, &stats[core].idle, 
            &stats[core].iowait,  &stats[core].irq, &stats[core].softirq, &stats[core].steal);
                return 0;
            }
        }

        if (!isdigit((unsigned char)line[3])) continue;

        printf("%s", line);
        int matched = sscanf(line, "cpu%d %llu %llu %llu %llu %llu %llu %llu %llu", &stats[core].index,
            &stats[core].user, &stats[core].nice, &stats[core].system, &stats[core].idle, 
            &stats[core].iowait,  &stats[core].irq, &stats[core].softirq, &stats[core].steal);
            
        
        if (matched < 9) {
            fprintf(stderr, "Warning: Malformed line in /proc/stat: %s\n", line);
            continue;
        }

        if (core >= CPUS_MAX) {
            fprintf(stderr, "Warning: core_id %d exceeds CPUS_MAX %d\n", core, CPUS_MAX);
            continue;
        }
        core++;
    }

    fclose(fp);

    return core;
}

int read_process_cpu_stats(cpu_stats *stats, char pid[],  bool total_cpu_flag) {

    char stat_path[256];
    sprintf(stat_path, "/proc/%s/stat", pid);

    FILE *fp = fopen(stat_path, "r");

    if (!fp) {
        perror("fopen /proc/stat error");
        exit(EXIT_FAILURE);
    }

    char line[512];
    int core = 0;


    while (fgets(line, sizeof(line), fp)) {
        
        if (strncmp(line, "cpu", 3) != 0) break;
        
        if (strncmp(line, "cpu ", 4) == 0){
            if (total_cpu_flag) {
                printf("read cpu line %s", line);
                int core = 0;
                sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
            &stats[core].user, &stats[core].nice, &stats[core].system, &stats[core].idle, 
            &stats[core].iowait,  &stats[core].irq, &stats[core].softirq, &stats[core].steal);
                return 0;
            }
        }

        if (!isdigit((unsigned char)line[3])) continue;

        printf("%s", line);
        int matched = sscanf(line, "cpu%d %llu %llu %llu %llu %llu %llu %llu %llu", &stats[core].index,
            &stats[core].user, &stats[core].nice, &stats[core].system, &stats[core].idle, 
            &stats[core].iowait,  &stats[core].irq, &stats[core].softirq, &stats[core].steal);
            
        
        if (matched < 9) {
            fprintf(stderr, "Warning: Malformed line in /proc/stat: %s\n", line);
            continue;
        }

        if (core >= CPUS_MAX) {
            fprintf(stderr, "Warning: core_id %d exceeds CPUS_MAX %d\n", core, CPUS_MAX);
            continue;
        }
        core++;
    }

    fclose(fp);

    return core;
}

/* calculates the cpu usage metrics using delta/diff */
double compute_cpu_usage(cpu_stats prev, cpu_stats curr, bool jiffy_flag) {

    my_u64 prev_idle = prev.idle + prev.iowait;
    my_u64 prev_non_idle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;

    my_u64 prev_total = prev_idle + prev_non_idle;
    
    my_u64 curr_idle = curr.idle + curr.iowait;
    my_u64 curr_non_idle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;
    
    my_u64 curr_total = curr_idle + curr_non_idle;

    // printf("Prev Total: %lld  Curr Total: %lld\n", prev_total, curr_total);
    // printf("Prev Idle: %lld Curr Idle: %lld\n", prev_idle, curr_idle);

    double total_delta = curr_total - prev_total;
    double idle_delta = curr_idle - prev_idle;
    double usage = ( ( total_delta - idle_delta ) / total_delta ) * 100.0;

    // printf("Total Delta: %f Idle Delta: %f\n", total_delta, idle_delta);
    
    if (jiffy_flag) {
        return total_delta;
    }
    return usage;
}

float read_uptime() {

    float uptime;
    
    char uptime_path [] = "/proc/uptime";

    FILE *fp = fopen(uptime_path, "r");
    if (!fp) {
        perror("fopen /proc/stat error");
        exit(EXIT_FAILURE);
    }
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), fp)) {
        sscanf(buffer, "%f %*f", &uptime);
    }

    fclose(fp);

    return uptime;

}