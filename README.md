# tune - TUI+Unix+Explore

### _A modern, extensible TUI-based process monitor for **Linux systems**, with advanced observability features like syscall, heatmaps, per-thread insights, and container awareness_

``` 
Inspired by htop, built from scratch using Go for TUI and C-based process APIs for direct collecting and parsing /proc and netlink syscalls
```

### System Architecture
**Core(engine)** -> C Language -> Fetch raw system data efficiently

**API Surface** -> C Language -> Expose structured data like get_cpu_info(), get_process_list(), etc

**TUI/CLI** -> Go Language -> Poll the API, render it interactively, and handle user input

