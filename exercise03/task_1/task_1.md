- What difference do you notice between `SIGINT` and `SIGKILL`?
SIGKILL cannot be handled differently and will always kill the process. Whereas SIGINT stands for just interupting the process (keyboard).
- What can the signals `SIGSTOP` and `SIGCONT` be used for?
To stop and resume/continue a process again.
- What happens when you press `CTRL + c` while the program is running (as a foreground process)?
Sends a SIGINT to the process.
- What happens when you press `CTRL + z` while the program is running (as a foreground process)?
It is stoped and moved to the background (SIGSTOP)
- Hypothetically, what would happen in your program if another signal arrives while your signal handler is still executing? Does it make a difference if it is the same signal or a different one?
Nothing different will happen because the signals get executed after the current handler finished