#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * Funcion(void* arg){
    int solution=*((int*) argv[1]);
	for(int i = 2 ; i < size ; i++){
    	solution-=*((int*)argv[i]);
    }    
    fprintf(stdout, "%i\n", solution);
}

int main(){
    pthread_t hilo;
    pthread_create(&hilo, NULL, Funcion, &Funcion);
    pthread_exit(pthread_self());
    return 0;
}
