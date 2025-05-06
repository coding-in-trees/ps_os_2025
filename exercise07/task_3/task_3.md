- How many threads do `task_3` and `task_3a` use, respectively?

3 uses as much threads as there are jobs and 3a uses a fixed number of threads for any
number of jobs
- - -
 - After you experimented with different thread pool sizes, which size results in the best performance? Why?

On zid i got the best results for a pool size arround 190 to 200.
- - -
- Report and interpret the results of your benchmarks. Which discoveries did you make?

At a thread rate of 200 the performance doesnt really increase further. But at first it
nearly increases linerarly
- - -
- What are the advantages of using a thread pool rather than spawning a new thread for each job?

No for extra system code for spawning the threads and joining them. Just the synchronization task.

- - -
- In which situations does it make sense to use a thread pool?
When you want optimal efficiency for working on many short task in parallel. While being able
to perfectly fit the thread pool size to the hardware youre using.
