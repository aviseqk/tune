#include "hdr.h"
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include "data_structures.h"
#include "proc_thread_parser.h"
#include "manage_memory.h"
#include "cpu_monitor.h"

#define PATH_MAX 4096 // to ignore IDE error of variable unavailable from limits.h
#define PROCS_MAX 1024
#define CPUS_MAX 16
#define CPU_INTERVAL 500000

static int my_pid;


/* TODO mismatch or lag in count of threads from /proc/PID/status file reading
    consider counting subdirectories in path/task, as that would be
    more accurate way of counting no of active threads ( as in htop ) */
int get_num_threads_of_process(const char processID[])
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "/proc/%s/status", processID);
    FILE *fp;
    fp = fopen(path, "r");
    int n_threads = 0;
    char state;
    if (fp)
    {
        char line[256];
        while (fgets(line, sizeof(line), fp))
        {

            /* check if kernel thread */
            // if (check_if_kernel_threads(processID) == 1)
            // {
            //     // printf("Kernel Thread - Process[%s] no of Threads[%d]\n", processID, n_threads);
            //     return 0;
            // }
            /* check if its a zombie process */
            if (strncmp(line, "State:", 6) == 0)
            {
                sscanf(line, "State: %c", &state);
                if (state == 'Z')
                {
                    printf("Zombie Process [%s] found: threads - %c", processID, state);
                    n_threads = 0;
                    return 0;
                }
            }
            /* otherwise consider calculation */
            if (strncmp(line, "Threads:", 8) == 0)
            {
                sscanf(line, "Threads: %d", &n_threads);
            }
        }
    }
    fclose(fp);
    printf("Process ID [%s] No of Threads [%d]\n", processID, n_threads);
    return n_threads;
}

void populate_threads_for_process(process_info *proc)
{
    printf("[PID: %d] - fetching threads Info\n", proc->process_id);
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", proc->process_id);

    char path[512];
    char thread_path[512];
    DIR *dp;
    FILE *fp;
    struct dirent *entry;
    snprintf(path, sizeof(path), "/proc/%s/task", pid_str);

    dp = opendir(path);
    if (dp == NULL) {
        perror("opendir failed");
        return;
    }
    while ((entry = readdir(dp)))
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
        {
            char *name = entry->d_name;
            snprintf(thread_path, sizeof(thread_path), "/proc/%s/task/%s", pid_str, name);
            // printf("found thread dir: %s at path %s\n", name, thread_path);

            thread_info thread = parse_thread_info(pid_str, name);
            add_thread_to_process_memory(proc, thread);
            
            // display_thread_info(&thread);

        }
    }
    closedir(dp);

}



int populate_process_info(process_list *list, char pid[])
{
    char path[256];
    DIR *dp;
    FILE *fp;
    struct dirent *entry;
    sprintf(path, "/proc/%s/task", pid);

    // get details of one process
    process_info detail = parse_process_info(pid);
    populate_threads_for_process(&detail);
    add_process_memory(list, detail);

    return 0;
}

int get_all_processes_info(process_list *list)
{

    DIR *dp;
    struct dirent *entry;

    dp = opendir("/proc");
    if (!dp)
    {
        perror("opendir /proc");
        exit(EXIT_FAILURE);
    }
    int count = 0;

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]) && atoi(entry->d_name) != my_pid)
        {
            char *name = entry->d_name;
            // printf("New Process found [%s] in main %d - %d\n",name,  (int) list->capacity, (int) list->count);
                
            populate_process_info(list, name);

        }
    }

    closedir(dp);
    return 0;
}

int validate_threads_count(process_info *proc) {
    int currActiveThreads = 0;
    printf("Thread Population: %d ", (int) proc->thread_count);
        char path[256];
        DIR *dp;
        FILE *fp;
        struct dirent *entry;
        sprintf(path, "/proc/%s/task", proc->process_name);
    
        dp = opendir(path);
    
        while (entry = readdir(dp))
        {
            const char *name = entry->d_name;
    
            if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
            {
                printf("found thread dir: %s\t", entry->d_name);
                currActiveThreads++;
            }
        }
        printf("Thread Realisation: %d\n", currActiveThreads);
        if (currActiveThreads != proc->thread_count) {
            printf("PID: %d - Case of thread expiry mid-parsing\n", proc->process_id);
        }
        closedir(dp);

        return currActiveThreads;
}


void populate_cpu_data() {
    cpu_stats prev_cores[CPUS_MAX], curr_cores[CPUS_MAX];

    int num_cores = read_cpu_stats(prev_cores, false);

    printf("NUM CORES : %d\n", num_cores);

    usleep(CPU_INTERVAL);

    read_cpu_stats(curr_cores, false);

    cpu_display_info info[num_cores];
    for (int core = 0; core < num_cores; core++) {
        info[core].index = core;
        info[core].usage = compute_cpu_usage(prev_cores[core], curr_cores[core], false);
        // display_core_info(&info[core]);
    }

    display_cpu_info(info, num_cores);
}


// int main(int argc, char *argv[])
// {
//     populate_cpu_data();
//     my_pid = (int) getpid();
//     printf("aloha process' PID is %d", (int) getpid());
//     process_list list;
//     init_process_list(&list);

//     get_all_processes_info(&list);

//     printf("aloha Processes Count %d\n", (int) list.count);
    
//     for (int j = 0; j < list.count; j++) {
//         // display_process_info(&list.processes[j]);
//     }
//     free_process_list_memory(&list);

    
//     return 0;
// }

int main(int argc, char *argv[]) {
    cpu_stats prev, curr;
    // cpu_stats total_prev, total_curr;
    read_cpu_stats(&prev, true);
    usleep(1000000);
    read_cpu_stats(&curr, true);

    // double cpu_total = compute_cpu_usage(total_prev, total_curr, true);
    // printf("%f", cpu_total);
    // parse_process_info("1");


    return 0;
}

