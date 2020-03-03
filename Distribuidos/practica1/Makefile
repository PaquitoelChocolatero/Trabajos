all: list array server client

lib:

server:
	gcc -Wall -o servidor servidor.c lista.c array.c -lrt -lpthread
client:
	gcc -Wall -o cliente cliente.c lista.c array.c -lrt -lpthread
array:
	gcc -Wall -c array.c -lrt -lpthread
list:
	gcc -Wall -c lista.c -lrt -lpthread
clean:
	@echo "Cleaning up..."
	@rm -rfv *.o
