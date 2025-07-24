#include "main.h"

int shell_terminal;
struct termios shell_modes;
pid_t shell_pgid;
int tty_fd;
FILE* tty;
volatile sig_atomic_t redisplay_prompt=0;

char shell_path[1024]={0};

void handler_SIGCHLD(int sig){
    //printf("SIGCHLD is generated\n");
    int saved_errono = errno;
    int status_bg;
    pid_t p;
    while ((p = waitpid(-1, &status_bg, WNOHANG)) > 0){
        for(int i=0;i<bg_job_cnt;i++){
            if(p==bg_jobs[i]){
                if(WIFEXITED(status_bg) && (run_bg==1)) exit_val=WEXITSTATUS(status_bg);
            }
        }

        if (j_head && p == j_head->pid){
            //printf("Job ID checking\n");
            if(WIFEXITED(status_bg) || WIFSIGNALED(status_bg)){
                remove_pid();
            }
            break;
        }
    }
    errno = saved_errono;
}


void return_prompt(int sig){

    redisplay_prompt=1;
}


int main(){

    //Get the path for the shell process into a  buffer
    if(getcwd(shell_path,sizeof(shell_path))!=NULL);

    //Configure the parameters for SIGCHLD handler
    struct sigaction tr_chld;
    tr_chld.sa_handler = handler_SIGCHLD;
    sigemptyset(&tr_chld.sa_mask);
    tr_chld.sa_flags=SA_RESTART | SA_NOCLDSTOP;

    //Signal handling for SIGCHLD signal (whenever any child terminates)
    sigaction(SIGCHLD,&tr_chld,NULL);

    //Configure the parameters for SIGTTIN and SIGTTOU handler (Ignore the signals)
    struct sigaction signal_IGN;
    signal_IGN.sa_handler = SIG_IGN;
    sigemptyset(&signal_IGN.sa_mask);

    //Signal handling for SIGTTIN and SIGTTOU handler (terminal I/O control)
    sigaction(SIGTTIN,&signal_IGN,NULL);
    sigaction(SIGTTOU,&signal_IGN,NULL);

    //Configure the parameters for SIGINT and SIGTSTP handler
    struct sigaction signal_return;
    signal_return.sa_handler= return_prompt;
    sigemptyset(&signal_return.sa_mask);
    signal_return.sa_flags=0;

    //Signal handling for SIGINT and SIGTSTP handler (SIGINT - ctrl + c [from keyboard], SIGTSTP - ctrl + z [from keyboard])
    sigaction(SIGINT,&signal_return,NULL);
    sigaction(SIGTSTP,&signal_return,NULL);

    
    //Reading input from terminal
    tty_fd = open("/dev/tty", O_RDWR);
    FILE *tty = fdopen(tty_fd,"r+");

    if (tty_fd < 0) {
        perror("open /dev/tty");
        exit(1);
    }

    char prompt[20]="minishell$";   //Default prompt
    char input[1024];
    short int cmd_type;
    char* cmd_buf[20];

    shell_pgid = getpid();
    setpgid(shell_pgid,shell_pgid);
    tcsetpgrp(tty_fd,shell_pgid);
    tcgetattr(tty_fd,&shell_modes);


while(1){                               //Super Loop for the Prompt

    //Resets
    pipe_cnt=0;                         //Pipe count
    errno=0;

    //Redisplay of prompt after any command execution
    if(redisplay_prompt){
        fprintf(tty,"\n");
        redisplay_prompt=0;
    }

    if(!tty){
        perror("fdopen");
        break;
    }
    fputs(prompt,tty);
    fflush(tty);
  
   //Taking user command after prompt from tty 
    if (!fgets(input, sizeof(input), tty)) {
        if(errno == EINTR){
            clearerr(tty);
            continue;
        }
        else{
            fclose(tty);
        break;

        }
    }
    input[strlen(input)-1]='\0';

    
    if(input[0]=='\0')continue;          //Re-display prompt if only Enter pressed at prompt 

    else{                                //Else start parsing the command buffer and proceed further

        if(!strcmp(input,"exit")) return SUCCESS;   // Exit the myshell program
    
        else if(!strncmp(input,"PS1=",4))           // user defined prompt
        {
            strcpy(prompt,input+4);
        }

        else if((cmd_type=check_cmd(input,cmd_buf))==INTERNAL){     //check if user entered Internal command (cd,pwd,jobs,fg,bg)
            int_cmd(cmd_buf);    
        }

        else if(cmd_type == ECHO) echo(cmd_buf);                    //Check for echo commands

        else if(cmd_type == pipe_grp)pipe_func(cmd_buf);            //Check if command contains a pipe '|'

        else if(cmd_type==EXTERNAL){                                //Check for external command                                           
        
            ext_cmnd(cmd_buf);

        continue;
        }

        else{
            printf("%s: command not found\n",input);
        }
    }
 
}

}