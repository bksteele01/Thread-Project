#include "msgq.h"
#include <stdio.h>
#include <stdlib.h>

struct msgq *msgq_init(int num_msgs){
    struct msgq *A;
    struct msgq *head = malloc(sizeof(struct msgq));
    head->place = 1;
    head->messag = " ";
    for(int i=0;i<num_msgs-1;i++){
        A = malloc(sizeof(struct msgq));
        A->place = i+2;
        A->messag = " ";
        A->next = head;
        head = A;
    }
    return head;
}
int msgq_send(struct msgq *mq, char *msg){
    struct msgq *head = mq;
    while(!head == NULL){
        if(head->messag == " "){
            char *msg2 = malloc(sizeof(msg));
            msg2 = msg;	
            head->messag = msg2;
            return 1;
        }
        head = head->next;
    }
    return -1;
}
char *msgq_recv(struct msgq *mq){
    return "";
}
int msgq_len(struct msgq *mq){
    return 1;
}
void msgq_show(struct msgq *mq){
    struct msgq *show = mq;
    while(show != NULL){
        printf("%s\n", show->messag);
        show = show->next;
    }
}