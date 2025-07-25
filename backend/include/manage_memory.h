#ifndef MANAGE_MEMORY_H
#define MANAGE_MEMORY_H
#include "data_structures.h"

#include <stdlib.h>
#include <string.h>

void add_process_memory(process_list* list, process_info proc);

void add_thread_to_process_memory(process_info *proc, thread_info tinfo);

void free_process_list_memory(process_list *list);

#endif