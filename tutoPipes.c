#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


int main(int argc, char *argv[]){
    
    //PIPE
    int p[2];
    pipe(p);

    //Create child
    int pid;
    pid=fork();

    //CHILD
    if(pid==0){
        close(p[1]);//Son is only going to read, close the writing pipe
        
        dup2(p[0], STDIN_FILENO);//Redirect the pipe to the standard output
        close(p[0]);//Close the pipe

        execlp("grep", "grep", "hola", NULL);//exec listing the parameters inheriting from the father //Doesn't work but...

//Just reading a doc and printing it through the other one
/*
        //Reading from pipe in chunks of 1024
        char print[1024];
        int bytes = read(STDIN_FILENO, print, 1024);
        while(bytes>0){
            write(STDOUT_FILENO, print, bytes);//Writing on screen   
            bytes = read(STDIN_FILENO, print, 1024);
        }
*/

    }
    
    //FATHER
    else{
        close(p[0]);//Father only writes
        
        //Reading desired file in chunks of 1024
        int openedFile = open(argv[1], O_RDONLY);
        char data[1024];
        int bytes = read(openedFile, data, 1024);
        while(bytes>0){
            
            write(p[1], data, bytes);//Writing read stuff in the pipe
            
            bytes = read(openedFile, data, 1024);
        } 
        //Close the file and the pipe
        close(openedFile);
        close(p[1]);
        
        //Wait for the child to die
        wait(NULL);
    }
}
