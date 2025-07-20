# tune - TUI+Unix+Explore

### _A modern, extensible TUI-based process monitor for **Linux systems**, with advanced observability features like syscall, heatmaps, per-thread insights, and container awareness_

!!! **IMPORTANT** Currently this is a single-threaded application that uses constant intervaled polling from TUI app to gather and display this data. Future plans are to outsource few activities to threads and then use some sort of IPC or pipe/sockets to get data from a daemon that is constantly capturing the system's current data and its delta too. This purely decouples the two operations. 

``` 
Inspired by htop, built from scratch using Go for TUI and C-based process APIs for direct collecting and parsing /proc and netlink syscalls
```

### System Architecture - v0.0
**Core(engine)** -> C Language -> Fetch raw system data efficiently and store in a data structure

**API Surface** -> C Language -> Expose structured data via callable methods like get_cpu_info(), get_process_list(), etc

**TUI/CLI** -> Go Language -> Poll the API, render it interactively, and handle user input

