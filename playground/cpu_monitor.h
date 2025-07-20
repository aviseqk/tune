#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

// #include <stdint.h>
#include "data_structures.h"

int read_cpu_stats(cpu_stats *stats, bool total_cpu_flag); 

double compute_cpu_usage(cpu_stats prev, cpu_stats curr, bool jiffy_flag);

#endif