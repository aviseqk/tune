#include "manage_memory.h"
#include "data_structures.h"

void add_process_memory(process_list* list, process_info proc) {
    // printf("in add_process_memory [PID %d] count - %d capacity - %d\n", proc.process_id, (int) list->count, (int) list->capacity);
    if (list->count == list->capacity) {
        size_t new_capacity = ( list->capacity == 0) ? 64 : list->capacity * 2;
        process_info* new_array = realloc(list->processes, new_capacity * sizeof(process_info));
        // printf("ALLOC PROCESS MEMORY for PID %d -> %p\n", proc.process_id, (void *)new_array);

        if (!new_array) {
            perror("realloc process list memory error");
            exit(EXIT_FAILURE);
        }

        list->processes = new_array;
        list->capacity = new_capacity;
    }

    list->processes[list->count++] = proc;  
    /* ISSUE - this proc is a local variable this is doing shallow copy meaning struct is copied by value and so is threads pointer
    and assigning like this only copies the pointer of thread array and not the actual memory it points to 
    So now:
    list->processes[i].threads and proc.threads both point to the same memory. But as proc is a local variable,
    so when the next iteration happens or proc goes out of scope, you lose access to the original threads array.*/
    // SOLUTION - change to a deep copy
    /* Deep Copying the threads array to my process_info*/
    /* COMMENT BELOW LOGIC IF YOU DONT WANT DEEP COPY and then no need for freeing call at the end too */

    // process_info *dest = &list->processes[list->count];
    // *dest = proc;

    // if (proc.thread_count > 0 && proc.threads != NULL) {
    //     dest->threads = malloc(proc.thread_count * sizeof(thread_info));
    //     if (!dest->threads) {
    //         perror("malloc failed for threads array");
    //         exit(EXIT_FAILURE);
    //     }

    //     memcpy(dest->threads, proc.threads, proc.thread_count * sizeof(thread_info));

    //     dest->thread_capacity = proc.thread_capacity;
    //     dest->thread_count = proc.thread_count;
    // } else {
    //     dest->threads = NULL;
    //     dest->thread_count = 0;
    //     dest->thread_capacity = 0;
    // }
    // list->count++;

    // free(proc.threads);
    // proc.threads = NULL;

}

void add_thread_to_process_memory(process_info *proc, thread_info tinfo) {
    // printf("\n\nin add_thread_memory [PID %d] count - %d capacity - %d\n\n", proc->process_id, (int) proc->thread_count, (int) proc->thread_capacity);
    if (proc->thread_count == proc->thread_capacity) {
        size_t new_capacity = (proc->thread_capacity == 0) ? 4 : proc->thread_capacity * 2;
        thread_info *new_threads = realloc(proc->threads, new_capacity * sizeof(thread_info));
        // printf("ALLOC THREADS @ %p (PID %d) new_capacity = %zu\n", (void*)new_threads, proc->process_id, new_capacity);

        if (!new_threads) {
            perror("realloc process list memory error");
            exit(EXIT_FAILURE);
        }

        proc->threads = new_threads;
        proc->thread_capacity = new_capacity;
    }

    proc->threads[proc->thread_count++] = tinfo;

    }

void free_process_list_memory(process_list *list) {
    // printf("Memory freeing process - count %d", (int) list->count);
    for (size_t i = 0; i < list->count; i++)
    {
        // printf("ALLOC FREEING threads for PID %d @ %p\n", list->processes[i].process_id, (void*)list->processes[i].threads);
        free(list->processes[i].threads);
        list->processes[i].threads = NULL;
    }
    // printf("ALLOC FREEING processes array @ %p\n", (void*)list->processes);
    free(list->processes);
}