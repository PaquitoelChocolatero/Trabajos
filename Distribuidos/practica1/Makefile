all:  servidor cliente libarray.so

servidor: lista.o
	gcc -Wall -o servidor servidor.c lista.o -lrt -lpthread

cliente.o:
	gcc -Wall -c cliente.c

cliente: cliente.o libarray.so
	gcc -Wall -o cliente cliente.o libarray.so -lrt

lista.o:
	gcc -Wall -c lista.c 

array.o:
	gcc -Wall -fPIC -c array.c

libarray.so: array.o
	gcc -Wall -fPIC -shared -o libarray.so array.o

clean:
	@echo "Cleaning up..."
	@rm -rfv *.o
	@rm -rfv cliente
	@rm -rfv servidor
	@rm -rfv libarray.so