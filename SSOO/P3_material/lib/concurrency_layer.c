#include "../include/concurrency_layer.h"

void init_concurrency_mechanisms(){

}
/*
void destroy_concurrency_mechanisms(){

}
*/
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
/*
void* operation_executer(void * args){

}
*/
/*
void* stats_reader(void * args){

}
*/
