#include "../include/concurrency_layer.h"
#include <pthread.h>

//Mutex needed for broker and executer
pthread_mutex_t queue;
pthread_cond_t space;

void init_concurrency_mechanisms(){

    //Initialize mutex
    pthread_mutex_init(&queue, NULL);
    pthread_cond_init(&space, NULL);

}

void destroy_concurrency_mechanisms(){

    pthread_mutex_destroy(&queue);
    pthread_cond_destroy(&space);

}

void* broker(void * args){

    struct broker_info *info;
    info = args;
    char file[256];
    //Extract variables from the struct
    strcpy(file, info->batch_file);
    stock_market * myMarket = info->market;

    iterator * it = new_iterator(file);

    char id[11];
    int type;
    int shares;
    int price;

    //While there are more operations
    while(next_operation(it, id, &type, &shares, &price) != -1){

        //Make space for a new operation
        operation * op;
        op = (operation*) malloc(sizeof(operation));
        new_operation(op, id, type, shares, price);

        //Enter the mutex
        pthread_mutex_lock(&queue);

        //If it is fulll wait for space
        while(operations_queue_full(myMarket->stock_operations)) pthread_cond_wait(&space, &queue);

        enqueue_operation(myMarket->stock_operations, op);
        free(op);

        //Exit the mutex
        pthread_mutex_unlock(&queue);
    }
    destroy_iterator(it);
}

void* operation_executer(void * args){

    struct exec_info *info;
    info = args;
    int *myexit = info->exit;
    stock_market * myMarket = info->market;
    pthread_mutex_t *myexit_mutex = info->exit_mutex;

    operation * op;

    while (!myexit){
        if(!operations_queue_empty(myMarket->stock_operations)){

            //Enter queue mutex
            pthread_mutex_lock(&queue);
            dequeue_operation(myMarket->stock_operations, op);
            //Tell the broker space has been made
            pthread_cond_signal(&space);
            pthread_mutex_unlock(&queue);

            //Enter operation mutex
            int lock = pthread_mutex_lock(myexit_mutex);
            if(lock == 0){
                process_operation(myMarket, op);
            } else {
                *myexit = 1; //If there is a reader kick it out
            }
            pthread_mutex_unlock(myexit_mutex);
        }
    }
}

void* stats_reader(void * args){

    struct reader_info *info;
    info = args;
    int *myexit = info->exit;
    stock_market * myMarket = info->market;
    pthread_mutex_t *myexit_mutex = info->exit_mutex;
    int myfreq = info->frequency;

    struct operation * op;

    int lock = pthread_mutex_lock(myexit_mutex);
    if(lock == 0){
        while (!myexit){
           print_market_status(myMarket);
           usleep(myfreq);
        }
    } else {
        *myexit = 1;
    }
    pthread_mutex_unlock(myexit_mutex);
}
