# Task 2
## Commands
- `ssh`: Used to log in on a remote machine via the ssl protocol. Use `-l` to supply a login name.
the command then opens a remote terminal shell session on the remote machine
### Looking around
- `pwd`: Prints the current working directory -> the place where youre at in your shell right now
- `ls`: Lists all document at a location. If no path is provided it prints the files at the current directory
- `cd`: Changes the working directory to the specified path. If no path is specified or `~` -> changes to $HOME directory. Use `..` to navigate a folder "out".
- `cat`: Prints the contents of the file specified as the first parameter to the std out
- `history`: Lists all previously used commands in the terminal session
### Creating Files
- `touch`: Creates a new File at the path specified e.g `path/to/new/file.txt`
- `mkdir`: Creates a new directory at a path specified e.g `path/to/new/folder`
### Modifying Files
- `cp`: Copies files from a source path to a destination path
- `mv`: Moves files from a source path to a destination path
- `rm`: Removes Files at a Location. To remove non empty folders use `rm -rf`
- `vi`/`vim`: Terminal code editor. It works in 3 modes: `insert`, `normal` and `comand-line`. The insert mode is used to input text to the file. In the normal mode
you can use shortcuts to move arround or edit something e.g `j` or `A`. The command mode allows you to execute commands respecitve to the whole file. Use `:wq` to write an quit the editor. To open files just type `vi path/to/file.txt`
### Grep
The grep command is a mighty tool to search/filter the output of a command or the contents of a file. You can specify a search string which can also be a regular expression to filter the output. E.g `ls | grep "welt"` highlights all occurences of "world" in the output of `ls`
### Command Operators
- `>`: Used to redirect the output of a command to a different stream e.g a file. So you could just write the output of a `ls` into a file
- `|`: The pipe operator is used to chain commands. This means executing them on after another. Often used with [grep](#Grep).
### Documentation
- `man`: View manual pages of a command. Its the paging application to navigate to manuals written for the man command. See [man pages](#man-pages) for more information.
- `help`: Get helping information to builtin shell commands. These are the commands in the shell binary
- `info`: Get the info page of a command. These provide often more detailed info than the man pages
- `apropos`: Tool to search all man pages. You can specify multiple or just one keyword to search for. By default all man pages are listed that contain the keywords. But there are options to customize the command output.
- `--help`: By convention all shell commands which are built to be commonly used should accept a `--help´ parameter. When called with this parameter documention for the command show up. Use this if you dont know that much about a command. Maybe even if a man page exists. Its a good entrypoint to get to know what a command does.
## Man Pages
### intro
The traditional way to interact with unix like operating systems like linux is the CLI. To open a new terminal session you
normaly must authenticate yourself. Thats commonly achieved by using the `login` command.
After you login you can now access the **shell**. A shell is the comand line interpreter you use to run you commands. There are different shells you could use. They are also just normal programms running on the machine.
The "conversation" with a shell is called **session** and it follows a simple schema. The shell shows with a command prompt-it
normaly $ that it's ready for input. Then it processes the input and shows the promt-it again if ready for new input.
The Operating system consists of a root filesystem which is organised in files and folders(directories). If e.g a disk is connected to the operating system then you can mount another filesystem(the disks filesystem) into youre root filesystem.
A global location can always be described with an absolute **path**. With paths you can percisely describe every location out of your operating system.
Also a terminal session always has a **current working directory** - the context it lives in.

To get information use the `man` command. This provides acces to the whole user manual for the commands on the machine.
### man-pages
The articles which document the user-space Api of the linux kernel and the GNU C library. The `man-pages` article documents what conventions to follow when writing new articles and describes the structure of the whole project. Including information on the different [sections](#sections) and the sections within a manual page such as:
- NAME
- DESCRIPTION
- SEE ALSO
and many others. There is also a whole style guide for writing the articles.
#### Sections
Are used to categorize the different man pages available on the machine there are categories for (1) User commands, (2) System calls,... and many more. In total 8 categories. When using `man` you can specifiy the section you want to open the command from. Thats why commands are often refered with their corresponding section e.g mount(8).
### file-hierarchy
Describes the file structure of the linux operating system. The directory structure starts at `/` the root path. There are directories for:
- `boot`: Contains the boot partition
- `efi`: Contains the efi system partition. Uses boot as fallback
- `etc`: System specific configuration
- `home`: Contains the users home directories
- `root`: Home directory of the root user
- `srv`: Stores general server payload
- `tmp`: Place for small temporary files
- - -
- `run`: Stores runtime data
- `usr`: Holds libraries and scripts installed by the user. And also documentation for these.
- `var`: Persistent but variable system data
- `dev`: Stores device nodes
- `proc`: Exposes the kernel file system
- `sys`: Exposes kernel file system for discovered devices 
- - -
- `bin`: Legacy path of `usr/bin`
- `lib`: Legacy path of `usr/lib`
