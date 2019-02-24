#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    
    char actual_dir[PATH_MAX];
    DIR *opened_dir;
    struct dirent *dir;

    if(argc < 2){              /*If arguments are lower than two it means that we are working with the actual directory*/

        if (getcwd(actual_dir, sizeof(actual_dir)) != NULL) {
            
            opened_dir = opendir(actual_dir);
            while ((dir = readdir(opened_dir)) != NULL) {
                
                if (dir->d_type == DT_REG){
                    
                    int file = open(dir->d_name,O_RDONLY);
                    int size = lseek(file, 0, SEEK_END);
                    
                    printf("%s\t", dir->d_name);
                    printf("%d\n", size);
                    
                    close(file);
                    
                }
            }
            
        } else {
            
            perror("getcwd() error");
        
        }
        return 0;
        
    } else {       /*It can't have more than two arguments*/
        
        printf("Error: introduce only one argument\n");
        return 0;
        
    }
    
    closedir(opened_dir);
    
}
