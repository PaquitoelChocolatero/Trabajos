#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include "array.h"
#include "mensaje.h"

#define INIT_OP 0
#define SET_OP  1
#define GET_OP  2
#define DESTROY_OP  3

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
    //respuesta del servidor
    struct respuesta res; 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = INIT_OP;
    pet.par1 = N;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Init(%s, %d)\n", getpid(), nombre, N);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("ERROR EN LA FUNCION INIT DEL CLIENTE-%d", getpid());
    else if(res.codigo == 0) printf("EL VECTOR YA ESTÁ CREADO CON EL MISMO N DE COMPONENTES DEL CLIENTE-%d", getpid());
    cerrarColas();
    return res.codigo;
    
}
int set (char *nombre, int i, int valor){
    iniciarColas();
    struct respuesta res;     //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = SET_OP;
    pet.par1 = i;
    pet.par2 = valor;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje a %s: Set(%d, %d)\n", getpid(), nombre, i, valor);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("ERROR EN LA FUNCION SET DEL CLIENTE-%d", getpid());
    cerrarColas();
    return res.codigo;
}
int get (char *nombre, int i, int *valor){
    iniciarColas();
    struct respuesta res;     //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = GET_OP;
    pet.par1 = i;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje a %s: Get(%d, %ls)\n", getpid(), nombre, i, valor);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("ERROR EN LA FUNCION GET DEL CLIENTE-%d", getpid());
    else{
        printf("El valor extraido de la lista por el cliente-%d es %d", getpid(), res.valor);
        *valor=res.valor;
    }
    cerrarColas();
    return res.codigo;
}
int destroy (char *nombre){
    iniciarColas();
    struct respuesta res;    //respuesta del servidor 
    struct peticion pet;
    /* se rellena la petición */
    pet.op  = DESTROY_OP;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Destroy(%s)\n", getpid(), nombre);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    mq_receive(q_cliente, (char*) &res, sizeof(int), 0);
    if(res.codigo == -1) printf("ERROR EN LA FUNCION DESTROY DEL CLIENTE-%d", getpid());
    cerrarColas();
    return res.codigo;
}
