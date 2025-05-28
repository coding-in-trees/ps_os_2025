```
[csbb9638@zid-gpl task_2]$ ./best_fit_allocator_global bench
1 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.38 ms
        Custom: 1.44 ms
10 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.26 ms
        Custom: 27.54 ms
50 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.13 ms
        Custom: 107.73 ms
100 thread(s), 2000 allocations of size 32 - 128 byte per thread. Avg time per thread:
        System: 1.54 ms
        Custom: 154.44 ms
```

Here the custom allocator is faster on a single single thread but looses much when working on multiple threads. This is due to the bad search complexity combined with many mutex locks and unlocks.