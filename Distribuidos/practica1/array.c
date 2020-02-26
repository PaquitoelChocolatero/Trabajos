#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Lista enlazada de vectores
typedef struct node {
    char *name;
    int *vector;
    struct node *next;
} nodeList;

//Inicializamos la lista enlazada
nodeList *linkedList;
nodeList *auxlinkedList;

//Función que añade nodos a la lista enlazada
void add (nodeList *node)
{
    if(linkedList->vector != NULL)  //Añadimos siguiente nodo, desplazamos cabeza y ponemos valor
    {
        linkedList->next=node;
        linkedList=node;
        linkedList->name = node->name;
        linkedList->vector=node->vector;
    }
    else  //Si es el primer nodo sólo añadimos el vector
    {
        linkedList->vector=node->vector;
    }
}

int init (char *nombre, int N)
{
    while(linkedList->next != NULL)
    {
        if(strcmp(linkedList->name, nombre) == 0) return 0;
        linkedList=linkedList->next;
    }
    nodeList *node;
    node->name = nombre;
    int vector[N];
    node->vector=vector;
    if(!add(&node)) return -1;
    return 1;
}

int set (char *nombre, int i, int valor)
{
    while(linkedList->next != NULL)
    {
        if(strcmp(linkedList->name, nombre) == 0)
        {
            for(int j=0; j<sizeof(linkedList->vector)/sizeof(char); j++)
            {
                if(j==i) 
                {
                    linkedList->vector[j]=valor;
                    return 0;
                }
            }
        }
    }
    return -1;
}

int get (char *nombre, int i, int *valor)
{
    while(linkedList->next != NULL)
    {
        if(strcmp(linkedList->name, nombre) == 0)
        {
            for(int j=0; j<sizeof(linkedList->vector)/sizeof(char); j++)
            {
                if(j==i) 
                {
                    valor = linkedList->vector[j];
                    return 0;
                   
                }

            }
        }
    }
    return -1;
}

int destroy (char *nombre)
{
    while(linkedList->next != NULL)
    {
        if(strcmp(linkedList->name, nombre) == 0)
        {
            struct node *aux = linkedList->next;
            free(linkedList->vector);
            free(linkedList);
            
            nodeList *linkedList = auxlinkedList;
            while(linkedList->next != NULL)
            {
                linkedList->next = aux;
                return 0;
            }
        }
    }
    return -1;
}
