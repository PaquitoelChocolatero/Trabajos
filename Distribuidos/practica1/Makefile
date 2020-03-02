all: list server client

lib:

server:
	gcc -Wall -o servidor servidor.c -lrt -lpthread
client:
	gcc -Wall -o cliente cliente.c -lrt -lpthread
array:

list:
	gcc -Wall -c lista.c -lrt -lpthread
clean:
	@echo "Cleaning up..."
	@rm -rfv *.o
