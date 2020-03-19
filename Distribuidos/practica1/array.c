#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include "array.h"
#include "mensaje.h"

//Definiciones globales de las peticiones
#define INIT_OP 0   
#define SET_OP  1
#define GET_OP  2
#define DESTROY_OP  3

mqd_t q_servidor; //creacion de cola de servidor
mqd_t q_cliente;    //creacion de cola del cliente
char queuename[MAXSIZE];    


void iniciarColas(){    //Funcion utilizada para inicializar las colas, se llamara a esta funcion al principio de init, set, get y destroy
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct respuesta);
    sprintf(queuename, "/Cola-%d", getpid());
    q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);  //funcion para abrir la cola del cliente
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);    //funcion para abrir la cola del servidor
}

void cerrarColas(){ //Funcion utilizada para cerrar las colas, se llamara a esta funcion al final de init, set, get y destroy
    mq_close(q_servidor);   //funcion para cerrar la cola del servidor
    mq_close(q_cliente);    //funcion para cerrar la cola del cliente
    mq_unlink(queuename);  
}

int init(char *nombre, int N){  //Funcion para inicializar un vector distribuido de N numeros enteros
    iniciarColas();
    //respuesta del servidor
    struct respuesta res; 
    struct peticion pet;
    //se rellena la petición 
    pet.op  = INIT_OP;
    pet.par1 = N;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Init(%s, %d)\n", getpid(), nombre, N);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);   //Se envia la peticion al servidor
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);   //El cliente recibe la respuesta
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION INIT\n", getpid());    //caso de error
    else if(res.codigo == 0) printf("CLIENTE-%d> EL VECTOR YA ESTÁ CREADO CON EL MISMO N DE COMPONENTES\n", getpid());  //caso de error porque ya existe un vector con el mismo numero de componentes
    else printf("CLIENTE-%d> Funcion Init realizada con exito\n", getpid());    //caso exito
    cerrarColas();
    return res.codigo;
    
}
int set (char *nombre, int i, int valor){   //Funcion para insertar el valor en la posicion i del vector nombre
    iniciarColas();
    //respuesta del servidor
    struct respuesta res;      
    struct peticion pet;
    //se rellena la petición 
    pet.op  = SET_OP;
    pet.par1 = i;
    pet.par2 = valor;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Set(%s, %d, %d)\n", getpid(), nombre, i, valor);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);   //Se envia la peticion al servidor
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);   //El cliente recibe la respuesta
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION SET\n", getpid()); //caso de error
    else printf("CLIENTE-%d> Funcion Set realizada con exito\n", getpid()); //caso de exito
    cerrarColas();
    return res.codigo;
}
int get (char *nombre, int i, int *valor){  //Funcion para recuperar el valor del elemento i del vector nombre
    iniciarColas();
    //respuesta del servidor
    struct respuesta res;      
    struct peticion pet;
    //se rellena la petición 
    pet.op  = GET_OP;
    pet.par1 = i;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Get(%s, %d)\n", getpid(), nombre, i);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);   //Se envia la peticion al servidor
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);   //El cliente recibe la respuesta
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION GET\n", getpid()); //caso de error
    else{   //caso de exito
        printf("CLIENTE-%d> Funcion get realizada con exito\n", getpid());
        printf("CLIENTE-%d> El valor extraido de la lista es %d\n", getpid(), res.valor);
        *valor=res.valor;
    }
    cerrarColas();
    return res.codigo;
}
int destroy (char *nombre){ //Funcion para borrar un vector previamente creado
    iniciarColas();
    //respuesta del servidor
    struct respuesta res;    
    struct peticion pet;
    //se rellena la petición
    pet.op  = DESTROY_OP;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);
    printf("CLIENTE-%d> Enviando mensaje: Destroy(%s)\n", getpid(), nombre);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);   //Se envia la peticion al servidor
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);   //El cliente recibe la respuesta
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION DESTROY\n", getpid()); //caso error
    else printf("CLIENTE-%d> Funcion Destroy realizada con exito\n", getpid()); //caso exito
    cerrarColas();
    return res.codigo;
}
