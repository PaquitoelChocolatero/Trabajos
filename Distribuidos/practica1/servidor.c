#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "lista.h"
#include "mensaje.h"

#define MAX_MESSAGES 10
#define MAX_THREADS 10
#define MAX_PETICIONES 256
#define MAXSIZE 256

typedef struct node {
    char name[MAXSIZE];
    int *vector;
    int Nelem;
    struct node *next;
} nodeList;


struct peticion buffer_peticiones[MAX_PETICIONES];

int n_elementos; // elementos en el buffer de peticiones
int pos_servicio = 0;
nodeList *Lista = NULL;
pthread_mutex_t listamutex;
pthread_mutex_t mutex;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;

pthread_mutex_t mfin;
int fin = false;
pthread_t thid[MAX_THREADS];
mqd_t serverQueue; /* cola del servidor */
    

void *servicio();
void cerrarServidor();
void ShowLista();

int main(void)
{
    printf("Para cerrar el servidor pulse: Ctrl+C\n");
    signal(SIGINT, cerrarServidor); //Capturamos Ctrl+C para cerrar el servidor

    struct peticion mess; /* mensaje a recibir */
    struct mq_attr atr; /* atributos de la cola */
    atr.mq_maxmsg = MAX_MESSAGES;
    atr.mq_msgsize = sizeof(struct peticion); 

    pthread_attr_t t_attr; /* atributos de los threads */
    
    int error;
    int pos = 0;
    
    pthread_mutex_init(&listamutex, NULL);
    
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
        if (error == -1) break;
        pthread_mutex_lock(&mutex);
        while (n_elementos == MAX_PETICIONES) pthread_cond_wait(&no_lleno, &mutex);
        buffer_peticiones[pos] = mess;
        printf("SERVIDOR> Mensaje recibido (%d) del cliente %s y metido en el buffer\n", mess.op, mess.q_name);
        pos = (pos+1) % MAX_PETICIONES;
        n_elementos++;
        pthread_cond_signal(&no_vacio);
        pthread_mutex_unlock(&mutex);
    } /* FIN while */

    
    
    return 0;
} /* Fin main */

void *servicio(){
        struct peticion mensaje; /* mensaje local */
        struct respuesta res;
        mqd_t q_cliente; /* cola del cliente */
        for(;;){
            pthread_mutex_lock(&mutex);
        while (n_elementos == 0) {
            if (fin==true) 
            {
                fprintf(stderr,"Finalizando servicio %lu\n", (unsigned long int)pthread_self());
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
        pthread_mutex_lock(&listamutex);
        if (mensaje.op ==0) res.codigo = Init(mensaje.v_name, mensaje.par1);
        else if (mensaje.op ==1) res.codigo = Set(mensaje.v_name, mensaje.par1, mensaje.par2);
        else if (mensaje.op ==2) res.codigo = Get(mensaje.v_name, mensaje.par1, &res.valor);
        else if (mensaje.op ==3) res.codigo = Destroy(mensaje.v_name);
        printf("SERVIDOR> Mensaje tratado con respuesta %d\n", res.codigo); 
        //ShowLista();            
        pthread_mutex_unlock(&listamutex);                
        /* Se devuelve el resultado al cliente */
        /* Para ello se envía el resultado a su cola */
        q_cliente = mq_open(mensaje.q_name, O_WRONLY);
        if (q_cliente == -1)
        perror("No se puede abrir la cola del cliente");
        else {
            printf("SERVIDOR> Respondiendo a %s:\n", mensaje.q_name); 
            int err = mq_send(q_cliente, (const char *) &res, sizeof(struct respuesta), 0);
            if(err==-1) fprintf(stderr, "error");
            mq_close(q_cliente);
        }
    }
    pthread_exit(0);
}

void cerrarServidor() {
    fprintf(stderr, "\nCerrando servidor...\n");

    pthread_mutex_lock(&mfin);
    fin = true;
    pthread_mutex_unlock(&mfin);

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&no_vacio);
    pthread_mutex_unlock(&mutex);

    for (int i=0;i<MAX_THREADS;i++) pthread_join(thid[i],NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    pthread_mutex_destroy(&mfin);
    pthread_mutex_destroy(&listamutex);
    
    mq_close(serverQueue);
    mq_unlink("/SERVIDOR");
}


void ShowLista(){
    printf("\n******LISTA******\n");
    nodeList *p = Lista;
    while(p != NULL){
        printf("\nNombre: %s\n", p->name);
        printf("Size: %d\n", p->Nelem);
        for(int i = 0; i < p->Nelem; i++) printf("%d, ", p->vector[i]);

        p = p->next;
    }
    printf("\n*****************\n");
}
