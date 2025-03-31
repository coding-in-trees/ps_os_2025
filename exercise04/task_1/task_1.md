- How do you set/unset environment variables in your shell?
    - You can use export ... to set arguments for the terminal session.
    Or you write export ... in the shell config file to have them set for every terminal session.
    You could also call explicit commands with env variables by defining them before the call: ENV_VARIABLE=... ./your_script ...
    - To uset just run unset ENV_ARIABLE
- How can you retrieve the returned *exit code* in your shell after running the program?
    - By checking the ? env variable. Like echo $? after running the command.
- In your shell `;`, `&&`, and `||` can be used to execute multiple commands on a single line. What are the differences between these 3 operators?
    - `;` runs commands one after each other while ignoring the exit code of the previous command
    - `&&`runs the following command only if the previous succeded
    - `||` runs the following command only if the previous failed
- What is the `PATH` environment variable and why is it important?
    - tells the shell where to search for executable files if commands get typed in the shell. It's important to be able to execute commands without specifying the path where the executable is