#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>

long get_total_memory_kb(void)
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return -1;

    long mem_kb = -1;
    char label[64];

    if (fscanf(fp, "%s %ld", label, &mem_kb) == 2) {
        printf("%s", label);
        fclose(fp);
        return mem_kb;
    }

    fclose(fp);
    return -1;
}