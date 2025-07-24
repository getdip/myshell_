This is myshell program.

1. A prompt for the user to enter commands:

     a. Display the default prompt as myshell>
     b. Prompt should be customizable using environmental variable PS1
          • To change the prompt user will do PS1=NEW_PROMPT
          • Make sure that you do not allow whitespaces between '=' i.e., do'not do PS1 = NEW_PROMPT
          • In the above case, it should be treated like a normal command
     
2. Execute user commands:

     a. User will enter a command to execute
     b. If it is an external command
          • Create a child process and execute the command
          • Parent should wait for the child to complete
          • Only on completion, myshell> prompt should be displayed
     c. Shell builtin commands(internal commands) are implemented separately (cd,pwd,jobs,fg,bg)


3. Special Variables:

     a. Exit status of the last command (echo $?)
          • After executing a command the exit status should be available
          • echo $?: prints the exit status of the last command executed
     b. PID of myshell (echo $$)
          • echo $$:  prints myshell's PID
     c. Shell name (echo $SHELL)
          • echo $SHELL: prints myshell's executable path

4. Signal handling

     Provide shortcuts to send signals to running program
     1. Ctrl+C (Send SIGINT)
          On pressing Ctrl-C
          • If a programming is running in foreground, send SIGINT to the program (child process)
          • If no foreground program exists, re-display the 'myshell>' prompt
     2. Ctrl+z (Send SIGTSTP)
          • On pressing Ctrl+z, the program running in foreground, should stop the program and parent will display pid of child

5. Built-in commands

     a. exit: This command will terminate the msh program
     b. cd: Change directory
     c. pwd: show the current working directory
     d. jobs: this lists all the stopped processes
     e. fg: runs the last stopped process in foreground
     f. bg: runs the last stopped process in background

6. Background Process / Job control
     a. Allow a program to run in background, to run a program in background use ampersand (&) after the command. For eg: sleep 50 &
     b. Implement fg, bg and jobs commands
          • bg will will move a stopped process to background sleep 10 & is equivalents to sleep 10 then ctrl + z and bg
     After this the 'myshell>' prompt should be displayed indicating it is ready to accept further commands. After a bg process ends, cleanup the process using wait. 
     NOTE: SIGCHLD signal handler is used for this 
     
     On termination of bg process, display its exit status. User should be able to run any number of background processes. 
          • fg will bring a background process to foreground. 
     
     Only fg bring last background process, or fg will bring given pid to foreground
          • jobs will print all background process details.
          
7. Pipe functionality

     a. Allow multiple processes communication by using pipes.
     b. Create pipes and childs dynamically as per pipes passed from command-line
     Eg: ls | wc, ls -l /dev | grep tty | wc -l

Build Process

Requirements:
     a.