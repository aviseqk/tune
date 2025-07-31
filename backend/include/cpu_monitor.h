#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// #include <stdint.h>
#include "data_structures.h"

int read_cpu_stats(cpu_stats *stats, bool total_cpu_flag); 
CPUStatsArray get_cpu_stats_array();

double compute_cpu_usage(cpu_stats prev, cpu_stats curr, bool jiffy_flag);
float compute_cpu_usage1(CPUStat prev, CPUStat curr, bool jiffy_flag);

void populate_cores_cpu_stats(cpu_stats *prev_cores, cpu_stats *curr_cores);

int read_process_cpu_stats(cpu_stats *stats, char pid[], bool total_cpu_flag);

float read_uptime();

#endif