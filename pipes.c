
#include "main.h"

void pipe_func(char* buf[]) {


    char* temp_arr[10][10]; // temp_arr[command_index][arg_index]
    memset(temp_arr, 0, sizeof(temp_arr));
    int i = 0, m = 0, n = 0;

    // Parse buf[] into temp_arr[][]
    while (buf[i] != NULL) {
        if (strcmp(buf[i], "|") == 0) {
            temp_arr[m][n] = NULL;
            m++;
            n = 0;
        } else {
            temp_arr[m][n++] = buf[i];
        }
        i++;
    }
    temp_arr[m][n] = NULL;
    int cmd_count = m + 1;


    // Create all pipes
    int pipefd[cmd_count-1][2];
    for (int i = 0; i < cmd_count - 1; i++) {   //No.of pipes = no.of commands -1
        if (pipe(pipefd[i]) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pids[10];
    int status[10];

    for (int i = 0; i < cmd_count; i++) {
        exit_val=0;
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {          // Child Process
           
            // If  first command, set STDOUT
            if (i == 0) {
                dup2(pipefd[i][1], STDOUT_FILENO);
            }

            // If  last command, set STDIN
            else if (i == cmd_count - 1) {
                dup2(pipefd[i-1][0], STDIN_FILENO);
            }
            else{
                dup2(pipefd[i-1][0], STDIN_FILENO);
                dup2(pipefd[i][1], STDOUT_FILENO);
            }

            // Close all pipe fds
            for (int j = 0; j <cmd_count-1; j++) {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }


            ////////////////////////////////////
            if(execvp(temp_arr[i][0], temp_arr[i]) == -1){
                printf("%s:command not found\n",temp_arr[i][0]);
                exit(127);
            }
            exit(0);
        }
    }

    // Parent closes all pipe fds
    for (int j = 0; j <cmd_count-1; j++) {
        close(pipefd[j][0]);
        close(pipefd[j][1]);
    }

    
        //Exit value of a pipe-command - AS per POSIX standard, the exit status of a pipe command is based on the last executed command.
        pid_t xPid = waitpid(pids[cmd_count-1],&status[cmd_count-1], 0);
        if (xPid == -1) {
            perror("waitpid failed");
        }
        if(WIFEXITED(status[cmd_count-1])){
            exit_val= WEXITSTATUS(status[cmd_count-1]);
             
        }
        

}