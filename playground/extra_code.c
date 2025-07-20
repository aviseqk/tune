// const char *name = entry->d_name;
            // // printf("The path is %s\n", path);
            // int is_pid = 1;
            // // check if the directory is a pure number or not
            // for (int i = 0; name[i] != '\0'; i++) {
            //     if (!isdigit(name[i])) {
            //         is_pid = 0;
            //         break;
            //     }
            // }
            // printf("Struct data %s\n", result.process_name);
            // if (is_pid) {
            // printf("Found process with PID: %s\n", name);


// const char *dtype_to_str(unsigned char dtype) {
//     switch (dtype) {
//         case DT_REG: return "regular file";
//         case DT_DIR: return "directory";
//         case DT_LNK: return "symlink";
//         case DT_FIFO: return "FIFO";
//         case DT_SOCK: return "socket";
//         case DT_CHR: return "char device";
//         case DT_BLK: return "block device";
//         case DT_UNKNOWN: return "unknown";
//         default: return "other";
//     }
// }

//

    dp = opendir(path);
//     while ((entry = readdir(dp)))
//     {
//         const char *name = entry->d_name;

//         if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
//         {
//             // this is the thread folder
//             DIR *tp;
//             struct dirent *tp_entry;
//             char task_path[256];
//             int num_threads = 0;

//             sprintf(task_path, "/proc/%s/task", pid);
//             tp = opendir(task_path);
            
//             while ((tp_entry = readdir(tp)) != NULL) {
//                 if (tp_entry->d_type == DT_DIR && isdigit(tp_entry->d_name[0])){
//                     num_threads++;
//                 }
//             }// && strcmp(entry->d_name, "task") == 0) {
//             if (num_threads > 0) {
//                 detail.thread_count = num_threads;
//             }

//             closedir(tp);
//     }
// }

    // closedir(dp);