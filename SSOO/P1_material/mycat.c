#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main (int argc, char *argv[]){                      //argc is the number os parameters
                                                        //argv are all the strings typed in the console
    if(argc != 2){                                      //Check if the user has typed a file to open
        printf("Error: You need to enter a file\n");
        return 0;
    }

    int openedFile = open(argv[1], O_RDONLY);           //Open the typed file only with read permissions

    if(openedFile == -1){                               //Check if the file exists
        perror(openedFile);
        return EXIT_FAILURE;
    }
    
    char file [255];
    itoa(openedFile, file, 10);

    printf("%s\n", file);                         //Print the file(?)

    close(openedFile);

    return 0;
}
