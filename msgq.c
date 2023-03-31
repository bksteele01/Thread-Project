#include "msgq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msgq *msgq_init(int num_msgs){
    //Head of list and constructor struct
    struct msgq *A;
    struct msgq *head = malloc(sizeof(struct msgq));

    //set empty message for beginning
    head->messag = ".";

    //fill the list with empty messages
    for(int i=0;i<num_msgs-1;i++){
        A = malloc(sizeof(struct msgq));
        A->messag = ".";
        A->next = head;
        head = A;
    }
    //return beginning of linked list
    return head;
}
int msgq_send(struct msgq *mq, char *msg){
    struct msgq *head = mq;
    //iterate through linked list
    while(!head == 0){

        //if the message is empty
        if(strcmp(head->messag, ".") == 0){

            //dynamically allocate message to memory
            char *msg2 = malloc(sizeof(msg));
            strcpy(msg2, msg);	

            //set message in the struct
            head->messag = msg2;
            return 1;
        }
        //increment place in list
        head = head->next;
    }
    return -1;
}
char *msgq_recv(struct msgq *mq){
    struct msgq *head = mq;

    //iterate through linked list
    while(!head == 0){

        //if message is not empty, return it
        if(strcmp(head->messag, ".") != 0){
            char *returner = head->messag;
            return returner;
        }
        //increment spot in linked list
        head = head->next;
    }
    //if all empty, return empty signifier
    return ".";
}
int msgq_len(struct msgq *mq){
    struct msgq *head = mq;
    int num_mes = 0;
    //iterate through linked list
    while(!head == 0){

        //if message is not empty, increment message number by 1
        if(strcmp(head->messag, ".") != 0){
            num_mes++;
        }

        //increment place in linked list
        head = head->next;
    }
    //number of non empty messages
    return num_mes;
}
void msgq_show(struct msgq *mq){
    struct msgq *show = mq;
    //iterate through linked list and show all messages.
    while(show != NULL){
        printf("%s\n", show->messag);
        show = show->next;
    }
}
