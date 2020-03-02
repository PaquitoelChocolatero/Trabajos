#include "lista.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "mensaje.h"

#define MAX_MESSAGES 10
#define MAX_THREADS 10
#define MAX_PETICIONES 256

struct peticion buffer_peticiones[MAX_PETICIONES];
    
int n_elementos; // elementos en el buffer de peticiones
int pos_servicio = 0;
pthread_mutex_t mutex;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;

pthread_mutex_t mfin;
int fin=false;
void *servicio();

int main(void)
{
    mqd_t serverQueue; /* cola del servidor */
    struct peticion mess; /* mensaje a recibir */
    struct mq_attr atr; /* atributos de la cola */
    atr.mq_maxmsg = MAX_MESSAGES;
    atr.mq_msgsize = sizeof(struct peticion); 

    pthread_attr_t t_attr; /* atributos de los threads */
    pthread_t thid[MAX_THREADS];
    
    int error;
    int pos = 0;
    

    if ((serverQueue = mq_open("/SERVIDOR", O_CREAT|O_RDONLY, 0700, &atr))==-1) 
    {
        perror("No se puede crear la cola de servidor");
        return 1;
    }

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&no_lleno,NULL);  
    pthread_cond_init(&no_vacio,NULL);
    pthread_mutex_init(&mfin,NULL);

    pthread_attr_init(&t_attr);

    for (int i = 0; i < MAX_THREADS; i++){
        if (pthread_create(&thid[i], NULL, servicio, NULL) !=0)
        {
            perror("Error creando el pool de threads\n");
            return 0;
        }
    }
    

    while (true) 
    {
        error = mq_receive(serverQueue, (char *) &mess, sizeof(struct peticion), 0);    
        if (error == -1 || mess.op == 4) break;
        pthread_mutex_lock(&mutex);
        while (n_elementos == MAX_PETICIONES) pthread_cond_wait(&no_lleno, &mutex);
        buffer_peticiones[pos] = mess;
        pos = (pos+1) % MAX_PETICIONES;
        n_elementos++;
        pthread_cond_signal(&no_vacio);
        pthread_mutex_unlock(&mutex);
    } /* FIN while */

    pthread_mutex_lock(&mfin);

    fin=true;

    pthread_mutex_unlock(&mfin);
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&no_vacio);
    pthread_mutex_unlock(&mutex);

    for (int i=0;i<MAX_THREADS;i++)
    {
        pthread_join(thid[i],NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    pthread_mutex_destroy(&mfin);
    
    //cerrar colas
    mq_close(serverQueue);
    mq_unlink("/SERVIDOR");

    return 0;
} /* Fin main */

void *servicio(){
    struct peticion mensaje; /* mensaje local */
    mqd_t q_cliente; /* cola del cliente */
    int resultado; /* resultado de la operación */
    int valor;  //auxiliar para almacenar funcion get
    for(;;){
        pthread_mutex_lock(&mutex);
        while (n_elementos == 0) {
            if (fin==true) 
            {
                fprintf(stderr,"Finalizando servicio\n");
                pthread_mutex_unlock(&mutex);
                pthread_exit(0);
            }
            pthread_cond_wait(&no_vacio, &mutex);
        }
        mensaje = buffer_peticiones[pos_servicio];
        pos_servicio = (pos_servicio + 1) % MAX_PETICIONES;
        n_elementos --;
        pthread_cond_signal(&no_lleno);
        pthread_mutex_unlock(&mutex);
        /* procesa la peticion */
        /* ejecutar la petición del cliente y preparar respuesta */
        //AQUI ES DONDE HAY QUE HACER LAS LLAMADAS
        if (mensaje.op ==0) resultado=init(mensaje.v_name, mensaje.par1);
        else if (mensaje.op ==1) resultado=set(mensaje.v_name, mensaje.par1, mensaje.par2);
        else if (mensaje.op ==2) resultado=get(mensaje.v_name, mensaje.par1, &valor);
        else if (mensaje.op ==3) resultado=destroy(mensaje.v_name);
        
        /* Se devuelve el resultado al cliente */
        /* Para ello se envía el resultado a su cola */
        q_cliente = mq_open(mensaje.q_name, O_WRONLY);
        if (q_cliente == -1)
        perror("No se puede abrir la cola del cliente");
        else {
            printf("SERVIDOR> Respondiendo a %s:\n", mensaje.q_name); 
            mq_send(q_cliente, (const char *) &resultado, sizeof(int), 0);
            //Para la funcion get, hay que enviar tambien el valor
            if(mensaje.op == 2 && resultado==0)  mq_send(q_cliente, (const char *) &valor, sizeof(int), 0); 
            mq_close(q_cliente);
        }
    } // FOR
    pthread_exit(0);
} // servicio
