#include "../include/concurrency_layer.h"
#include "../lib/parser.c"
#include "../lib/operations_queue.c"

void init_concurrency_mechanisms(){

}

void destroy_concurrency_mechanisms(){

}

void* broker(void * args){
  struct broker_info * info;
  args = &info;
  char* file[256];
  *file = info->batch_file;
  stock_market * myMarket = info->market;

  struct operations_queue * myQueue = myMarket->stock_operations;
  iterator * it = new_iterator(*file);

  int operation;
  char id[11]; //Accion sobre la que se hace la operacion
  int type; //0->BUY  1->SELL
  int num_shares; //Numero de acciones
  int price; //Precio por accion
  struct operation * op;

  while(next_operation(it, id, &type, &num_shares, &price) != -1){
    new_operation(op, id, type, num_shares, price);
    enqueue_operation(myQueue, op);
  }
  destroy_iterator(it);
}

void* operation_executer(void * args){

}

void* stats_reader(void * args){

}
