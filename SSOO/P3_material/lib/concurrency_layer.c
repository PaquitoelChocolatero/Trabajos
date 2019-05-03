#include "../include/concurrency_layer.h"
#include <pthread.h>

//Mutex needed for readers and writers
pthread_mutex_t stock_operations;
pthread_mutex_t stat_readers;

void init_concurrency_mechanisms(){

    //Initialize mutex
    pthread_mutex_init(&stock_operations, NULL);
    pthread_mutex_init(&stat_readers, NULL);

}

void destroy_concurrency_mechanisms(){
    
    pthread_mutex_destroy(&stock_operations);
    pthread_mutex_destroy(&stat_readers);
}

void* broker(void * args){
    
    struct broker_info *info;
    info = args;
    char* file[256];
    *file = info->batch_file;
    stock_market * myMarket = info->market;

    iterator * it = new_iterator(*file);

    struct operation * op;

    while(next_operation(it, op->id, &op->type, &op->num_shares, &op->share_price) != -1){
        new_operation(op, op->id, op->type, op->num_shares, op->share_price);
        enqueue_operation(myMarket->stock_operations, op);
    }
    destroy_iterator(it);
}

void* operation_executer(void * args){
    
    struct exec_info *info;
    info = args;
    int* myexit = info->exit;
    stock_market * myMarket = info->market;
    pthread_mutex_t *myexit_mutex = info->exit_mutex;
    
    struct operation * op;
    
    int lock = pthread_mutex_lock(myexit_mutex);
    if(lock == 0){
        
        while (!*myexit){
            dequeue_operation(myMarket->stock_operations, op);
            process_operation(myMarket, op);
        }
    } else {
        *myexit = 1;
    }
    pthread_mutex_unlock(myexit_mutex);
}


void* stats_reader(void * args){

    struct reader_info *info;
    info = args;
    int* myexit = info->exit;
    stock_market * myMarket = info->market;
    pthread_mutex_t *myexit_mutex = info->exit_mutex;
    int myfreq = info->frequency;
    
    struct operation * op;
    
    int lock = pthread_mutex_lock(myexit_mutex);
    if(lock == 0){
        
        while (!*myexit){
           print_market_status(myMarket);
           usleep(myfreq);
        }
    } else {
        *myexit = 1;
    }
    pthread_mutex_unlock(myexit_mutex);
    
}
