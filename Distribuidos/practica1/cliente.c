#include "array.h"
#include <unistd.h>

int main(void) 
{
    init("vector1", 10);

    init("vector2", 20);

    init("vector1", 10);

    init("vector2", 20);

    init("vector1", 15);

    init("vector2", 25);

    set("vector1", 0, 44);

    set("vector1", 12, 30);

    set("vector2", 5, 22);

    set("vector2", 24, 50);

    set("vector3", 0, 47);

    int valor;
    get("vector1", 0, &valor);

    get("vector1", 54, &valor);

    get("vector3", 54, &valor);

    int valor2;
    get("vector2", 0, &valor2);

    get("vector2", 54, &valor2);

    get("vector3", 1, &valor2);

    destroy("vector1");

    destroy("vector3");

    destroy("vector2");

    destroy("vector2");

}
