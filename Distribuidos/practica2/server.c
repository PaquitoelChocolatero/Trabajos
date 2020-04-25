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
#define MAX_LINE 256

//Declaracion de de los mutex
pthread_mutex_t bdmutex;
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t thid[MAX_THREADS];
pthread_attr_t t_attr; //Atributos de los hilos

const int MAX_LEN = 1024;
int fin = false;    //Variable para controlar el final de los hilos

mqd_t serverQueue; //Declaración de la cola del servidor

struct threadParams{
    int socket;
    char IPaddress[MAX_LINE];
}

void cerrarServidor();
void comunicacion(void *th_params)
void receive(int socket, char *mensaje);
void send(int socket, char *mensaje);
void print_usage() {
	printf("Usage: server -p puerto \n");
}
int sd; //socket

int busy = 1;


int main(int argc, char *argv[]) {
	int option=0;
    char port[256]="";
    struct threadParams tp;
    int val;
    int err;
    socklen_t size;
    struct sockaddr_in server_addr, client_addr;
	
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


	//Iniciamos las bases de datos
	startServer();

	//Capturamos Ctrl+C para añadir funcionalidades
    printf("Para cerrar el servidor pulse: Ctrl+C\n");
    signal(SIGINT, cerrarServidor);

    pthread_mutex_init(&bdmutex, NULL);  //Iniciamos el mutex
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond), NULL);
    pthread_attr_init(&t_attr);
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
         perror("Error en socket");
         exit(1);
    }
    val = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
    if(err < 0){
        perror("Error en option");
        exit(1);
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port =htons((short)atoi(port));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        print("Error en el bind");
        return -1;
    }
    if(listen(sd, SOMAXCONN)<0){
        printf("Error en el listen\n");
        return -1;
    }
    char hostname[MAXSIZE];
    gethostname(hostname, MAXSIZE);
    printf("S> init %s : %s \n", hostname, port);
    printf("S> Esperando conexiones con clientes...(Ctrl+C para terminar)\n");
    size= sizeof(client_addr);
    int i = 0;
    while(true){
        int newsd = accept(sd, (struct sockaddr *) &client_addr, &size);
        if (newsd < 0){
            printf("Error en el accept");
            return -1;
        } 
        tp.socket = newsd;
        strcpy(tp.IPaddress, inet_ntoa(client_addr.sin_addr));
        printf("S> Conectado con cliente IP: %s Puerto: %d\n", tp.IPaddress, ntohs(client_addr.sin_port));
        pthread_create(&thid[i], &attr, (void *) &comunicacion, &tp);
        i = (i + 1) % MAX_THREADS;
        pthread_mutex_lock(&mutex);
        while(busy) pthread_cond_wait(&cond, $mutex);
        busy = 1;
        pthread_mutex_unlock(&mutex);
    }
    return 0; //no llega nunca aqui
}

void comunicacion(void *th_params){
    char buf[MAX_LINE];
    char user[MAX_LINE];
    char user_dest[MAX_LINE];
    char result[MAX_LINE];
    char puerto[MAX_LINE];
    char file_name[MAX_LINE];
    char descript[MAX_LINE];
    int s_local;
    char ip_local[20];
    struct threadParams *tp = (struct threadParams*) th_params;
    pthread_mutex_lock(&mutex);
    s_local = tp->socket;
    strcpy(ip_local, tp->IPaddress);
    busy = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    printf("S> Abriendo comunicación con cliente %s...\n", ip_local);
    receive(s_local, buf);
    printf("S> %s\n", buf);
    if(strcmp(buf, "REGISTER")==0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        pthread_mutex_lock(&bdmutex);
        int resultado = registerUsers(user);
        pthread_mutex_unlock(&bdmutex);
        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
    }
    else if(strcmp(buf, "UNREGISTER") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        pthread_mutex_lock(&bdmutex);
        int resultado = unregisterUsers(user);
        pthread_mutex_unlock(&bdmutex);
        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
    }
    else if(strcmp(buf, "CONNECT") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        receive(s_local, puerto);
        printf("S> puerto: %s\n", puerto);
        int port = atoi(puerto);
        pthread_mutex_lock(&bdmutex);
        int resultado = connectUser(user, ip_local, port);
        pthread_mutex_unlock(&bdmutex);
        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
    }
    else if(strcmp(buf, "DISCONNECT") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        pthread_mutex_lock(&bdmutex);
        int resultado = disconnectUser(user);
        pthread_mutex_unlock(&bdmutex);
        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
    }
    else if(strcmp(buf, "PUBLISH") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        receive(s_local, file_name)
        printf("S> Fichero: %s\n", file_name);
        receive(s_local, descript);
        printf("S> Descripcion: %s\n", descript);
        pthread_mutex_lock(&bdmutex);
        int resultado = publishFile(user, file_name, descript);
        pthread_mutex_unlock(&bdmutex);
        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
    }
    else if(strcmp(buf, "DELETE") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        receive(s_local, file_name)
        printf("S> Fichero: %s\n", file_name);
        pthread_mutex_lock(&bdmutex);
        int resultado = deleteFile(user, file_name);
        pthread_mutex_unlock(&bdmutex);
        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
    }
    else if(strcmp(buf, "LIST_USERS") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        char **content;
        pthread_mutex_lock(&bdmutex);
        int resultado = list_users(user, &content);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);

        for(int i=0; i<resultado; i++) send(s_local, content[i]);

        free(content);
    }
    else if(strcmp(buf, "LIST_CONTENT") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s\n", user);
        char **content;
        receive(s_local, user_dest);
        pthread_mutex_lock(&bdmutex);
        int resultado = list_content(user, user_dest, &content);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        printf("S> Enviando respuesta %s\n", result);
        send(s_local, result);
        
        for(int i=0; i<resultado; i++) send(s_local, content[i]);
        
        free(content);
    }



}
	
//Antes de cerrar el proceso cerramos todos los elementos: conexiones a bbdd, hilos y mutex
void cerrarServidor() {
    fprintf(stderr, "\nCerrando servidor...\n");

    for (int i=0;i<MAX_THREADS;i++) pthread_join(thid[i],NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&bdmutex);
    pthread_mutex_destroy(&cond);
    pthread_mutex_destroy(&attr);
    
    stopServer();
}

void receive(int socket, char *mensaje){
    if(readLine(socket, mensaje, MAX_LINE) <=0){
        printf("Error en readLine\n");
        exit(0);
    }
}

void send(int socket, char *mensaje){
    if(enviar(socket, mensaje, strlen(mensaje) + 1) <=0){
        printf("Error en readLine\n");
        exit(0);
    }
}