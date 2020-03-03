#include "array.h"

int main(void) 
{
    int res;
    res = init("vector1", 10);

    res = init("vector2", 20);

    res = set("vector1", 0, 44);

    res = set("vector1", 12, 30);

    int valor;
    res = get("vector1", 0, &valor);

    res = init("vector1", 20);

    res = destroy("vector1");

    res = destroy("vector");

    res = destroy("vector2");

}
