#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

#include "data_structures.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <mntent.h>
#include <sys/statvfs.h>

#define MAX_DISKS_ALLOWED 4

bool get_relevant_disks(disk_system_info* disks[MAX_DISKS_ALLOWED], int* count);
uint64_t get_disk_size(const char disk_device[]);
battery_info get_battery_info();

bool is_real_fs(const char *type);
void print_fs_usage(const char *mount_point);
void read_filesystems();
swap_info read_swap_info();
memory_info get_mem_info();
void get_cpu_name(char *buffer, size_t bufsize);
float get_cpu_current_mhz();

#endif