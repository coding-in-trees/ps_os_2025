# Is the order of the messages consistent?
Yes it is consistent following the time a child process took to execute. So its not always the same process numbers but sorted by time.
# Can the order of these messages be predicted?
No it cant be predicted percisely what process takes what amount of time before executing the process
# What does it depend on?
Calculation time of the DR_p function and operating system internal queing
# Does it matter wether you do srand(getpid()) befor or after the call to fork()?
Yes it does:
- calling outside: getpid() is pid of the main programm. This would be as like all childs would get the same dice.
- calling inside: getpid() is unique to every process. This would be as every child uses its own n-sided dice.
