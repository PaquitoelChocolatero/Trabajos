#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


void inicializarLista(){
    pthread_mutex_init(&listamutex, NULL);
    Lista=NULL;
}

void finalizarLista(){
    pthread_mutex_destroy(&listamutex);
    Lista=NULL;
}

//Función que añade nodos a la lista enlazada
int Init(char *nombre, int N)
{
    pthread_mutex_lock(&listamutex);
    nodeList *p = Lista; //puntero para recorrer la lista y comprobar si ya hay un nodo con el mismo nombre
    while(p != NULL){
        if(strcmp(nombre, p->name)==0){
            if(p->Nelem==N){
                pthread_mutex_unlock(&listamutex);
                return 0;
            } 
            pthread_mutex_unlock(&listamutex);
            return -1;  
        }
        p=p->next;
    }
    nodeList *nuevoNodo = (nodeList *)malloc(sizeof(nodeList));
    strcpy(nuevoNodo->name, nombre);
    nuevoNodo->Nelem=N;
    nuevoNodo->vector=(int *)malloc(N*sizeof(int));
    for(int i=0; i<N; i++) nuevoNodo->vector[i] = 0;
    nuevoNodo->next = Lista;
    Lista = nuevoNodo;
    pthread_mutex_unlock(&listamutex);
    return 1;

}

int Set(char *nombre, int i, int valor)
{
    pthread_mutex_lock(&listamutex);
    nodeList *p = Lista;
    while(p != NULL){
        if(strcmp(nombre, p->name)==0){
            if(i <0 || i >= p->Nelem){
                pthread_mutex_unlock(&listamutex);
                return -1;
            } 
            p->vector[i]= valor;
            pthread_mutex_unlock(&listamutex);
            return 0;  
        }
        

        p=p->next;
    }
    pthread_mutex_unlock(&listamutex);
    return -1;
}

int Get(char *nombre, int i, int *valor)
{
    pthread_mutex_lock(&listamutex);
    nodeList *p = Lista;
    while(p != NULL){
        if(strcmp(nombre, p->name)==0){
            if(i <0 || i >= p->Nelem){
                pthread_mutex_unlock(&listamutex);
                return -1;
            } 
            *valor = p->vector[i];
            pthread_mutex_unlock(&listamutex);
            return 0;  
        }
        

        p=p->next;
    }
    pthread_mutex_unlock(&listamutex);
    return -1;
}

int Destroy(char *nombre) 
{
    pthread_mutex_lock(&listamutex);
    if(Lista==NULL) return -1;
    if(strcmp(Lista->name, nombre) == 0){
        nodeList *aux=Lista;
        Lista=Lista->next;
        aux->next = NULL;
        free(aux->vector);
        free(aux);
        pthread_mutex_unlock(&listamutex);
        return 1;
    }
    nodeList *p = Lista;
    while(p->next != NULL){
        if(strcmp(nombre, p->next->name)==0){
            nodeList *aux=p->next;
            p->next=p->next->next;
            aux->next= NULL;
            free(aux->vector);
            free(aux);
            pthread_mutex_unlock(&listamutex);
            return 1;
        }
        p=p->next;
    }
    pthread_mutex_unlock(&listamutex);
    return -1;
}
