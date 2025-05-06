- Explain the program.

1. Creates N Threads. And N chopsticks
2. Then every thread tries to pickup the chopstick (lock the mutex) on its right first and then the one on its left
3. After picking up two chopsticks the tread should sleep for EAT_DURATION and then unlock the chopsticks again
4. After finishing for a Philosopher it prints that the philosopher with id i is done eating

- How can a deadlock occur? Does it always happen?

It occurs when every thread of a programm circularly waits for the same condition to be true but the condition is depending on the actions of on of the threads.
In this case the deadlock happens because all threads lock the mutex on their right. Because there are excatly the same amount of mutexes and threads all mutexes get locked. Then every thread wants to lock another mutex on their left. But all mutexes are locked so all threads are indefinitly waiting for a mutex to unlock

In this code it happens always

- Change the program to prevent any potential deadlocks from occurring. Submit your solution. Explain how your changes prevent deadlocks.

I fixed the problem by changing the behaviour of one thread to pick up / lock the mutex on the opposite site as all the other threads do. This allows the following thread to lock both its mutexes and the circular wait is broken.

- How can you test your solution to ensure that it is deadlock-free?

1. Run stress testing with high iterations
2. Use debuggers like helgrind
3. Use console logging to check what happens exaclty when
4. Perform a formal analysis of the programm code. But this is very time consuming and resource expensive

- Is it possible with this test to miss deadlocks?

You cant be 100% sure just by testing wiht helgrind for example. To verify you would have to perform a formal analysis but this is way to time consuming and expensive for the most cases

- What are the advantages and disadvantages of your solution?

The code still contains many mutexes. But the explicit change i made has not really disadvantages on its own. You could rewrite the whole code to use only two mutexes and one semaphore. This would be more efficient

- Does your solution need additional synchronization?

No
