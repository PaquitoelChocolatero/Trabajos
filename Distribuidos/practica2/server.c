#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <mqueue.h>
#include "operations.h"

#define MAX_MESSAGES 10
#define MAX_THREADS 40
#define MAX_PETICIONES 256

//Declaracion de de los mutex
pthread_mutex_t bdmutex;
pthread_mutex_t mutex;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;
pthread_mutex_t mfin;
pthread_t thid[MAX_THREADS];
pthread_attr_t t_attr; //Atributos de los hilos

const int MAX_LEN = 1024;
int fin = false;    //Variable para controlar el final de los hilos

mqd_t serverQueue; //Declaración de la cola del servidor

void cerrarServidor();

void print_usage() {
	printf("Usage: server -p puerto \n");
}

int main(int argc, char *argv[]) {

	int sd;
	int newsd;
	int val;
	int err;
	char buff [MAX_LEN];
	struct sockaddr_in server_addr, client_addr;

	int  option = 0;
	char port[256]= "";

	while ((option = getopt(argc, argv,"p:")) != -1) {
		switch (option) {
			case 'p' : 
				strcpy(port, optarg);
		    	break;
		    default: 
				print_usage(); 
		    	exit(-1);
		}
	}
	if (strcmp(port,"")==0){
		print_usage(); 
		exit(-1);
	}

	printf("Port: %s\n", port);

	//Iniciamos las bases de datos
	startServer();

	//Capturamos Ctrl+C para añadir funcionalidades
    printf("Para cerrar el servidor pulse: Ctrl+C\n");
    signal(SIGINT, cerrarServidor);

    // //Creación del socket
    // sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // if (sd < 0) {
    //     perror("Error en socket");
    //     exit(1);
    // }

    // val = 1;
    // err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
    // if (err < 0) {
    //     perror("Error en option");
    //     exit(1);
    // }
    
    // int pos = 0;    //Posicion en el buffer

    // pthread_mutex_init(&bdmutex, NULL);  //Iniciamos el mutex
    // pthread_mutex_init(&mfin,NULL);
    // pthread_mutex_init(&mutex,NULL);
    // pthread_cond_init(&no_lleno,NULL);  
    // pthread_cond_init(&no_vacio,NULL);
    // pthread_attr_init(&t_attr);

    // //Creacion del pool de threads
    // for (int i = 0; i < MAX_THREADS; i++){
    //     if (pthread_create(&thid[i], NULL, servicio, NULL) !=0)
    //     {
    //         perror("Error creando el pool de threads\n");
    //         return 0;
    //     }
    // }

	// //Establecimiento de la dirección del socket
	// if (bind(sd,(struct sockaddr *)&server_addr, sizeof(server_addr)) <0) {
	// 	printf("Error en el bind\n");
	// 	return(-1);
	// }
    
    // //El servidor queda a la espera de mensajes del cliente
    // while (true)
    // {
	// 	socklen_t antonio = sizeof(&client_addr);
	// 	listen(sd,SOMAXCONN);
	// 	newsd = accept(sd, (struct sockaddr *) &client_addr, &antonio);
	// 	if (newsd < 0) {
	// 		printf("Error en el accept\n");
	// 		return(-1);
	// 	}

	// 	while(true)
	// 	{
	// 		readLine(newsd, &buff, MAX_LEN);
	// 		enviar(newsd, buff, MAX_LEN);
	// 		if(strcmp(buff, "EXIT")==0){
	// 			close(newsd);		
	// 		}
	// 	}
        
    //     pthread_mutex_lock(&mutex);

    //     //La función queda en espera si el n elementos alcanza el número máximo de peticiones
    //     while (n_elementos == MAX_PETICIONES) pthread_cond_wait(&no_lleno, &mutex);

    //     buffer_peticiones[pos] = mess;
    //     printf("SERVIDOR> Mensaje recibido (%d) del cliente %s y metido en el buffer\n", mess.op, mess.q_name);
    //     pos = (pos+1) % MAX_PETICIONES;
    //     n_elementos++;
        
    //     pthread_cond_signal(&no_vacio); 
    //     pthread_mutex_unlock(&mutex);
    // }   
    // close(sd);
    // return 0;
    publishFile("test", "test.txt", "test");
	
	return 0;
}

// void *servicio(){

//         for(;;){
//             pthread_mutex_lock(&mutex);
//             while (n_elementos == 0) {
//                 if (fin==true) 
//                 {
//                     fprintf(stderr,"Finalizando servicio %lu\n", (unsigned long int)pthread_self());
//                     pthread_mutex_unlock(&mutex);
//                     pthread_exit(0);
//                 }
//                 pthread_cond_wait(&no_vacio, &mutex);
//         }

//         mensaje = buffer_peticiones[pos_servicio];
//         pos_servicio = (pos_servicio + 1) % MAX_PETICIONES;
//         n_elementos --;
//         pthread_cond_signal(&no_lleno);
//         pthread_mutex_unlock(&mutex);
        
//         //Procesa la peticion y guarda su estado
//         pthread_mutex_lock(&listamutex);
//         if (mensaje.op ==0) res.codigo = Init(mensaje.v_name, mensaje.par1);
//         else if (mensaje.op ==1) res.codigo = Set(mensaje.v_name, mensaje.par1, mensaje.par2);
//         else if (mensaje.op ==2) res.codigo = Get(mensaje.v_name, mensaje.par1, &res.valor);
//         else if (mensaje.op ==3) res.codigo = Destroy(mensaje.v_name);
//         printf("SERVIDOR> Mensaje tratado con respuesta %d\n", res.codigo);            
//         pthread_mutex_unlock(&listamutex);

//         //Se devuelve el resultado a la cola del cliente
//         q_cliente = mq_open(mensaje.q_name, O_WRONLY);
//         if (q_cliente == -1) perror("No se puede abrir la cola del cliente");
//         else {
//             printf("SERVIDOR> Respondiendo a %s:\n", mensaje.q_name); 
//             int err = mq_send(q_cliente, (const char *) &res, sizeof(struct respuesta), 0);
//             if(err==-1) fprintf(stderr, "error al responder al cliente");
//             mq_close(q_cliente);
//         }
//     }
//     pthread_exit(0);
// }
	
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
    pthread_mutex_destroy(&bdmutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    
    stopServer();
}