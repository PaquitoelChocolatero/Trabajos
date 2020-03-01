#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "mensaje.h"

mqd_t q_servidor;
mqd_t q_cliente;
char queuename[MAXSIZE];

void iniciarColas();
void cerrarColas();
void cerrarserver();
int init (char *nombre, int N);
int set (char *nombre, int i, int valor);
int get (char *nombre, int i, int *valor);
int destroy (char *nombre);