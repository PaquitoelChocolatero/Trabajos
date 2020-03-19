#include <pthread.h>
#define MAXSIZE 256

typedef struct node {   //Nodo utilizado para la lista enlazada
    char name[MAXSIZE]; //nombre del vector
    int *vector;    //Vector
    int Nelem;  //Numero de elementos del vector
    struct node *next;  //Siguiente elemento en la lista
} nodeList;

int Init(char *nombre, int N);
int Set(char *nombre, int i, int valor);
int Get(char *nombre, int i, int *valor);
int Destroy(char *nombre);
