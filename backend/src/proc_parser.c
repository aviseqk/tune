#include "../include/data_structures.h"
#include "../include/proc_parser.h"
#include "../include/cpu_monitor.h"
#include "../include/manage_memory.h"
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CPU_COUNT 4
#define DELTA_TIME 100000

static int clk_tck;
static int my_pid;

typedef struct vm_mem_data {
    float VmSize;
    float VmRSS;
    bool isValid;
} vm_mem_data;

/** ENTRYPOINT FUNCTION
* A pure function that parses all the processes and returns a malloc'ed list of process_info struct */
process_info* parse_all_processes(int* count, bool zombie_pid_filter, bool kthread_pid_filter, bool log) {

    my_pid = (int) getpid();
    clk_tck = sysconf(_SC_CLK_TCK);
    DIR *dp;
    struct dirent *entry;

    dp = opendir("/proc");
    if (!dp)
    {
        perror("opendir /proc");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]) && (atoi(entry->d_name) != my_pid))
        {
            char *this_pid = entry->d_name;

            process_info process_detail = parse_process_info(this_pid, false, false);
            if (zombie_pid_filter && process_detail.process_state == 'Z') {
                log && printf("[LOG] - ignoring process %d: is a zombie process\n", process_detail.process_id);
                continue;
            }
            if (kthread_pid_filter && process_detail.is_kernel_thread) {
                log && printf("[LOG] - ignoring process %d: is a kernel thread\n", process_detail.process_id);
                continue;
            }
            
            (*count)++; // increment the count for total process counter
            
            traverse_threads_for_a_process(this_pid, &process_detail);
            free(process_detail.threads);       // malloc security freeing
        }
    }
    printf("Number of processes found %d\n", *count);
    closedir(dp);

}

/* get a process cpu usage - /proc/[pid]/stat */
float get_process_cpu_usage(char pid[]) {
    // char stat_path[256];
    // snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", pid);

    unsigned long utime1, utime2, stime1, stime2;

    // NOTE need uptime only when want to show the total CPU usage by the process since startup - like uptime1 = read_uptime();
    get_utime_stime(pid, &utime1, &stime1);
    usleep(DELTA_TIME);
    get_utime_stime(pid, &utime2, &stime2);

    float proc_time_delta_jiffies = (utime2 + stime2) - (utime1 + stime1);

    long clk_tck = sysconf(_SC_CLK_TCK);
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);

    float proc_time_delta_seconds = proc_time_delta_jiffies / (double)clk_tck;
    float cpu_usage = (proc_time_delta_seconds / (DELTA_TIME * nprocs)) * 100;

    return cpu_usage;
    // long cpu_usage = (delta_thread / delta_total) * 100.0;
}

vm_mem_data get_process_vm_value_kb(const char pid[]) {
    char path[64], line[256];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    vm_mem_data res;

    FILE *fp = fopen(path, "r");
    if (!fp){
        res.isValid = false;
        res.VmRSS = 0;
        res.VmSize = 0;
        return res;
    }

    while(fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "VmSize: %f", &res.VmSize) == 1) continue;
        if (sscanf(line, "VmRSS: %f", &res.VmRSS) == 1) continue;
    }

    fclose(fp);
    return res;
}

void get_utime_stime(char pid[], unsigned long *utime, unsigned long *stime) {

    char stat_path[256];
    snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", pid);

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
                "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %lu %lu", utime, stime);
        }
    }
    // printf("%ld %ld\n", *utime, *stime);

    fclose(fp);
}

int count_processes(ProcFilerFlags flags) {
    DIR *dp = opendir("/proc");
    if (!dp) {
        perror("can't open /proc to count processes\n");
        return -1;
    }

    int count = 0;
    struct dirent* entry;
    while((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            if (is_zombie(entry->d_name) && (flags && PROC_EXCLUDE_ZOMBIE)) continue;
            if (is_kernel_thread(entry->d_name) && (flags && PROC_EXCLUDE_KERNEL)) continue;
            count++;
        }
    }

    closedir(dp);
    return count;
}

process_info parse_process_info(char pid[], bool cpu_filter, bool mem_filter) {

    process_info detail = { 0 };
    bool is_process_currently_zombie = false;

    /* get status info - /status path */
    char status_path[256];
    snprintf(status_path, sizeof(status_path), "/proc/%s/status", pid);
    FILE *fp = fopen(status_path, "r");
    
    if (fp) {
        char line[1024];
        while (fgets(line, sizeof(line), fp)) {
            // printf("checks before line iter start %s %d\n", line, (int) detail.thread_count);
            if (sscanf(line, "Name: %s", detail.process_name) == 1) continue;
            if (sscanf(line, "State: %c", &detail.process_state) == 1) continue;
            if (sscanf(line, "PPid: %d", &detail.parent_pid) == 1) continue;
            if (sscanf(line, "Pid: %d", &detail.process_id) == 1) continue;
            if (sscanf(line, "Kthread: %d", &detail.is_kernel_thread)) continue;
            if (sscanf(line, "VmSize: %f", &detail.vm_size) == 1) continue;
            if (sscanf(line, "VmRSS: %f", &detail.vm_res_mem) == 1) continue;
            // printf("checks after line iter end  %s %d\n", line, (int) detail.thread_count);
        }
    fclose(fp);

    }

    unsigned long utime, stime, process_uptime;
    get_utime_stime(pid, &utime, &stime);
    clk_tck = sysconf(_SC_CLK_TCK); // TODO remove later
    assert(clk_tck != 0);
    process_uptime  = (utime + stime) / clk_tck;
    detail.uptime = process_uptime;
    // TODO add a util function that changes the format of the abbove time that is in seconds to minutes 

    if (cpu_filter) {
        detail.cpuUsage = get_process_cpu_usage(pid);
        // printf("CPU Usage : %.2f\n", detail.cpuUsage);
    }
    if (mem_filter) {
        vm_mem_data mem_data = get_process_vm_value_kb(pid);
        detail.vm_size = mem_data.VmSize;
        detail.vm_res_mem = mem_data.VmRSS;
    }
    if (detail.process_state == 'Z') {
        is_process_currently_zombie = true;
    }
    

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

    /* get cmdline info - /cmdline path */
    // NOTE - IMP - keep this the last function 
    char cmdline_path[256];
    sprintf(cmdline_path, "/proc/%s/cmdline", pid);
    FILE *fcp = fopen(cmdline_path, "r");

    if (fcp == NULL) {
        printf("cmdline not available for process %d %d\n", detail.process_id, detail.is_kernel_thread);
    } else {
        if (!detail.is_kernel_thread || !is_process_currently_zombie) {
            char buffer[1024];
            while(fgets(buffer, sizeof(buffer), fcp)) {
                strncpy(detail.command, buffer, sizeof(buffer));
                detail.command[sizeof(detail.command) - 1] = '\0';
            }
        } else {
            printf("[LOG] - ignoring process %d: is a zombie or kthread\n", detail.process_id);
        }
        fclose(fcp);        
    }
    // display_process_info(&detail);
    return detail;
}

int count_threads_of_process(char pid[]) {
    char thread_path[512];
    DIR *dp;
    struct dirent* entry;
    int count = 0;

    snprintf(thread_path, sizeof(thread_path), "/proc/%s/task", pid);
    dp = opendir(thread_path);
    if (dp == NULL) {
        perror("can't open /proc/pid/task to count threads\n");
        return -1;
    }

    while((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {count++;}
    }
    closedir(dp);
    return count;
}

bool is_zombie(char pid[]) {
    char path[64], state;
    snprintf(path, sizeof(path), "/proc/%s/stat", pid);
    FILE *f = fopen(path, "r");
    if (!f) return false;

    int dummy;
    char comm[256];
    fscanf(f, "%d %s %c", &dummy, comm, &state);
    fclose(f);
    return (state == 'Z');
}

int is_kernel_thread(char pid[]) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/cmdline", pid);

    FILE *f = fopen(path, "r");
    if (!f) return false;

    int c = fgetc(f);
    fclose(f);

    return (c == EOF); // Empty means it's a kernel thread
}

/* Dynamically MALLOC's data, be sure to free the `threads` field of declared process_info struct */
void traverse_threads_for_a_process(char pid[], process_info *process_detail) {
    char thread_dir_path[512];
    char thread_path[512];
    DIR *dp;
    struct dirent *entry;
    
    snprintf(thread_dir_path, sizeof(thread_dir_path), "/proc/%s/task", pid);

    dp = opendir(thread_dir_path);
    if (dp == NULL) {
        perror("opendir for thread path failed");
        return;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            char *tid = entry->d_name;
            snprintf(thread_path, sizeof(thread_path), "/proc/%s/task/%s", pid, tid);
            // printf("-------NEW_THREAD--------\n");

            thread_info thread_detail = parse_thread_info(pid, tid);
            // display_thread_info(&thread_detail);
            display_thread_info(&thread_detail);
            // assert(process_detail->thread_count <= process_detail->thread_capacity);
            add_thread_to_process_memory(process_detail, thread_detail);

        }
    }


    closedir(dp);
}


/* A pure function that parses all the threads inside of a process and returns a malloc'ed list of thread_info struct */
// thread_info* parse_threads_for_process(pid_t pid, int* count) {
    
// }

thread_info parse_thread_info(char pid[], char tid[]) {

    thread_info detail = {0};
    
    detail.thread_id = atoi(tid); 

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