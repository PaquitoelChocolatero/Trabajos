#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//Lista enlazada de vectores
typedef struct node {
    char *name;
    int *vector;
    int Nelem;
    struct node *next;
} nodeList;

pthread_mutex_t listamutex;
nodeList *Lista;

void inicializarLista();
   
int init(char *nombre, int N);
int set(char *nombre, int i, int valor);
int get(char *nombre, int i, int *valor);
int destroy(char *nombre);
