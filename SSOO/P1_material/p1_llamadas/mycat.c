#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main (int argc, char *argv[]){                      //argc is the number os parameters, argv are all the strings typed in the console    
    
    if(argc != 2){                                      //Check if the user has typed a file to open
        printf("Error: you need to enter a file\n");
        return -1;
    }

    int openedFile = open(argv[1], O_RDONLY);           //Open the typed file only with read permissions

    struct stat buf;                                    //Functions imported in order to extract the size of the whole file
    fstat(openedFile, &buf);
    off_t size = buf.st_size;

    char file_buffer [size];                            //Text itself, the buffer is the same size as the file
    ssize_t nbytes;                                     //Size of the text in bytes

    if(openedFile == -1){                               //Check if the file exists
        printf("Error: the file doesn't exit\n");
        return -1;
    } else {
        nbytes = read(openedFile, file_buffer, size);
        close(openedFile);
        
        if(nbytes == 0){                                //Check if the file is empty
            printf("The file is empty\n");
            return 0;
        } else {
            printf("%s", file_buffer);                  //Print the file
        }
    }
    return 0;
}
