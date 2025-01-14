#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "lista.h"

extern nodeList *Lista;

//Función que añade nodos a la lista enlazada
int Init(char *nombre, int N)
{
    nodeList *p = Lista;    //Puntero para recorrer la lista y comprobar si ya hay un nodo con el mismo nombre

    //Primero comprobamos si ese vector ya existe
    while(p != NULL){
        if(strcmp(nombre, p->name)==0){
            if(p->Nelem == N) return 0; //Lista ya existente
            return -1;  //Existe con otra longitud
        }
        p = p->next;
    }

    //Insertamos nodo al principio de la lista dandole el espacio necesario
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
    nodeList *p = Lista;
    
    //Recorre la lista buscando dicho vector
    while(p != NULL){
        if(strcmp(nombre, p->name) == 0){
            if(i <0 || i >= p->Nelem) return -1;    //La direccion deseada no está en un rango valido del array
            
            p->vector[i]= valor;
            return 0;  
        }
        p = p->next;
    }
    return -1;  //No se ha encontrado el vector
}

int Get(char *nombre, int i, int *valor)
{
    nodeList *p = Lista;
    
    //Recorre la lista buscando dicho vector
    while(p != NULL){
        if(strcmp(nombre, p->name) == 0){
            if(i <0 || i >= p->Nelem) return -1;  //La direccion deseada no está en un rango valido del array

            *valor = p->vector[i];
            return 0;  
        }
        p = p->next;
    }
    return -1;  //No se ha encontrado el vector
}

int Destroy(char *nombre) 
{
    if(Lista == NULL) return -1;    //La lista esta vacia

    if(strcmp(Lista->name, nombre) == 0){   //Si el primer elemento es el que buscamos lo borramos
        nodeList *aux = Lista;
        Lista = Lista->next;
        aux->next = NULL;
        free(aux->vector);
        free(aux);
        return 1;
    }

    nodeList *p = Lista;

    //Recorremos la lista hasta encontrar el vector a eliminar
    while(p->next != NULL){
        if(strcmp(nombre, p->next->name) == 0){
            nodeList *aux=p->next;
            p->next=p->next->next;
            aux->next= NULL;
            free(aux->vector);
            free(aux);
            return 1;
        }
        p = p->next;
    }
    return -1;  //No se ha encontrado el vector
}