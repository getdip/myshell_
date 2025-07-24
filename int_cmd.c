#include "main.h"

void int_cmd(char *buf[]){
    exit_val=0;

    // cd 
    // pwd
    // jobs
    // fg 
    // bg
    
    // cd ==> change directory as per user mentioned path. If path not provided changes to default /home/<username> 

    if(!strcmp(buf[0],"cd")){
        
        if((buf[1]==NULL)){
            chdir(getenv("HOME"));
        }
        else if( chdir(buf[1])==-1)exit_val=1;

    }

    //pwd  ==> Prints the present working directory on the terminal
    
    if(!strcmp(buf[0],"pwd")){ 
        char dir_name[1024];
        if(getcwd(dir_name,sizeof(dir_name))!=NULL);//returns the absolute path of pwd
        printf("%s",dir_name);
        printf("\n");
    }

    //jobs ==> display the processes in background and stopped processes 

    if(!strcmp(buf[0],"jobs")){

        Jlist* p=j_head;

        if(j_head!=NULL){

        while(p!=NULL){
            
            printf("[%d]    Stopped\n",p->pid);
            p=p->link;
            
        }
    }

        else  printf("No jobs found\n");

    }

    //fg ==> resumes the recently stopped in foreground

    if(!strcmp(buf[0],"fg")){

        //fg_running=1;
        if(j_head==NULL)printf("No suspended process to resume\n");

        else{

            tcsetpgrp(tty_fd,j_head->pid);
            pid_t pid_temp = j_head->pid;
            int st;
            kill(j_head->pid,SIGCONT);
            printf("PID:%d      [running]\n",pid_temp);
            waitpid(j_head->pid,&st,WUNTRACED); 
            
            if(WIFEXITED(st) || WIFSIGNALED(st)){

                remove_pid();
            }
  
            tcsetpgrp(tty_fd,shell_pgid);
            tcsetattr(tty_fd,TCSADRAIN,&shell_modes);

     }
    }

    //bg ==> resumes the recently stopped in background

    if(!strcmp(buf[0],"bg")){

        if(j_head==NULL)printf("No suspended process to resume\n");

        else{

            kill(j_head->pid,SIGCONT);
        }
    } 
}

