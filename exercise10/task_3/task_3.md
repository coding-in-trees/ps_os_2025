```
[csbb9638@zid-gpl task_3]$ ./free_list_allocator_local bench
1 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 1.70 ms
        Custom: 22.42 ms
10 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.06 ms
        Custom: 1.94 ms
50 thread(s), 2000 allocations of size 256 - 1024 byte per thread. Avg time per thread:
        System: 2.16 ms
        Custom: 1.44 ms
100 thread(s), 2000 allocations of size 32 - 128 byte per thread. Avg time per thread:
        System: 1.35 ms
        Custom: 1.38 ms
```

The performance improved a lot over the previous version of the free_list_allocator. Its now faster than
the system allocator and caught up everywhere except for single threaded application. But also there the
performance increased drasticly