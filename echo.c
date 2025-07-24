#include "main.h"

void echo(char* buf[]){
    //exit_val=0;

    if(!strcmp(buf[1],"$SHELL")){       //Prints the SHELL's path
        exit_val=0;
        //getcwd(shell_path,sizeof(shell_path));
        printf("%s\n",shell_path);
    }

    else if(!strcmp(buf[1],"$?")){      //Prints the exit value of the last executed command
       // exit_val=0;
        printf("%d\n",exit_val);        
        exit_val=0;
    }

    else if(!strcmp(buf[1],"$$")){      //Prints SHELL's process ID      
        exit_val=0;
        printf("%d\n",getpid());
    }

    else{
        exit_val=127;
        printf("%s\n",buf[1]);
    }

}