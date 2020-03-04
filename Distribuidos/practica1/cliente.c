#include "array.h"

int main(void) 
{
    init("vector1", 10);

    init("vector2", 20);

    set("vector1", 0, 44);

    set("vector1", 12, 30);

    int valor;
    get("vector1", 0, &valor);

    init("vector1", 20);

    destroy("vector1");

    destroy("vector");

    destroy("vector2");

}
