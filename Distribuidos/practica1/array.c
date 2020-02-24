#include <stdlib.h>

//Lista enlazada de vectores
typedef struct node {
    char *name;
    int *vector;
    struct node *next;
} nodeList;

//Inicializamos la lista enlazada
nodeList *linkedList;

//Función que añade nodos a la lista enlazada
void add (nodeList *node)
{
    if(linkedList->vector == NULL)  //Añadimos siguiente nodo, desplazamos cabeza y ponemos valor
    {
        linkedList->next=node;
        linkedList=node;
        strcpy(linkedList->name, node->name);
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
        if(strcmp(linkedList->name, nombre) == 1) return 0;
        linkedList=linkedList->next;
    }
    nodeList *node;
    strcpy(node->name, nombre);
    int vector[N];
    node->vector=vector;
    if(!add(&node)) return -1;
    return 1;
}

int set (char *nombre, int i, int valor)
{
    return 0;
}

int get (char *nombre, int i, int *valor)
{
    return 0;
}

int destroy (char *nombre)
{
    return 0;
}
