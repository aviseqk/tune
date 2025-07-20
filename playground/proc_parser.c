#include "proc_thread_parser.h"
#include "manage_memory.h"
#include "cpu_monitor.h"

void get_utime_stime(const char *stat_path, unsigned long *utime, unsigned long *stime) {
     FILE *fp = fopen(stat_path, "r");
    if (!fp) {
        perror("fopen");
        return;
    }

    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), fp)) {
        char *start = strchr(buffer, ')');
        if (start) {
            start += 2;  // Skip past ") "

            // Field 14 = utime, field 15 = stime (start from field 3)
            sscanf(start,
                "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %lu %lu",
                utime, stime);
        }
    }
    printf("%ld %ld\n", *utime, *stime);

    fclose(fp);
}

process_info parse_process_info(char pid[]) {

    process_info detail = { 0 };

    /* get status info - /status path */
    char status_path[256];
    sprintf(status_path, "/proc/%s/status", pid);
    FILE *fp = fopen(status_path, "r");
    
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp))
        {

        if (sscanf(line, "State: %c", &detail.process_state) == 1) continue;
        // printf("Process State: %c", detail.process_state);
        if (sscanf(line, "Pid: %d", &detail.process_id) == 1) continue;
        if (sscanf(line, "Kthread: %d", &detail.is_kernel_thread)) continue;
        if (sscanf(line, "VmSize: %f", &detail.vm_size) == 1) continue;
        if (sscanf(line, "VmRSS: %f", &detail.vm_res_mem) == 1) continue;
        }
    fclose(fp);

    }

    /* get cpu usage - /stat */
    char stat_path[256];
    snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", pid);

    unsigned long utime1, utime2, stime1, stime2;
    get_utime_stime(stat_path, &utime1, &stime1);
    usleep(1000000);
    get_utime_stime(stat_path, &utime2, &stime2);

    cpu_stats prev_cpu, curr_cpu;
    // TODO get the prev cpu stats and curr cpu stats from /stat file but using the cpu line
    // modify the read_cpu_stats function to have a flag that returns either cpu line data or cpu[core_no] cpu line
    read_cpu_stats(&prev_cpu, false);
    usleep(1000000);
    read_cpu_stats(&curr_cpu, false);
    double delta_total = compute_cpu_usage(prev_cpu, curr_cpu, false);

    long delta_thread = (utime2 + stime2) - (utime1 + stime1);
    printf("Delta thread %lu jiffies", delta_thread);
    // long delta_total = total_cpu_t2 - total_cpu_t1;

    long cpu_usage = (delta_thread / delta_total) * 100.0;
    printf("CPU Usage : %ld", cpu_usage);
    // convert ticks to seconds
       

    /* get sched info - /sched path */
    char sched_path[256];
    sprintf(sched_path, "/proc/%s/sched", pid);
    fp = fopen(sched_path, "r");

    if (fp) {
        char line[256];
        while(fgets(line, sizeof(line), fp)) {
            if (sscanf(line, " policy : %d", &detail.process_sched_policy) == 1) continue;
            if (sscanf(line, " prio : %d", &detail.process_sched_prio) == 1) continue;
        }
        fclose(fp);
    }
    
    return detail;
}
