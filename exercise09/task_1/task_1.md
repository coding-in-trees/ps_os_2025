## Pthread mutex
```
[csbb9638@zid-gpl task_1]$ /usr/bin/time -v ./task_1 
Sum: 10000000
        Command being timed: "./task_1"
        User time (seconds): 5.10
        System time (seconds): 3.10
        Percent of CPU this job got: 182%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:04.49
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 25420
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 5632
        Voluntary context switches: 97623
        Involuntary context switches: 26
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```
## My mutex
```
[csbb9638@zid-gpl task_1]$ /usr/bin/time -v ./task_1 
Sum: 10000000
        Command being timed: "./task_1"
        User time (seconds): 3.22
        System time (seconds): 1.21
        Percent of CPU this job got: 195%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:02.27
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 40788
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 9475
        Voluntary context switches: 5
        Involuntary context switches: 8
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

## Findings
My mutex implementation uses less system time. Approx only half of the system time of the original example. 
This may be because i only use atomics for the implementation which take no extra time for the programm to spend in kernel mode.