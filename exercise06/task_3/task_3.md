- What is the advantage of using a condition variable in this case, compared to using plain mutexes?

When using a condition variable a thread can sleep based on the condition. So there is no need for busy waiting. Which would be the case when using just mutexes wihtout a condition variable
- - -
- When would you use condition variables?

When the execution of a thread should be stopped or continued based on a condition. In other words when i would have to implement busy waiting then i would substitute for using condition variables.
- - -
- What are spurious wakeups in the context of condition variables, and how can they be mitigated?

When a thread wakes up without the condition variable changing. You can mitigate them by checking the variable in a loop in the thread.
- - -
- How does the program behavior differ between the two variants?

The variant in task 2 uses busy waiting in the treads so it constantly checks the queue for new items. Whereas the task 3 variant only wakes up based on the condition variable.