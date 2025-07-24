#include "main.h"

Jlist* j_head = NULL;

void insert_pid(pid_t data){

    Jlist* ptr = (Jlist*)malloc(sizeof(Jlist));

    if(j_head==NULL){     
        j_head=ptr;
        ptr->pid=data;
        ptr->link=NULL;

    }

    else{

        if(data!=j_head->pid){
            ptr->pid=data;
            ptr->link = j_head;
            j_head=ptr;

        }
        
    }
        
}

void remove_pid(){

    j_head=j_head->link;
}