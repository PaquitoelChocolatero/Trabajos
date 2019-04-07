#include <stdio.h>
#include <pthread.h>

void * print(){
    fprintf(stdout, "¡Hola, soy un hilo!\n");
}

int main(){
    pthread_t hilo;
    pthread_create(&hilo, NULL, print, NULL);
    pthread_exit(pthread_self());
    return 0;
}
