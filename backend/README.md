### Layers
#### C Backend - Fast, Low Level Data Parsing - Data Acquisition Engine - optimize with threading
#### Go Aggregrator - applies logic/aggregates data - Aggregrator or Query Engine - intermediate service or module to talk to C functions and applies aggregations
-- allows future scaling like REST API Server, Exporting to Prometheus Format or even Web Dashboard
#### Go Frontend - UI, sorting, filtering, user interactions - complete Go TUI with interactivity - TUI Layer
#### Shared Memory or other IPC between two layers - flattened data to be passed from backend layer to other layers to parse and display - 

#### Threading Scope?
-- /proc/pid reading can be made independent and parallel
-- can add job queue and thread-pool pattern to reduce total parsing time on multi core systems
-- race conditions are minimal, except the shared memory writes or other IPC generated scenarios

### Flow of the design ( AI-Generated )
1. Backend (C):
Periodically (or on-demand), reads /proc, collects all process/thread data.
Flattens the data to a fixed format — e.g., flat array of process_info with embedded thread counts or thread summaries (if needed).
Writes to a mmap()-ed or shm_open()-ed region.
Uses a mutex/semaphore to lock while writing.

2. Frontend (Go Bubbletea):
Periodically reads the same shared memory region.
Waits for the mutex to be released, or uses sem_timedwait or sem_trywait.
Reads the flat data blob and transforms it into usable Go structs.
Applies dynamic sorting, filtering, and user interaction logic.

3. Shared Memory Buffer Format:
Flat struct: N entries, each fixed-size
Optional: 1st entry or header includes metadata like timestamp, count of records, etc.
Zero pointers. Only arrays, offsets, integers, fixed char buffers (e.g., char cmdline[256], etc.)

