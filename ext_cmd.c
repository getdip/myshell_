
#include "main.h"
int exit_val=0;
int stp_fl;
int term_fl;
int pipefd[2];
pid_t bg_jobs[15]={0};

int bg_job_cnt=0;

void ext_cmnd(char *buf[]) {

        int x;
        pid_t ret = fork();

    /* Child process */
        if(ret == 0){                
            
           setpgid(0,0);
            /*Running in foreground*/
            if(run_bg==0){ 
                /**
                    This is from the context of a child process, previously defined( in main- parent process) SIGINT & SIGTSTP 
                    was modified to redisplay the prompt.But here we need to define what action when ctrl+C(SIGINT) and ctrl+Z(SIGTSTP) are 
                    pressed from keyboard. SIGINT & SIGTSTP should have default behaviour. 
                */
                tcsetpgrp(tty_fd, getpid());  /*Terminal control goes from myshell process(parent) to the child process*/
                struct sigaction signal_fgnd;
                signal_fgnd.sa_handler = SIG_DFL;  /* SIGINT - Terminates the process , SIGTSTP - Stops the process*/
                sigemptyset(&signal_fgnd.sa_mask);
                sigaction(SIGTSTP,&signal_fgnd,NULL);
                sigaction(SIGINT,&signal_fgnd,NULL);
                
            }

            /* Running in background */
            if(execvp(buf[0],buf)==-1){
                perror("Invalid command");
                exit(127);
            }
            
        }

    /* Parent Process */
        else if(ret>0) {       
            setpgid(ret,ret);
            int status;
                
            if (run_bg==0){     /* Foreground process */
                 
                tcsetpgrp(tty_fd,ret);
           
                x = waitpid(ret,&status,WUNTRACED);
            
            if (WIFEXITED(status)) {
                exit_val=WEXITSTATUS(status);
                if(exit_val == 0) printf("Process %d terminated\n",ret);  /* Child terminates gracefully */   
                 
            }
            else if (WIFSIGNALED(status)){ 
                printf("Process %d has terminatd with a signal %d\n",ret,(exit_val=WTERMSIG(status)));  /* Child terminates with a signal */
                exit_val+=128;
            }
            else if (WIFSTOPPED(status)){
                insert_pid(ret);
                printf("\nPID:%d      [stopped]\n",ret);    /* Child is stopped */
                exit_val+=128;
            } 
            tcsetpgrp(tty_fd,shell_pgid);                   /* Terminal control comes backto myshell process */
            tcsetattr(tty_fd,TCSADRAIN,&shell_modes);
            } 

            else if(run_bg ==1){                            
                insert_pid(ret);                            /* Background jobs are also added into the job queue */
                bg_jobs[bg_job_cnt++]=ret;
                printf("[%d]%d\n",bg_job_cnt,ret);
            }
        }
}



