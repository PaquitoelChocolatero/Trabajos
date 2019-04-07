// fichero ej7-crearthreads.c
//THREADS
// Realizar un programa que declare 2 funciones 1 y 2, donde se indique la identidad del thread 
//que la está ejecutando. la función 1 se duerme 2 segundos y la 2 se duerme 5 segundos.     
//A continuación el programa principal debe lanzar 2 threads, uno para cada función, escribir 
//su propia identidad y terminar.
//Haga una nueva versión en la que el padre espere a que los hijos terminen antes de terminar. 
// compilar con  gcc -lpthread ej7-crearthreads.c

//Jesús Carretero

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

pthread_t thread1, thread2, thmain; /* Declaración de los threads */
pthread_attr_t attr; /* atributos de los threads */ 

/* Definición de las funciones func1 y func2 */
void *func1 () 
{ 
  pthread_t tid = pthread_self(); /* identificador de thread*/ 
  printf("Soy el thread 1 y voy a ejecutar func1 \n"); 
  sleep(2); 
  printf("Soy el thread 1 y he terminado de ejecutar la funcion 1\n"); 
  pthread_exit(NULL); /* Provoca la terminación del thread*/ 
} 
void *func2 () 
{ 
  pthread_t tid = pthread_self(); /* identificador de thread*/ 
  printf("Soy el thread 2 y voy a ejecutar func2 \n"); 
  sleep(5); 
  printf("Soy el thread 2 y he terminado de ejecutar la funcion 2\n"); 
  pthread_exit(NULL); /* Provoca la terminación del thread*/ 
}


/*Función main*/ 
int main(void) 
{ 
  thmain = pthread_self(); 
  /*La propia función main es un thread*/ 
  /*inicializa los parámetros de los threads por defecto*/
  pthread_attr_init (&attr); 
  printf("Soy la funcion main y voy a lanzar los dos threads \n"); 
  pthread_create (&thread1, &attr, func1, NULL); 
  pthread_create (&thread2, &attr, func2, NULL); 
  printf("Soy main: he lanzado los dos threads y termino\n"); 
  pthread_exit (NULL); 
}
