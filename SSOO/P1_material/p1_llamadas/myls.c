#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <unistd.h>             /* Cabecera llamada al sistema gtcwd  */
#include <sys/types.h>          /* Cabeceras llamadas al sistema opendir, readdir y closedir  */
#include <dirent.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char actual_dir[PATH_MAX];

    if(argc < 2){              /*If arguments are lower than two it means that we are working with the actual directory*/

	if (getcwd(actual_dir, sizeof(actual_dir)) != NULL) {
		printf("Current working dir: %s\n", actual_dir);
	} else {
		perror("getcwd() error");
		return 1;
   	}
	return 0;
        
    } else if (argc > 2){       /*It can't have more than two arguments*/
        
        printf("Error: introduce only one argument\n");
        return 0;
        
    } else {                    /*It means that we are working with an introduced directory*/
        
        getcwd(actual_dir, sizeof(actual_dir));
	strcat(actual_dir, argv[1]);
        printf("Current working dir: %s\n", actual_dir);
    }
    
}
