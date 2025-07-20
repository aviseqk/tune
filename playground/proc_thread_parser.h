#ifndef PROC_THREAD_PARSER_H
#define PROC_THREAD_PARSER_H
#include "data_structures.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

thread_info parse_thread_info(char pid[], char tid[]);

process_info parse_process_info(char pid[]);

#endif