#include "../include/disk_monitor.h"
#include "../include/data_structures.h"
#include <mntent.h>
#include <stdbool.h>
#include <string.h>
/**
    This program is to get data about the real mounted filesystems with the linux device (Disk Usage) and also 
    the Swap Memory (Swap Usage) with their usage and free percentage!
    We filter out the virtual or system filesystems and don't collect data for them
    so exclude tmpfs, proc, sysfs, etc filesystems
*/

bool get_relevant_disks(disk_system_info* disks[MAX_DISKS_ALLOWED], int* count) {
    char device_path[128];
    sprintf(device_path, "/sys/block");
    DIR* dp;
    FILE* fp;
    struct dirent* entry;

    dp = opendir(device_path);
    if (!dp) {
        perror("opendir /proc");
        return false;
    }
    int diskCount = 0;

    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}
        if (strncmp(entry->d_name, "loop", 4) == 0) {continue;}
        
        disks[diskCount] = malloc(sizeof(disk_system_info));
        if (!disks[diskCount]) {
            perror("malloc");
            closedir(dp);
            return false;
        }
        strncpy(disks[diskCount]->name, entry->d_name, 16);
        disks[diskCount]->name[15] = '\0'; // null-terminate
        
        printf("read a device %s\n", entry->d_name);
        unsigned long size = 0;
        size = get_disk_size(disks[diskCount]->name);
        disks[diskCount]->disk_size = size;
        printf("%lu check here\n", get_disk_size(disks[diskCount]->name));
        diskCount++;
        if (diskCount >= MAX_DISKS_ALLOWED) { break; }
    }

    printf("disk count %d", diskCount);
    *count = diskCount;
    // &disk_count = diskCount;

    closedir(dp);
    return true;
}

uint64_t get_disk_size(const char disk_device[]) {
    char path[128];
    snprintf(path, sizeof(path), "/sys/block/%s/size", disk_device);
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("error reading disk path");
        return 0;
    }

    unsigned long long sectors = 0;
    fscanf(fp, "%llu", &sectors);
    fclose(fp);
    
    return sectors * 512;
}

battery_info get_battery_info() {
    battery_info info = {-1, "unknown"};
    FILE *fp;
    char line[256];

    fp = popen("upower -i $(upower -e | grep BAT)", "r");
    if (fp == NULL) {
        perror("Failed to run upower, either upower missing or no battery found\n");
        return info;
    }
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "percentage:")) {
            // Trim and extract percentage
            char *percent_str = strstr(line, ":") + 1;
            while (*percent_str == ' ') percent_str++;
            info.percentage = atoi(percent_str);
        } else if (strstr(line, "state:")) {
            // Extract state (charging/discharging/etc)
            char *state_str = strstr(line, ":") + 1;
            while (*state_str == ' ') state_str++;
            strncpy(info.state, state_str, sizeof(info.state) - 1);
            // Remove newline
            info.state[strcspn(info.state, "\n")] = 0;
        }
    }

    pclose(fp);
    return info;
}

// Exclude non mounted file systems
bool is_real_fs(const char *type) {
    const char *skip[] = {
        "proc", "sysfs", "tmpfs", "devtmpfs", "devpts", "cgroup", "overlay",
        "squashfs", "autofs", "mqueue", "debugfs", NULL
    };

    for (int i = 0; skip[i]; i++) {
        if (strcmp(type, skip[i]) == 0) {
            return false;
        }
    }
    return true;
}

void print_fs_usage(const char *mount_point) {
    struct statvfs fs;
    if (statvfs(mount_point, &fs) == 0) {
        unsigned long total = fs.f_blocks * fs.f_frsize;
        unsigned long free  = fs.f_bfree  * fs.f_frsize;
        unsigned long used  = total - free;

        double gb = 1024.0 * 1024.0 * 1024.0;
        printf("Mount: %-20s Used: %6.2f GB  Free: %6.2f GB  Total: %6.2f GB\n",
               mount_point, used / gb, free / gb, total / gb);
    }
}

void read_filesystems() {
    FILE *fp = setmntent("/proc/mounts", "r");
    if (!fp) {
        perror("Failed to read from /proc/mounts");
        return;
    }

    struct mntent *ent;
    while((ent=getmntent(fp)) != NULL) {
        if (is_real_fs(ent->mnt_type)) {
            print_fs_usage(ent->mnt_dir);
        }
    }
}

swap_info read_swap_info() {
    swap_info info = { -1, -1, -1};
    FILE *fp = fopen("/proc/swaps", "r");
    if (!fp) {
        perror("Failed to open /proc/swaps");
        return info;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // skip header

    printf("\nSwap Partitions:\n");
    while (fgets(line, sizeof(line), fp)) {
        char name[64];
        char type[16];
        long size, used;
        sscanf(line, "%s %s %ld %ld", name, type, &size, &used);

        double gb = 1024.0 * 1024.0;
        info.free_gb = size / gb;
        info.total_gb = size / gb;
        info.used_gb = (size - used) / gb;

        printf("Swap: %-20s Used: %6.2f GB  Total: %6.2f GB\n",
               name, used / gb, size / gb);
    }

    fclose(fp);
    return info;
}

memory_info get_mem_info() {
    memory_info info = {0};
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("Failed to open /proc/meminfo");
        return info;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu kB", &info.total_kb) == 1) continue;
        if (sscanf(line, "MemFree: %lu kB", &info.free_kb) == 1) continue;
        if (sscanf(line, "MemAvailable: %lu kB", &info.available_kb) == 1) continue;
        if (sscanf(line, "Buffers: %lu kB", &info.buffers_kb) == 1) continue;
        if (sscanf(line, "Cached: %lu kB", &info.cached_kb) == 1) continue;
    }

    fclose(fp);

    // Used = Total - Free - Buffers - Cached (like btop and free)
    info.used_kb = info.total_kb - info.free_kb - info.buffers_kb - info.cached_kb;

    return info;
}

void get_cpu_name(char *buffer, size_t bufsize) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("Failed to open /proc/cpuinfo");
        snprintf(buffer, bufsize, "Unknown CPU");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *colon = strchr(line, ':');
            if (colon) {
                snprintf(buffer, bufsize, "%s", colon + 2);
                buffer[strcspn(buffer, "\n")] = 0;
                
                char *p;
                // Very basic cleanup like btop does
                while ((p = strstr(buffer, "(TM)"))) memmove(p, p + 4, strlen(p + 4) + 1);
                while ((p = strstr(buffer, "(R)"))) memmove(p, p + 3, strlen(p + 3) + 1);
                while ((p = strstr(buffer, "CPU"))) memmove(p, p + 3, strlen(p + 3) + 1);
                while ((p = strstr(buffer, "  "))) memmove(p, p + 1, strlen(p + 1) + 1);
            }
            break;
        }
    }

    fclose(fp);
}

float get_cpu_current_mhz() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("Failed to open /proc/cpuinfo");
        return -1.0;
    }

    char line[256];
    float mhz = -1.0f;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "cpu MHz : %f", &mhz) == 1) {
            break; // First core's MHz
        }
    }

    fclose(fp);
    return mhz;
}
