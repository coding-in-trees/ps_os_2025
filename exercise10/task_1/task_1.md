```
[csbb9638@zid-gpl task_1]$ ./free_list_allocator_global bench
1 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.08 ms
        Custom: 130.19 ms
10 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.34 ms
        Custom: 3.85 ms
50 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.10 ms
        Custom: 2.56 ms
100 thread(s), 2000 allocations of size 32 - 128 byte per thread. Avg time per thread:
        System: 1.43 ms
        Custom: 2.16 ms
```

You can clearly see that the system allocator outperforms the custom one in single threaded application. Then, the more
threads the more the custom allocator catches up. Maybe the performance is worse in single threaded application because maintaining the list is to much overhead.