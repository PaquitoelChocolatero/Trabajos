#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//Lista enlazada de vectores
typedef struct node {
    char *name;
    int *vector;
    int Nelem;
    struct node *next;
} nodeList;

pthread_mutex_t listamutex;
nodeList *Lista;

//Funcion para inicializar lista 
void InicializarLista();
   
int Init (char *nombre, int N);
int Set (char *nombre, int i, int valor);
int Get (char *nombre, int i, int *valor);
int Destroy (char *nombre);