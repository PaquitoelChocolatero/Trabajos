#define MAXSIZE 256

struct peticion{
    int op; //codigo de la llamada (0 init) (1 set) (2 get) (4 terminar)
    char v_name[MAXSIZE];   //nombre del vector 
    int par1;   //parametro 1
    int par2;   //parametro 2
    char q_name[MAXSIZE];   //nombre de la cola del cliente
};