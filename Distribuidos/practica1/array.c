#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"

//Lista enlazada de vectores
typedef struct node {
    char *name;
    int *vector;
    int Nelem;
    struct node *next;
} nodeList;

//Inicializamos la lista enlazada
nodeList *linkedList;
nodeList *headerList;

//Función que añade nodos a la lista enlazada
int add (nodeList *node)
{
    while(linkedList->next != NULL) linkedList=linkedList->next;
    if(linkedList->vector != NULL)  //Añadimos siguiente nodo, desplazamos cabeza y ponemos valor
    {
        linkedList->next=node;
        linkedList=node;
        linkedList->name = node->name;
        linkedList->vector=node->vector;
        if(headerList->next == NULL) headerList=linkedList;
        return 0;
    }
    else  //Si es el primer nodo sólo añadimos el vector
    {
        linkedList->vector=node->vector;
        headerList=linkedList;
        return 0;
    }
    return -1;
}

int init (char *nombre, int N)
{
    linkedList=headerList; //volvemos a poner el puntero al principio de la lista
    while(linkedList->next != NULL) //Recorre la lista y si encuentra un nodo igual devuelve 0
    {
        if(strcmp(linkedList->name, nombre) == 0) return 0;
        linkedList=linkedList->next;
    }
    //En caso de no encontrar un nodo igual, lo añade al final de la lista
    nodeList *node;
    node->name = nombre;
    int vector[N];
    node->vector=vector;
    node->Nelem=N;
    if(add(&node)!=0) return -1;
    return 1;
}

int set (char *nombre, int i, int valor)
{
    linkedList=headerList; 
    while(linkedList->next != NULL) //Recorre la lista 
    {
        if(strcmp(linkedList->name, nombre) == 0) //Si encuentra el vector, lo recorre
        {
         
            //Cuando llega a la posicion i del vector le da el valor
            linkedList->vector[i]=valor;
            return 0;
        }
    }
    return -1;
}

int get (char *nombre, int i, int *valor)
{
    linkedList=headerList;
    while(linkedList->next != NULL) //Recorre la lista
    {
        if(strcmp(linkedList->name, nombre) == 0) //Si encuentra el vector, lo recorre
        {
            *valor = linkedList->vector[i];
            return 0;
        }
    }
    return -1;
}

int destroy (char *nombre) 
{
    linkedList=headerList;
    while(linkedList->next != NULL)
    {
        if(strcmp(linkedList->name, nombre) == 0)
        {
            struct node *aux = linkedList->next;
            free(linkedList->vector);
            free(linkedList);
            
            nodeList *linkedList = headerList;
            while(linkedList->next != NULL) linkedList=linkedList->next;
            linkedList->next = aux;
            return 0;   
        }
    }
    return -1;
}
