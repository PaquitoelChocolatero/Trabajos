#include <pthread.h>

#define MAXSIZE 256

//Nodo utilizado para la lista enlazada
typedef struct node {
    char name[MAXSIZE];
    int *vector;
    int Nelem;
    struct node *next;
} nodeList;

//Funciones
int Init(char *nombre, int N);
int Set(char *nombre, int i, int valor);
int Get(char *nombre, int i, int *valor);
int Destroy(char *nombre);