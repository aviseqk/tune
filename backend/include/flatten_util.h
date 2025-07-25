#ifndef FLATTEN_UTILS_H
#define FLATTEN_UTILS_H
#include "data_structures.h"

void flatten_process_info(flat_process_info *output, const process_info *input);

void flatten_thread_info(thread_info *output, const thread_info *input);

#endif