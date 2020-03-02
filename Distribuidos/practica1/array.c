#include "array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "mensaje.h"

mqd_t q_servidor;
mqd_t q_cliente;
char queuename[MAXSIZE];


void iniciarColas(){
    struct mq_attr attr;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(int);
    sprintf(queuename, "/Cola-%d", getpid());
    q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
}

void cerrarColas(){
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(queuename);
}

int init(char *nombre, int N){
    iniciarColas();
    int res;    //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = 0;
    pet.par1 = N;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Init(%s, %d)\n", getpid(), nombre, N);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(int), 0);
    if(res == -1) printf("ERROR EN LA FUNCION INIT DEL CLIENTE-%d", getpid());
    cerrarColas();
    return res;
    
}
int set (char *nombre, int i, int valor){
    iniciarColas();
    int res;    //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = 1;
    pet.par1 = i;
    pet.par2 = valor;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Set(%s, %d)\n", getpid(), nombre, i, valor);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(int), 0);
    if(res == -1) printf("ERROR EN LA FUNCION SET DEL CLIENTE-%d", getpid());
    cerrarColas();
    return res;
}
int get (char *nombre, int i, int *valor){
    iniciarColas();
    int res;    //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = 2;
    pet.par1 = i;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Set(%s, %d)\n", getpid(), nombre, i, valor);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(int), 0);
    if(res == -1) printf("ERROR EN LA FUNCION GET DEL CLIENTE-%d", getpid());
    else{
        mq_receive(q_cliente,(char*) &valor, sizeof(int), 0);
    }
    cerrarColas();
    return res;
}
int destroy (char *nombre){
    iniciarColas();
    int res;    //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = 3;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Destroy(%s, %d)\n", getpid(), nombre);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(int), 0);
    if(res == -1) printf("ERROR EN LA FUNCION DESTROY DEL CLIENTE-%d", getpid());
    cerrarColas();
    return res;
}