#ifndef MAIN_H
#define MAIN_H

#define SUCCESS 0
#define FAILURE -1

#define INTERNAL 1
#define EXTERNAL 2
#define ECHO     3
#define pipe_grp 4
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   
#include <errno.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>

/*Variables*/
//Pipe
extern char pipe_cnt;
extern int r;   //No.of commands

//Terminal Control
extern struct termios shell_modes;
extern int tty_fd;
extern FILE* tty;

//Shell
extern pid_t shell_pgid;

//Linked list to store stopped /background processes 
typedef struct node
{
    pid_t pid;
    struct node* link;

}Jlist;

extern Jlist* j_head;

//Jobs 
extern pid_t bg_jobs[15];
extern int bg_job_cnt;

//Exit value
extern int exit_val;

// Status flags
extern int run_bg;
extern int stp_fl,term_fl,ch_term_fl;


//Shell path
extern char shell_path[1024];


//functions prototypes
int check_cmd(char *buf,char*[]);
void getword(char *,char*[]);

void int_cmd(char *[]);
void ext_cmnd(char* []);
void echo(char*[]);
void insert_pid(pid_t);
void remove_pid();
void pipe_func(char*[]);

#endif