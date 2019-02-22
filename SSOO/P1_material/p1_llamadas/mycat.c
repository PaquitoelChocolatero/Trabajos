#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main (int argc, char *argv[]){                      //argc is the number os parameters, argv are all the strings typed in the console    
    
    const int BUFF = 1024;

    if(argc != 2){                                      //Check if the user has typed a file to open
        printf("Error: you need to enter a file\n");
        return -1;
    }

    int openedFile = open(argv[1], O_RDONLY);           //Open the typed file only with read permissions

    char file_buffer [BUFF];                            //Text itself, the buffer is the same size as the file
    ssize_t nbytes = read(openedFile, file_buffer, BUFF);//Bytes read each time
    
    if(nbytes == 0){                                //Check if the file is empty
        printf("The file is empty\n");
        return 0;
    }else if(openedFile == -1){                               //Check if the file exists
        printf("Error: the file doesn't exit\n");
        return -1;
    }

    do{
        write(STDOUT_FILENO, file_buffer, nbytes);                  //Print the file
        nbytes = read(openedFile, file_buffer, BUFF);
    }while(nbytes != 0);
    
    close(openedFile);
    return 0;
}
