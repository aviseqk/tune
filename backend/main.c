#include "include/data_structures.h"
#include "include/disk_monitor.h"

#include <stdio.h>


int main() {
    disk_system_info* myDisks[MAX_DISKS_ALLOWED]; 
    int count = 0;
    if (get_relevant_disks(myDisks, &count)) {
        printf("Success\n");
        for (int i = 0; i < count; i++) {
            printf("Found a device %s size-%lu\n", myDisks[i]->name, myDisks[i]->disk_size);
        }
    } else {
        printf("some error occured");
    }

    for (int i = 0; i < MAX_DISKS_ALLOWED; i++) {
        free(myDisks[i]);
    }

    battery_info info = get_battery_info();
    if (info.percentage == -1) { printf("No battery found in the machine\n");}
    printf("Battery: %d%%, Status: %s\n", info.percentage, info.state);

    printf("Mounted Filesystems:\n");
    read_filesystems();

    swap_info s_info = read_swap_info();
    if (s_info.free_gb == -1){
        printf("Unable to read Swap \n");
    }

    memory_info mem = get_mem_info();

    printf("Memory Stats:\n");
    printf("  Total:      %lu MB\n", mem.total_kb / 1024);
    printf("  Used:       %lu MB\n", mem.used_kb / 1024);
    printf("  Free:       %lu MB\n", mem.free_kb / 1024);
    printf("  Cached:     %lu MB\n", mem.cached_kb / 1024);
    printf("  Buffers:    %lu MB\n", mem.buffers_kb / 1024);
    printf("  Available:  %lu MB\n", mem.available_kb / 1024);

    char cpu[128];
    get_cpu_name(cpu, sizeof(cpu));
    printf("CPU (Name): %s\n", cpu);

    float mhz = get_cpu_current_mhz();
    if (mhz > 0)
        printf("Current CPU Frequency: %.2f MHz\n", mhz);
    

    return 0;
}