* What happens in your program if two clients with the same name would join?
  Nothing
  If this would produce an error in your program, how could you mitigate it?
  I dont know
  _Note:_ You don't have to actually implement it.
* Why is it important that a message is at most `PIPE_BUF` long?
Because this ensures that the write of the data is atomic - meaning the kernel will write everything in "one go" where as if the data is bigger then PIPE_BUF other daata might be written by other processes and the kernel may interleave the rest of the data
* What file permissions did you use to create and open your FIFOs? Why?
- 0666 To allow writing from the fifo processes