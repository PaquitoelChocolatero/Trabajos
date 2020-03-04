all: list array lib server client-comp client-exec

lib:
	gcc -shared -fPIC -o libarray.so array.o
server:
	gcc -Wall -o servidor servidor.c lista.c array.c -lrt -lpthread
client-comp:
	gcc -Wall -c cliente.c
array:
	gcc -Wall -fPIC -o array.o -c array.c -lrt -lpthread
list:
	gcc -Wall -c lista.c -lrt -lpthread
client-exec:
	gcc -o cliente cliente.o lista.c libarray.so -lrt
clean:
	@echo "Cleaning up..."
	@rm -rfv *.o
