# Concurrency-Processing-Labs
A collection of C projects demonstrating concurrency processing, including Inter-Process Communication (IPC), process synchronization.

## Programs
### Inter-Process Communication
  1. Inter-Process Communication via Unnamed Pipes
  - **File:** `pipe8_2.c`
  - **Tech:** C (POSIX), `pipe`, `fork`, `wait`, `read`/`write`
  - **Description:** Implements a multi-process pipeline where three concurrent processes (Parent, Child 1, Child 2) pass text data through unidirectionally chained unnamed pipes. Data is transformed (converted to uppercase) across the processing pipeline.
  
  **Build and run:**
  ```
  gcc pipe8_2.c -o pipe_demo
  ./pipe_demo
  ```
---
  2. Named Pipes Communication (FIFOs)
  - **File:** `fifo8_3.c`
  - **Tech:** C (POSIX), `mkfifo`, `open`, `read`, `write`, `fork`
  - **Description:** Demonstrates independent process communication using named system pipes (`fifo1`, `fifo2`). Shows how separate processes read and write to persistent FIFO file descriptors in the filesystem.
    
  **Build and run:**
  ```
  gcc fifo8_3.c -o fifo_demo
  ./fifo_demo
  ```

---
### Producer-Consumer
Concurrency Synchronization & File Locking (Producer-Consumer)
- **File:** `template.c` 
- **Tech:** C, `fork`, File Locking (`flock`)
- **Description:** Implements the Producer-Consumer pattern using process concurrency. Uses exclusive advisory file locks (`flock(fd, LOCK_EX)`) on shared persistent data files to prevent race conditions during concurrent state updates.
    
**Build and run:**
```
gcc template.c -o producer_consumer -L. -lpqueue
LD_LIBRARY_PATH=. ./producer_consumer
```

