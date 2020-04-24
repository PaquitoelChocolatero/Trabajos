#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include "operations.h"
#define MAX_THREADS 40

//Declaracion de de los mutex
pthread_mutex_t listamutex;
pthread_mutex_t mutex;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;
pthread_mutex_t mfin;
pthread_t thid[MAX_THREADS];

int fin = false;    //Variable para controlar el final de los hilos
int n_elementos; //Numero de elementos en el buffer de peticiones

void cerrarServidor();

void print_usage() {
	printf("Usage: server -p puerto \n");
}

int main(int argc, char *argv[]) {

	// int  option = 0;
	// char port[256]= "";

	// while ((option = getopt(argc, argv,"p:")) != -1) {
	// 	switch (option) {
	// 	    	case 'p' : 
	// 				strcpy(port, optarg);
	// 	    		break;
	// 	    	default: 
	// 				print_usage(); 
	// 	    		exit(-1);
	// 	    }
	// }
	// if (strcmp(port,"")==0){
	// 	print_usage(); 
	// 	exit(-1);
	// }

	// printf("Port: %s\n", port);
	
	startServer();

	//Capturamos Ctrl+C para añadir funcionalidades
    printf("Para cerrar el servidor pulse: Ctrl+C\n");
    signal(SIGINT, cerrarServidor);

    struct peticion mess; //Mensaje a recibir

    //Atributos de la cola
    struct mq_attr atr;
    atr.mq_maxmsg = MAX_MESSAGES;
    atr.mq_msgsize = sizeof(struct peticion); 

    pthread_attr_t t_attr; //Atributos de los hilos
    
    int pos = 0;    //Posicion en el buffer
    
    //Error en caso de que no se pueda crear la cola del servidor
    if ((serverQueue = mq_open("/SERVIDOR", O_CREAT|O_RDONLY, 0700, &atr))==-1)
    {
        perror("No se puede crear la cola de servidor");
        return 1;
    }

    pthread_mutex_init(&listamutex, NULL);  //Iniciamos el mutex
    pthread_mutex_init(&mfin,NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&no_lleno,NULL);  
    pthread_cond_init(&no_vacio,NULL);
    pthread_attr_init(&t_attr);

    //Creacion del pool de threads
    for (int i = 0; i < MAX_THREADS; i++){
        if (pthread_create(&thid[i], NULL, servicio, NULL) !=0)
        {
            perror("Error creando el pool de threads\n");
            return 0;
        }
    }
    
    //El servidor queda a la espera de mensajes del cliente
    while (true)
    {
        //En caso de que haya un error al recibir se sale del bucle
        if(mq_receive(serverQueue, (char *) &mess, sizeof(struct peticion), 0) == -1) break; 
        
        pthread_mutex_lock(&mutex);

        //La función queda en espera si el n elementos alcanza el número máximo de peticiones
        while (n_elementos == MAX_PETICIONES) pthread_cond_wait(&no_lleno, &mutex);

        buffer_peticiones[pos] = mess;
        printf("SERVIDOR> Mensaje recibido (%d) del cliente %s y metido en el buffer\n", mess.op, mess.q_name);
        pos = (pos+1) % MAX_PETICIONES;
        n_elementos++;
        
        pthread_cond_signal(&no_vacio); 
        pthread_mutex_unlock(&mutex);
    }   
    return 0;
	
	stopServer();


	// registerUser("test");
	// registerUser("test2");
	// registerUser("test3");
	//Antes de hacer el conect insertar un archivo en registered.db -> INSERT INSERT INTO FILES (USER,NAME, DESCRIPTION) VALUES('test', 'test.mp3', 'Best song ever!');

	// connectUser("test", "128.160.46.1", 88);
	// connectUser("test2", "128.160.46.2", 89);
	// connectUser("test3", "128.160.46.3", 45);

	// publishFile("128.160.46.1", "test.txt", "This is a test");
	// publishFile("128.160.46.1", "paquito.docs", "This is a test");
	// publishFile("128.160.46.2", "saray.raw", "un bebé");
	// publishFile("128.160.46.3", "alex.py", "yo mismo");
	// publishFile("128.160.46.3", "test.c", "This is a test");

	//deleteFile("128.160.46.1", "test.txt");

	//disconnectUser("128.160.46.1");

	//unregisterUser("test2");
	//unregisterUser("test3");
	
	return 0;
}

void *servicio(){

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
        
        //Procesa la peticion y guarda su estado
        pthread_mutex_lock(&listamutex);
        if (mensaje.op ==0) res.codigo = Init(mensaje.v_name, mensaje.par1);
        else if (mensaje.op ==1) res.codigo = Set(mensaje.v_name, mensaje.par1, mensaje.par2);
        else if (mensaje.op ==2) res.codigo = Get(mensaje.v_name, mensaje.par1, &res.valor);
        else if (mensaje.op ==3) res.codigo = Destroy(mensaje.v_name);
        printf("SERVIDOR> Mensaje tratado con respuesta %d\n", res.codigo);            
        pthread_mutex_unlock(&listamutex);

        //Se devuelve el resultado a la cola del cliente
        q_cliente = mq_open(mensaje.q_name, O_WRONLY);
        if (q_cliente == -1) perror("No se puede abrir la cola del cliente");
        else {
            printf("SERVIDOR> Respondiendo a %s:\n", mensaje.q_name); 
            int err = mq_send(q_cliente, (const char *) &res, sizeof(struct respuesta), 0);
            if(err==-1) fprintf(stderr, "error al responder al cliente");
            mq_close(q_cliente);
        }
    }
    pthread_exit(0);
}
	
//Antes de cerrar el proceso cerramos todos los elementos: conexiones a bbdd, hilos y mutex
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
    pthread_mutex_destroy(&mfin);
    pthread_mutex_destroy(&listamutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    
    stopServer();
}