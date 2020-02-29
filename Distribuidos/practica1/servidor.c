#include "lista.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#define MAX_MESSAGES 10
    
pthread_mutex_t mutex_mensaje;
pthread_cond_t cond_mensaje;    //Definimos hilos
int mensaje_no_copiado=true;

int main(){

    mqd_t serverQueue;  //Creamos la cola
    struct mq_attr atr; //Defnimos la cola
    atr.mq_maxmsg=MAX_MESSAGES;
    atr.mq_msgsize=sizeof(int);

    pthread_attr_t t_attr;
    pthread_t th_id;

    if((serverQueue=mq_open("/serverQueue.txt", O_CREAT|O_WRONLY, 0777, &atr))==-1){
        printf("ERROR MQ_OPEN");
        exit(-1);
    }

    pthread_mutex_init(&mutex_mensaje, NULL);
    pthread_cond_init(&cond_mensaje, NULL);
    pthread_attr_init(&t_attr);

    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    while(True)
    {
        mq_receive(serverQueue, (char *) &mess, sizeof(int), 0);
        pthread_create(&th_id, &t_attr, tratar_mensaje, &mess);
    }
    


    mq_close(serverQueue);
    mq_unlink("/serverQueue.txt");
}
