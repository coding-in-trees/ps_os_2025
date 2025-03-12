# The provided Makefile
### What does it do?
It creates a zip file outside the directory where the make command was executed.
- zip
    - cleans the exercise folder
    - removes already created archives of the exercise
    - creates a new zip archive of the included exercise files
- clean
    - calls clean command of the makefiles in task subdirs
### How would you call it?
Just `make`.
### Where are you supposed to put it?
In the top level of an exercise directory
### Can you think of an additional use case for `make`?
Creating package related scripts. E.g for running the binary or converting output files to the target format.