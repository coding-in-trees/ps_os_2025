1. Optimization flags:

- What is the effect of specifying optimization flags when compiling?

Flags tell the compiler how to compile the code. The optimization can make the programm faster or smaller. It tells the compiler how to simplfy the code.
- - -
- How do the flags `-O2` and `-O3` differ?

O2 enables safe performance improvements without bloating the binary size

O3 is based on O2 optimization and uses aggressive optimizations to speed up the code. Can be more unsafe and less predictable in some situations.
- - -
2. Comparing program behavior of the two variants:

- First, create optimized builds by using either `-O2` or `-O3`.
- Measure the execution time of both variants using `/usr/bin/time -v`.

- What is the difference between `elapsed (wall clock) time`, `user time`, and `system time`?

- Wall clock: real world time
- User: time for running just the programms code (without system/kernel calls)
- System: time spent in system/kernel while running the programm
- - -

- What conclusions can you draw from inspecting your measurements?

Measurements O3:
- atomic:
        Command being timed: "./task_1_atomic"
        User time (seconds): 7.86
        System time (seconds): 0.04
        Percent of CPU this job got: 1166%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.67
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 5168
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1138
        Voluntary context switches: 1323
        Involuntary context switches: 1375
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
- mutex:
        Command being timed: "./task_1_mutex"
        User time (seconds): 5.20
        System time (seconds): 32.92
        Percent of CPU this job got: 1120%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:03.40
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 5576
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 1
        Minor (reclaiming a frame) page faults: 1105
        Voluntary context switches: 508696
        Involuntary context switches: 21531
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0

You clearly see that the atomics variant is faster and uses less system time.
- - -
3. Atomic operations:

- What is the difference between using `+=` on a normal vs. an *atomic* integer type?

On a normal int this creates a race condition when done in multiple threads. When using atomics the += translates into atomic_fetch_add
- - -
- How does using e.g. `+=` on atomic types relate to using atomic functions such as `atomic_fetch_add`?

+= can be considered as syntactic sugar to work with atomics
- - -
- Which operations other than decrementing/incrementing can be done atomically?

load, store, swap, compare and swap, bitwise operations