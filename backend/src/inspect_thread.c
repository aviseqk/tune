// int check_if_kernel_threads(const char processID[])
// {
//     char cmdline_path[PATH_MAX];
//     snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%s/cmdline", processID);

//     FILE *cmd = fopen(cmdline_path, "r");

//     if (cmd)
//     {
//         int c = fgetc(cmd);

//         fclose(cmd);
//         if (c == EOF)
//         {
//             return 1;
//         }
//         else
//         {
//             return 0;
//         }
//     }
// }

// int get_thread_count_for_process(const char processID[])
// {
//     char path[PATH_MAX];
//     snprintf(path, sizeof(path), "/proc/%s/status", processID);
// }