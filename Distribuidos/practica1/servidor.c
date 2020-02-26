#include "array.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MESSAGES 10

int main(){

    mqd_t serverQueue;
    struct mq_attr atr;
    atr.mq_maxmsg=MAX_MESSAGES;
    atr.mq_msgsize=sizeof(int);
    if((serverQueue=mq_open("/serverQueue.txt", O_CREAT|O_WRONLY, 0777, &atr))==-1){
        printf("ERROR MQ_OPEN");
        exit(-1);
    }


    mq_close(serverQueue);
    mq_unlink("/serverQueue.txt");
}