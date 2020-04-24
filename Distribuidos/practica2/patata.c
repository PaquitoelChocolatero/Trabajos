#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lines.h"

const int MAX_LEN = 1024;

int main(int argc, char *argv[])
{
    int sd;
	int newsd;
	int val;
	int err;
	char buff [MAX_LEN];
	struct sockaddr_in server_addr, client_addr;

	//Creación del socket
    sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd < 0) {
        perror("Error en socket");
        exit(1);
    }

    val = 1;
    err = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));
    if (err < 0) {
        perror("Error en option");
        exit(1);
    }

	//Escribimos a 0 el socket
	bzero((char *)&server_addr, sizeof(server_addr));
	//Tipo de dirección
	server_addr.sin_family = AF_INET;
	//Puerto
	server_addr.sin_port = htons(4200);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	//Establecimiento de la dirección del socket
	if (bind(sd,(struct sockaddr *)&server_addr, sizeof(server_addr)) <0) {
		printf("Error en el bind\n");
		return(-1);
	}
	while(1){
		socklen_t antonio = sizeof(&client_addr);
		listen(sd,SOMAXCONN);
		newsd = accept(sd, (struct sockaddr *) &client_addr, &antonio);
		if (newsd < 0) {
			printf("Error en el accept\n");
			return(-1);
		}

		while(1){
			readLine(newsd, &buff, MAX_LEN);
			enviar(newsd, buff, MAX_LEN);
			if(strcmp(buff, "EXIT")==0){
				close(newsd);		
			}
		}

	}
	
	close(sd);
    return 0;
}

