#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "lines.h"

#define MAX_LINE 	1024

int main(int argc, char *argv[])
{

        int sd;
	char buff [MAX_LINE];
        struct sockaddr_in server_addr;
        struct hostent *hp;

        if (argc != 3) {
                printf("Uso: client <direccion_servidor> <puerto_servidor>\n");
                exit(0);
        }

	// se crea el socket
        sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sd < 0){
                perror("Error en socket");
                exit(1);
        }

	// se prepara la dirección del servidor
        bzero((char *)&server_addr, sizeof(server_addr));
        hp = gethostbyname (argv[1]);
        if (hp == NULL) {
                perror("Error en gethostbyname");
                exit(1);
        }

	memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("Error en el connect\n");
		return(-1);
	}
	while(1){
		readLine(0, buff, MAX_LINE);
		enviar(sd, buff, strlen(buff)+1);
		readLine(sd, buff, strlen(buff)+1);
		printf("%s\n", buff);
		if(strcmp(buff, "EXIT")==0){
			close(sd);		
		}
	}

        return (sd);

        return(0);
}

