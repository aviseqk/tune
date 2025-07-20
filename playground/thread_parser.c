#include "proc_thread_parser.h"

thread_info parse_thread_info(char pid[], char tid[]) {

    thread_info detail = {0};
    
    detail.thread_id = atoi(detail.thread_name); 

    /* get thread state and memory usage - /status */
    char status_path[256];
    snprintf(status_path, sizeof(status_path), "/proc/%s/task/%s/status", pid, tid);
    // printf("status_path - %s", status_path);
    FILE *fp = fopen(status_path, "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp))
        {   
            if (sscanf(line, "State: %c", &detail.thread_state) == 1) continue;
            if (sscanf(line, "VmRSS: %d", &detail.vm_res_mem) == 1) continue;
            if (sscanf(line, "VmSize: %d", &detail.vm_size) == 1) continue;
        }
        fclose(fp);
    }

    /* get scheduling info - /sched */
    char sched_path[256];
    snprintf(sched_path, sizeof(sched_path), "/proc/%s/task/%s/sched", pid, tid);
    // printf("sched_path - %s", sched_path);

    fp = fopen(sched_path, "r");
    if (fp) {
        char line[256];
        while(fgets(line, sizeof(line), fp)) {
            if (sscanf(line, " policy : %d", &detail.sched_policy) == 1) continue;
            if (sscanf(line, " prio : %d", &detail.sched_priority) == 1) continue;
        }
        fclose(fp);
    }

    /* get cpu usage - /stat */
    char stat_path[256];
    snprintf(stat_path, sizeof(stat_path), "/proc/%s/task/%s/stat", pid, tid);
    // printf("stat_path - %s", stat_path);

    fp = fopen(stat_path, "r");
    if (fp) {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), fp)) {
            unsigned long utime, stime;

            char *start = strchr(buffer, ')') + 2; // skip past )
            sscanf(start, "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %lu %lu", &utime, &stime); // removed l from some to suppress 
            detail.cpu_usage_secs = utime + stime;
        }

        // convert ticks to seconds
        long ticks_per_sec = sysconf(_SC_CLK_TCK);
        detail.cpu_usage_secs = detail.cpu_usage_secs / ticks_per_sec;
        fclose(fp);
    }

    /* get thread name - /comm */
    char comm_path[256];
    snprintf(comm_path, sizeof(comm_path), "/proc/%s/task/%s/comm", pid, tid);
    // printf("comm_path - %s", comm_path);
    fp = fopen(comm_path, "r");
    if (fp) {
        fgets(detail.thread_name, sizeof(detail.thread_name), fp);
        fclose(fp);
    }

    return detail;
}
