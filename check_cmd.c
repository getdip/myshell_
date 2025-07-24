#include "main.h"
int run_bg=0;
char pipe_cnt=0;


int r,len,c;
void getword(char *temp,char *res[]){       // Parses the command line buffer, creates an array of space separated strings and returns it to the calling function
    r=0;
    char *ptr=temp;
    c=0;                                    
    unsigned char fl_space=0;
    unsigned char trailing_space=0;
    for(int i=0;i<len;i++){
        if(temp[i]==' '){
            if(i==len-1)trailing_space=1;
            
            if(fl_space==0){
                
                temp[i]='\0';
                res[r++]=ptr;
                ptr+=(c+1);
                c=0;
            }

            else if(fl_space==1){
                ptr++;
            }

            fl_space=1;
        }

     else if(temp[i]=='|'){
        if(fl_space==0){

            temp[i]='\0';
            res[r++]=ptr;
            
            res[r++]="|";
            ptr+=(c+1);
            
        }
        else if(fl_space==1){
            res[r++]="|";
            ptr++;
        }
        fl_space=1;
        }

        else{
            fl_space=0;
            c++;
        }
    }

    if(!strcmp(ptr,"&")){

        run_bg=1;
        res[r]=NULL;
        return; 
    }
    if(trailing_space)res[r]=NULL;
    else{
        res[r++]=ptr;
        res[r]=NULL;
    }
}

// check_cmd() checks for type of command (INTERNAL, EXTERNAL, pipe_grp, ECHO)
int check_cmd(char *buf,char* cmd[]){

    run_bg=0;
    len = strlen(buf);
    getword(buf,cmd);
    
   for(int i=0; i<=r;i++){
        
        if( (cmd[i]!=NULL) && !strcmp(cmd[i],"|")){
            pipe_cnt++;
        }
        if((i==r) && (pipe_cnt>0))return pipe_grp;   
    }

    char* icmd[5]={"cd","pwd","jobs","fg","bg"};

    for(int i=0;i<5;i++){

        if(!strcmp(cmd[0],icmd[i])) return INTERNAL;
    }
    
    if(!strcmp(cmd[0],"echo")) return ECHO;
    
    if(!strcmp(cmd[r-1],"&")){
        run_bg=1;
    }
    return EXTERNAL;
    
}