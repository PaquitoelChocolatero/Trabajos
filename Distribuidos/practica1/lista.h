#include <pthread.h>
#define MAXSIZE 256

typedef struct node {
    char name[MAXSIZE];
    int *vector;
    int Nelem;
    struct node *next;
} nodeList;

int Init(char *nombre, int N);
int Set(char *nombre, int i, int valor);
int Get(char *nombre, int i, int *valor);
int Destroy(char *nombre);
