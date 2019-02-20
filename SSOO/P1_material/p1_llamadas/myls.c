#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <unistd.h>             /* Cabecera llamada al sistema gtcwd  */
#include <sys/types.h>          /* Cabeceras llamadas al sistema opendir, readdir y closedir  */
#include <dirent.h>
#include <string.h>


int main(int argc, char *argv[])
{
    
    if(argc < 2){              /*If arguments are lower than two it means that we are working with the actual directory*/
        
        
        
    } else if (argc > 2){       /*It can't have more than two arguments*/
        
        printf("Error: introduce only one argument\n");
        
    } else {                    /*It means that we are working with an introduced directory*/
        
        
        
    }
    
}
