#include "../include/concurrency_layer.h"
#include "../lib/parser.c"
#include "../lib/operations_queue.c"

void init_concurrency_mechanisms(){

}

void destroy_concurrency_mechanisms(){

}

void* broker(broker_info info){
  //Definiciones
  char file[256] = info.batch_file;
  stock_market * market = info.market;
  //Creacion de puntero
  iterator * it = new_iterator(file);
  //Variables de la operacion
  int operation;
  char id; //Accion sobre la que se hace la operacion
  int type; //0->BUY  1->SELL
  int num_shares; //Numero de acciones
  int price; //Precio por accion
  //Lectura reiterada del archivo
  while(operation != -1){
    operation = next_operation(it, id, type, num_shares, price);
    new_operation(operation, id, type, num_shares, price);
    enqueue_operation(, operation)
  }
  //Destruccion del puntero
  destroy_iterator(it);
}

void* operation_executer(void * args){

}

void* stats_reader(void * args){

}
