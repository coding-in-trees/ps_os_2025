1. What do you notice when measuring the execution time of your solutions for Task 2 and Task 3a using `/usr/bin/time`?
Task 2 is a little faster because there is no synchronization
2. What is the difference between shared memory and a global variable after calling `fork()`?
For global variables the address space gets copied (changes do not affect other processes global variable)
With shared memory the address space is explicitly shared. So both processes can write on the same memory.
3. What is a race condition? Why do race conditions occur?
Unsynchronized access on the same target with modifying by multiple subjects where you cannot determine in what order they will access.

These occur for example when two processes try to access shared memory the same time. as in task_2
4. Is the output of a program with a race condition always incorrect?
No. It can happen that everything works fine

5. If you run your program 100 times and always get the correct result, does that mean that there is no race condition?
No its just that this 100 times nothing happend. But there could at the 101th time.

6. What is a critical section? What role does it play in preventing race conditions?
- Code that accesses shared resources
- These code parts need synchronization to work safely

7. What is synchronization?
concurrent operations can be processed in a safe, predictable order

8. Why are semaphores well suited for synchronizing such programs?
Because they are atomic and can only be accessed by one process at a time. Without any interference. It doesnt matter how many processes

9. Could you also just use *one* semaphore to ensure correct execution of this program? Is there a disadvantage when only using one semaphore?
Yes you could block consuming until production is finsihed. But this would screw the parallelization.

10. What would you need to do in order to support a *second* consumer process such that each consumer computes its own (local) sum and then the parent process computes the final sum after all the children have exited? Is your approach efficient?
I would need an extra result for every consumer. Also only call them with subsets of the produced data. And implement fitting synchro

11. There are two ways of creating a POSIX semaphore. Which would you use when?
I would only use the unnamed ones if only child processes need to communicate. The named ones are also suitable for inter-process communication where there are two seperate binarys.