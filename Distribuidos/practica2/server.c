#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "operations.h"
#include "lines.h"

#define MAX_MESSAGES 10
#define MAX_THREADS 40
#define MAX_LINE 256
#define MAX_LEN 1024

//Declaracion de los mutex
pthread_mutex_t bdmutex;
pthread_mutex_t mutex;
pthread_cond_t cond;

//Declaracion de los hilos
pthread_t thid[MAX_THREADS];
pthread_attr_t attr;
struct threadParams{
    int socket;
    char IPaddress[MAX_LINE];
}typedef threadParams;

//Declaracion de los hilos
int sd;

//Declaracion de funciones
void cerrarServidor();
void comunicacion(void *th_params);
void receive(int socket, char *mensaje);
void mySend(int socket, char *mensaje);

//Flag de actividad de los hilos
int busy = 1;



/*
*   MAIN
*/
int main(int argc, char *argv[]) {
	int option=0;
    char port[256]="";
    int val, err;
    threadParams tp;
    socklen_t size;
    struct sockaddr_in server_addr, client_addr;
	
	while((option = getopt(argc, argv,"p:")) != -1) {
		switch (option) {
			case 'p' : 
				strcpy(port, optarg);
		    	break;
		    default: 
				printf("Usage: server -p puerto \n");
		    	exit(-1);
		}
	}
	if (strcmp(port,"")==0){
		printf("Usage: server -p puerto \n"); 
		exit(-1);
	}

	//Iniciamos las bases de datos
	startServer();

	//Capturamos Ctrl+C para añadir funcionalidades
    signal(SIGINT, cerrarServidor);

    //Iniciamos los hilos
    pthread_mutex_init(&bdmutex, NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //Iniciamos el socket
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        perror("Error al iniciar el socket");
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
        printf("Error en el bind");
        return -1;
    }
    if(listen(sd, SOMAXCONN)<0){
        printf("Error en el listen\n");
        return -1;
    }

    //Capturamos el nombre del host
    char hostname[MAX_LINE];
    gethostname(hostname, MAX_LINE);

    //Inicio del servicio
    printf("S> Init %s: %s\n", hostname, port);
    printf("S> Esperando conexiones con clientes... (Ctrl+C para terminar)\n");
    size= sizeof(client_addr);
    int i = 0;
    while(1){

        //Aceptamos la conexión de un cliente
        int newsd = accept(sd, (struct sockaddr *) &client_addr, &size);
        if (newsd < 0){
            printf("Error en el accept");
            return -1;
        }

        //Le pasamos el socket a los hilos
        tp.socket = newsd;

        //Tratamos los datos del cliente
        strcpy(tp.IPaddress, inet_ntoa(client_addr.sin_addr));
        printf("S> Conectado con cliente IP: %s Puerto: %d\n", tp.IPaddress, ntohs(client_addr.sin_port));

        //Creamos un hilo
        pthread_create(&thid[i], &attr, (void *) &comunicacion, &tp);
        i = (i + 1) % MAX_THREADS;
        pthread_mutex_lock(&mutex);
        while(busy) pthread_cond_wait(&cond, &mutex);
        busy = 1;
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}



/*
*   COMUNICACION
*/
void comunicacion(void *th_params){

    char buf[MAX_LINE], user[MAX_LINE], user_dest[MAX_LINE], result[MAX_LINE];
    char puerto[MAX_LINE], file_name[MAX_LINE], descript[MAX_LINE], ip_local[20];
    int s_local;
    threadParams *tp = (threadParams*) th_params;

    //Asignamos socket e ip del cliente
    pthread_mutex_lock(&mutex);
    s_local = tp->socket;
    strcpy(ip_local, tp->IPaddress);
    busy = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    //Recivimos la operacion del cliente
    printf("S> Abriendo comunicación con cliente %s...\n", ip_local);
    receive(s_local, buf);

    //Tratamos cada operacion como le corresponde
    if(strcmp(buf, "REGISTER")==0){
        receive(s_local, user);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        //Accedemos a la base de datos
        pthread_mutex_lock(&bdmutex);
        int resultado = registerUser(user);
        pthread_mutex_unlock(&bdmutex);

        //Formateamos la respuesta del servidor
        sprintf(result, "%d", resultado);
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);
    }
    else if(strcmp(buf, "UNREGISTER") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        pthread_mutex_lock(&bdmutex);
        int resultado = unregisterUser(user);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);
    }
    else if(strcmp(buf, "CONNECT") == 0){
        receive(s_local, user);
        receive(s_local, puerto);
        int port = atoi(puerto);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        pthread_mutex_lock(&bdmutex);
        int resultado = connectUser(user, ip_local, port);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);
    }
    else if(strcmp(buf, "DISCONNECT") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        pthread_mutex_lock(&bdmutex);
        int resultado = disconnectUser(user);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);
    }
    else if(strcmp(buf, "PUBLISH") == 0){
        receive(s_local, user);
        receive(s_local, file_name);
        receive(s_local, descript);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        pthread_mutex_lock(&bdmutex);
        int resultado = publishFile(user, file_name, descript);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);
    }
    else if(strcmp(buf, "DELETE") == 0){
        receive(s_local, user);
        receive(s_local, file_name);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        pthread_mutex_lock(&bdmutex);
        int resultado = deleteFile(user, file_name);
        pthread_mutex_unlock(&bdmutex);

        sprintf(result, "%d", resultado);
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);
    }
    else if(strcmp(buf, "LIST_USERS") == 0){
        receive(s_local, user);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        //Creamos la lista en la que van a guardarse los resultados de la base de datos
        char **content;

        pthread_mutex_lock(&bdmutex);
        int resultado = list_users(user, &content);
        pthread_mutex_unlock(&bdmutex);

        //Formateamos y enviamos la respuesta del servidor
        if (resultado < 0) sprintf(result, "%d", -resultado);
        else strcpy(result, "0");
        mySend(s_local, result);
        printf("S> Enviando respuesta al cliente %s\n", result);

        if (resultado >=0) {
            //Enviamos el número de usuarios activos
            sprintf(result, "%d", resultado / 3);
            mySend(s_local, result);
            printf("S> Enviando num de usuarios: %s\n", result);
            //Enviamos todo lo que ha devuelto la base de datos
            //Hay 3 campos por cada usuario: nombre, ip, puerto
            for(int i = 0; i < resultado; i++) {
                mySend(s_local, content[i]);
                printf("S> Enviando datos: %s\n", content[i]);
            }

            //Eliminamos la lista
            if (resultado > 0) free(content); 
        }
    }
    else if(strcmp(buf, "LIST_CONTENT") == 0){
        receive(s_local, user);
        receive(s_local, user_dest);
        printf("S> Usuario: %s OPERACION: %s\n", user, buf);

        char **content;
        
        pthread_mutex_lock(&bdmutex);
        int resultado = list_content(user, user_dest, &content);
        pthread_mutex_unlock(&bdmutex);
        
        //Formateamos y enviamos la respuesta del servidor
        if (resultado < 0) { 
            sprintf(result, "%d", -resultado);
            mySend(s_local, result);
            printf("S> Enviando respuesta al cliente %s\n", result);
        }
        else {
            strcpy(result, "0");
            mySend(s_local, result);
            printf("S> Enviando respuesta al cliente %s\n", result);

            //Hay 2 campos por cada fichero del usuario: nombre, descripcion
            sprintf(result, "%d", resultado / 2); // envio num de files
            mySend(s_local, result);
            printf("S> Enviando numero de ficheros %s\n", result);
            // envio cada file name y descripcion
            for(int i = 0; i < resultado; i++) {
                mySend(s_local, content[i]);
                printf("S> Enviando datos:  %s\n", content[i]);
            }
            
            //Eliminamos la lista
            if (resultado > 0) free(content); 
        }
    }
    else
	{
		printf("S> RECEIVED WRONG COMMAND: %s-\n", buf);
	}
	printf("S> Cerrando comunicacion con cliente %s...\n", ip_local);
	close(s_local);
	pthread_exit(NULL);
}



void cerrarServidor() {
    
    close(sd);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&bdmutex);
    pthread_cond_destroy(&cond);
    //Migra los datos de la base de datos de activos a la de registrados para no perder nada
    stopServer();
    printf("\nCerrando servidor...\n");
    exit(0);

}



void receive(int socket, char *mensaje){
    if(readLine(socket, mensaje, MAX_LINE) <=0){
        printf("Error en readLine\n");
        exit(0);
    }
}



void mySend(int socket, char *mensaje){
    if(enviar(socket, mensaje, strlen(mensaje)+1) < 0){
        printf("Error en enviar\n");
        exit(0);
    }
}