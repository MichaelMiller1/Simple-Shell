# Simple-Shell  
Implement a simple shell that supports Linux shell commands in addition to the internal commands. Using C programming language. Your implementation must work on Linux machine. 
  
Required Modules: 
  
• Your shell should repeatedly display a prompt and allow the user to enter a command to run. Your shell is supposed to read the input from system standard input, parse the line with command and arguments, and execute. You must use fork() and exec*() system calls.  
    
• You are not allowed to use system(), as it invokes the system's /bin/sh shell. You cannot use execlp() or execvp(), because your shell has its own path variable (explained in part2).  
  
• By convention, the command arguments are seperated by white spaces. Please describe your customized argument seperation rules in your README document if you have special arrangement (not recommended though). Your shell does not need to handle special characters.  
  
• Implement the following three additional internal commands (running in the main process rather than the child process).
  
◦ cd: is a command, obviously, to change directories. You are required to use the chdir system call.  
  
◦ path: is not only a command to show the current command searching pathnames (if no argument is provided), but also a utility to modify (either add or remove) the command searching pathnames.  
  
◦ path (without arguments) displays the pathnames currently set. e.g., "/bin:/sbin". Initially, the pathnames are empty (so you have to manually use "path" command to construct your pathnames in the runtime. 
  
◦ path + /abc/def appends the pathname "/abc/def" to the "path" variable. e.g., "/bin:/sbin:/abc/def".  
  
◦ path - /abc/def removes the pathname "/abc/def" from the "path" variable. 
  
◦ quit: is a command to leave the shell.  

• Bonus if your shell to support several I/O redirection
