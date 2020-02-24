#include <stdlib.h>

//Lista enlazada de vectores
typedef struct node {
    int *vector;
    struct node *next;
} nodeList;

//Inicializamos la lista enlazada
nodeList *linkedList;

//Función que añade nodos a la lista enlazada
void add (nodeList *node, int *vector)
{
    if(linkedList->vector == NULL)  //Añadimos siguiente nodo, desplazamos cabeza y ponemos valor
    {
        linkedList->next=node;
        linkedList=node;
        linkedList->vector=vector;
    }
    else  //Si es el primer nodo sólo añadimos el vector
    {
        linkedList->vector=vector;
    }
}

int init (char *nombre, int N)
{
    return 0;
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
