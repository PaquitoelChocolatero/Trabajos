#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "operations.h"

void print_usage() {
	printf("Usage: server -p puerto \n");
}

int main(int argc, char *argv[]) {

	// int  option = 0;
	// char port[256]= "";

	// while ((option = getopt(argc, argv,"p:")) != -1) {
	// 	switch (option) {
	// 	    	case 'p' : 
	// 				strcpy(port, optarg);
	// 	    		break;
	// 	    	default: 
	// 				print_usage(); 
	// 	    		exit(-1);
	// 	    }
	// }
	// if (strcmp(port,"")==0){
	// 	print_usage(); 
	// 	exit(-1);
	// }

	// printf("Port: %s\n", port);
	startServer();
	
	stopServer();


	// registerUser("test");
	// registerUser("test2");
	// registerUser("test3");
	//Antes de hacer el conect insertar un archivo en registered.db -> INSERT INSERT INTO FILES (USER,NAME, DESCRIPTION) VALUES('test', 'test.mp3', 'Best song ever!');

	// connectUser("test", "128.160.46.1", 88);
	// connectUser("test2", "128.160.46.2", 89);
	// connectUser("test3", "128.160.46.3", 45);

	// publishFile("128.160.46.1", "test.txt", "This is a test");
	// publishFile("128.160.46.1", "paquito.docs", "This is a test");
	// publishFile("128.160.46.2", "saray.raw", "un bebé");
	// publishFile("128.160.46.3", "alex.py", "yo mismo");
	// publishFile("128.160.46.3", "test.c", "This is a test");

	//deleteFile("128.160.46.1", "test.txt");

	//disconnectUser("128.160.46.1");

	//unregisterUser("test2");
	//unregisterUser("test3");
	
	return 0;
}
	
