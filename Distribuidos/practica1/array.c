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

mqd_t q_servidor; //Creacion de cola de servidor
mqd_t q_cliente;  //Creacion de cola del cliente
char queuename[MAXSIZE];    

void iniciarColas(){
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(struct respuesta);
    sprintf(queuename, "/Cola-%d", getpid());
    q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);  //Abrir la cola del cliente
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);                    //Abrir la cola del servidor
}

void cerrarColas(){
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(queuename);  
}

//Funcion para inicializar un vector distribuido de N numeros enteros
int init(char *nombre, int N){
    iniciarColas();
    struct respuesta res;
    struct peticion pet;

    //se rellena la petición 
    pet.op  = INIT_OP;
    pet.par1 = N;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);

    //Se envia la peticion al servidor
    printf("CLIENTE-%d> Enviando mensaje: Init(%s, %d)\n", getpid(), nombre, N);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);

    //El cliente recibe la respuesta
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION INIT\n", getpid());    //Si ha encontrado un error
    else if(res.codigo == 0) printf("CLIENTE-%d> EL VECTOR YA ESTÁ CREADO CON EL MISMO N DE COMPONENTES\n", getpid());  //Ya existe un vector con el mismo numero de componentes
    else printf("CLIENTE-%d> Funcion Init realizada con exito\n", getpid());    //Exito
    
    cerrarColas();
    return res.codigo; //La función devuelve unicamente devulve como ha ido la operacion
}

//Funcion para insertar el valor en la posicion i del vector nombre
int set (char *nombre, int i, int valor){
    iniciarColas();
    struct respuesta res;      
    struct peticion pet;

    //se rellena la petición 
    pet.op  = SET_OP;
    pet.par1 = i;
    pet.par2 = valor;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);

    //Se envia la peticion al servidor
    printf("CLIENTE-%d> Enviando mensaje: Set(%s, %d, %d)\n", getpid(), nombre, i, valor);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);

    //El cliente recibe la respuesta
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION SET\n", getpid()); //caso de error
    else printf("CLIENTE-%d> Funcion Set realizada con exito\n", getpid()); //caso de exito
    
    cerrarColas();
    return res.codigo;
}

//Funcion para recuperar el valor del elemento i del vector nombre
int get (char *nombre, int i, int *valor){
    iniciarColas();
    struct respuesta res;      
    struct peticion pet;

    //se rellena la petición 
    pet.op  = GET_OP;
    pet.par1 = i;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);

    //Se envia la peticion al servidor
    printf("CLIENTE-%d> Enviando mensaje: Get(%s, %d)\n", getpid(), nombre, i);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);

    //El cliente recibe la respuesta
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION GET\n", getpid()); //caso de error
    else{   //caso de exito
        printf("CLIENTE-%d> Funcion get realizada con exito\n", getpid());
        printf("CLIENTE-%d> El valor extraido de la lista es %d\n", getpid(), res.valor);
        *valor=res.valor;
    }

    cerrarColas();
    return res.codigo;
}

//Funcion para borrar un vector previamente creado
int destroy (char *nombre){
    iniciarColas();
    struct respuesta res;    
    struct peticion pet;

    //se rellena la petición
    pet.op  = DESTROY_OP;
    strcpy(pet.v_name, nombre);
    strcpy(pet.q_name, queuename);

    //Se envia la peticion al servidor
    printf("CLIENTE-%d> Enviando mensaje: Destroy(%s)\n", getpid(), nombre);
    mq_send(q_servidor, (const char *) &pet, sizeof(struct peticion), 0);
    
    //El cliente recibe la respuesta
    mq_receive(q_cliente, (char*) &res, sizeof(struct respuesta), 0);
    if(res.codigo == -1) printf("CLIENTE-%d> ERROR EN LA FUNCION DESTROY\n", getpid()); //caso error
    else printf("CLIENTE-%d> Funcion Destroy realizada con exito\n", getpid()); //caso exito
    
    cerrarColas();
    return res.codigo;
}