 #include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

extern nodeList *Lista;


//Función que añade nodos a la lista enlazada
int Init(char *nombre, int N)
{
    nodeList *p = Lista;    //Puntero para recorrer la lista y comprobar si ya hay un nodo con el mismo nombre
    while(p != NULL){   //busca en toda la lista un vector que tenga el mismo nombre 
        if(strcmp(nombre, p->name)==0){
            if(p->Nelem == N) return 0; //si tiene el mismo número de elementos devuelve 0
            return -1;  //En caso contrario devuelve -1
        }
        p = p->next;
    }
    //Insertamos nodo en la lista
    nodeList *nuevoNodo = (nodeList *)malloc(sizeof(nodeList));
    strcpy(nuevoNodo->name, nombre);
    nuevoNodo->Nelem=N;
    nuevoNodo->vector=(int *)malloc(N*sizeof(int));
    for(int i=0; i<N; i++) nuevoNodo->vector[i] = 0;
    nuevoNodo->next = Lista;    
    Lista = nuevoNodo;
    return 1;
}

int Set(char *nombre, int i, int valor)
{
    
    nodeList *p = Lista;    //Puntero para recorrer la lista y comprobar si ya hay un nodo con el mismo nombre
    while(p != NULL){   //busca en toda la lista un vector que tenga el mismo nombre
        if(strcmp(nombre, p->name) == 0){
            if(i <0 || i >= p->Nelem) return -1;    //Si la direccion objetivo del array no está en un rango valido devuelve -1
            p->vector[i]= valor;    //Si esta en un rango valido establece el valor y devuelve 0
            return 0;  
        }
        p = p->next;
    }
    return -1;
}

int Get(char *nombre, int i, int *valor)
{
    nodeList *p = Lista;    //Puntero para recorrer la lista y comprobar si ya hay un nodo con el mismo nombre
    while(p != NULL){   //busca en toda la lista un vector que tenga el mismo nombre
        if(strcmp(nombre, p->name) == 0){
            if(i <0 || i >= p->Nelem){  //Si la direccion objetivo del array no está en un rango valido devuelve -1
                return -1;
            } 
            *valor = p->vector[i];  ////Si esta en un rango valido recoge el valor en *valor y devuelve 0
            return 0;  
        }
        p = p->next;
    }
    return -1;
}

int Destroy(char *nombre) 
{
    if(Lista == NULL) return -1;    //Si la lista esta vacia, devuelve -1
    if(strcmp(Lista->name, nombre) == 0){   //Si el primer elemento es el que buscamos lo borramos y devuelve 1
        nodeList *aux = Lista;
        Lista = Lista->next;
        aux->next = NULL;
        free(aux->vector);
        free(aux);
        return 1;
    }
    nodeList *p = Lista;
    while(p->next != NULL){ //recorremos la lista hasta encontrar el vector a eliminar
        if(strcmp(nombre, p->next->name) == 0){ //Al encontrarlo lo eliminamos y devuelve 1
            nodeList *aux=p->next;
            p->next=p->next->next;
            aux->next= NULL;
            free(aux->vector);
            free(aux);
            return 1;
        }
        p = p->next;
    }
    return -1;  //en caso de no encontrar el vector, devuelve -1
}
