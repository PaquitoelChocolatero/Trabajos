#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <unistd.h>             /* Cabecera llamada al sistema gtcwd  */
#include <sys/types.h>          /* Cabeceras llamadas al sistema opendir, readdir y closedir  */
#include <dirent.h>
#include <string.h>


int main(int argc, char *argv[]){

    char actual_dir[PATH_MAX];
    DIR *opened_dir;
    struct dirent *dir;

    if(argc < 2){              /*If arguments are lower than two it means that we are working with the actual directory*/

        if (getcwd(actual_dir, sizeof(actual_dir)) != NULL) {
            
            opened_dir = opendir(actual_dir);
            
            while ((dir = readdir(opened_dir)) != NULL) {
                printf("%s\n", dir->d_name);
            }
            
        } else {
            perror("getcwd() error");
            return -1;
        }
        
    } else if (argc > 2){       /*It can't have more than two arguments*/
        
        printf("Error: introduce only one argument\n");
        return -1;
        
    } else {                    /*It means that we are working with an introduced directory*/

        char *root_directory = "/";

        if(argv[1][0]==root_directory[0]){                                        //If it is a root directory we don't want to concatenate it with the actual one

            opened_dir = opendir(argv[1]);
            if(opened_dir){
                while ((dir = readdir(opened_dir)) != NULL) {
                    printf("%s\n", dir->d_name);
                }
            }else{
                printf("Error: directory doesn't exist\n");
                return -1;
            }

        }else{
            getcwd(actual_dir, sizeof(actual_dir));
            strcat(actual_dir, "/");
            strcat(actual_dir, argv[1]);
            opened_dir = opendir(actual_dir);
            if(opened_dir){                                         //If the directory exists print the containing files
                while ((dir = readdir(opened_dir)) != NULL) {
                    printf("%s\n", dir->d_name);
                }
            }else{
                printf("Error: directory doesn't exist\n");         //Error message
                return -1;
            }
        }
    }
    closedir(opened_dir);
}
