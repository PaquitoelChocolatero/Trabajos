#include "mensaje.h"
#include<mqueue.h>
#include <pthread.h>
#include <stdio.h>
void main(void) 
{
    mqd_t q_servidor; /* cola de mensajes del proceso servidor */
    mqd_t q_cliente; /* cola de mensajes para el proceso cliente */
    struct peticion pet;
    int res;
    struct mq_attr attr;
    char queuename[MAXSIZE];
    attr.mq_maxmsg= 1;
    attr.mq_msgsize= sizeof(int);
    sprintf(queuename,  "Cola-%d", getpid());
    q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
    q_servidor= mq_open("/SERVIDOR_SUMA", O_WRONLY); /* se rellena la petición */
    pet.opm= 0;
    pet.a= 5;
    pet.b= 2;   
    strcpy(pet.q_name, “/CLIENTE_UNO”);
    mq_send(q_servidor, (constchar*) &pet, sizeof(structpetiticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(int), 0);
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(queuename);
}
