#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
// #include "zemaphore.h"
#include "msgq.h"

// SEE Labs/GdbLldbLab for more information on lldb - lowlevel debugger

struct msgq *mq;
sem_t mutex; 

//
// Main threads
//
char *messages[] = { "msg1", "msg2", "hellomsg", "gustymsg" };

// sends msgs in messages[]
void *promtAndSend(void *arg) {
    sem_wait(&mutex); 
    for (int i = 0; i < sizeof(messages)/sizeof(char*); i++) {//for all possible messages
        char response[80];
        printf("Send? ");//ask to send the message
        scanf("%s", response);
        //if yes send the message to send method
        if (response[0] == 'y' || response[0] == 'Y') {
            printf("sending: %s\n", messages[i]);
            msgq_send(mq, messages[i]);
        }
    }
    sem_post(&mutex);
    return NULL;
}

// consume messges in msgq
void *recvMsgs(void *arg) {
    sem_wait(&mutex);
    sleep(5);
    //get length of list
    int msg_count = msgq_len(mq);
    printf("mq msg_count: %d\n", msg_count);
    for (int i = 0; i < msg_count; i++) {//for length of linked list
        //receive one message
        char *m = msgq_recv(mq);
        printf("recvMsgs: %s\n", m);    
        struct msgq *lastNode = mq;//sample to traverse linked list
        // go to the end of the list
        while(lastNode->next != 0){
            lastNode = lastNode->next;
        }
        //make new empty message
        struct msgq *A = malloc(sizeof(struct msgq));
        A->messag = ".";
        //add to end of list
        lastNode->next = A;
        //push start of list down 1
        mq = mq->next;
        //free received message
        if(strcmp(m, ".") != 0){ 
            free(m);
        }     
    }
    sem_post(&mutex);
    return NULL;
    
}

void *passiton(void *arg) {
    sem_wait(&mutex);
    int me = (int) arg;
    while (1) {
        sleep(1);
        //get length of list
        printf("passiton%d initial msgq_len: %d\n", me, msgq_len(mq));

        //receive one message
        char *m = msgq_recv(mq);

        //sample to traverse linked list
        struct msgq *lastNode = mq;

        //go to end of linked list
        while(lastNode->next != 0){
            lastNode = lastNode->next;
        }

        //make new empty message to put at end
        struct msgq *A = malloc(sizeof(struct msgq));
        A->messag = ".";

        //add to end of linked list
        lastNode->next = A;

        //push head of list down one
        mq = mq->next;
        printf("passiton%d: %p %p %s\n", me, &m, m, m);
        printf("passiton%d after recv msgq_len: %d\n", me, msgq_len(mq));
        //send it back to the end of the linked list
        msgq_send(mq, m);
        printf("passiton%d after send msgq_len: %d\n", me, msgq_len(mq));   
        //free the old message 
        free(m);
    }
    sem_post(&mutex); 
    return NULL;
}

#define MSGQLEN 4

int main(int argc, char *argv[]) {
    sem_init(&mutex, 0, 1);
    pthread_t p1, p2;
    mq = msgq_init(MSGQLEN);
    char test = '3';
    if (argc == 2)
        test = argv[1][0];
    switch (test) {
      case '1':
        printf("test fill and empty msgq\n");
        pthread_create(&p1, NULL, promtAndSend, NULL);
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 1:\n");
        msgq_show(mq);
        pthread_create(&p2, NULL, recvMsgs, NULL);
        pthread_join(p2, NULL);
        printf("msgq_show() after all consumed by test 1:\n");
        msgq_show(mq);
        break;
      case '2':
        printf("test fill msgs and pass it on\n");
        pthread_create(&p1, NULL, promtAndSend, NULL);
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 2:\n");
        msgq_show(mq);
        pthread_create(&p1, NULL, passiton, (void *)1);
        pthread_create(&p2, NULL, passiton, (void *)2);
        pthread_join(p1, NULL);
        pthread_join(p2, NULL);
        break;
      case '3': 
	printf("test fill send msgs and blocks\n");
        pthread_create(&p1, NULL, promtAndSend, NULL);
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 3:\n");
        msgq_show(mq);
      case'4': 
	printf("test fill recv msgs and blocks\n");
        pthread_create(&p1, NULL, recvMsgs, NULL);
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 4:\n");
        msgq_show(mq);
      case '5':
	printf("test fill a lot\n");
	int i = 0; 
	while (i < 101) { 
            pthread_create(&p1, NULL, promtAndSend, NULL);
 	    i++; 
	} 
        pthread_join(p1, NULL);
        printf("msgq_show() after filling for test 5:\n");
        msgq_show(mq);
      default:
        printf("invalid test selection!\n");
        break;
    }
    return 0;
}
