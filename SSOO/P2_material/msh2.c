/*-
 * msh.c
 *
 * Minishell C source
 * Show how to use "obtain_order" input interface function
 *
 * THIS FILE IS TO BE MODIFIED
 */

#include <stddef.h>			/* NULL */
#include <stdio.h>			/* setbuf, printf */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/time.h>


extern int obtain_order();		/* See parser.y for description */

int main(void)
{
	char ***argvv;
	//int command_counter;
	int num_commands;
	//int args_counter;
	char *filev[3];
	int bg;
	int ret;

	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

	while (1) 
	{
		fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		num_commands = ret - 1;		/* Line */
		if (num_commands == 0) continue;	/* Empty line */


		if(strncmp("mypwd", argvv[0][0],5)==0){
			char directorio[PATH_MAX];
			if(getcwd(directorio, sizeof(directorio))!= NULL){
				printf("Current dir: %s \n", directorio);
			}else{
				perror("Mypwd error");
			}
		}

		else if(strncmp("mytime", argvv[0][0],6)==0){
			struct timeval inicio, fin;
			gettimeofday(&inicio, NULL);
			int pid;

			pid=fork();

			if(pid==0){
				//solo un archivo por argumento
				if(filev[0] !=NULL){
					close(0);
					open(filev[0], O_RDONLY);
				}
				if (filev[1] !=NULL){
					close(1);
					open(filev[1], O_RDONLY | O_TRUNC | O_CREAT);
				}
				if (filev[2] !=NULL){
					close(2);
					open(filev[2], O_RDONLY | O_TRUNC | O_CREAT);
				}
				execvp(argvv[0][1],argvv[0]+1);

			}

			wait(NULL);
			gettimeofday(&fin, NULL);
			double hora= (double)(fin.tv_sec - inicio.tv_sec);
			printf("time spent: %f secs.\n", hora);
		}
		else if(num_commands==1){
			int pid;

			pid=fork();

			if(pid==0){
				//solo un archivo por argumento
				if(filev[0] !=NULL){
					close(0);
					open(filev[0], O_RDONLY);
				}
				if (filev[1] !=NULL){
					close(1);
					open(filev[1], O_RDONLY | O_TRUNC | O_CREAT);
				}
				if (filev[2] !=NULL){
					close(2);
					open(filev[2], O_RDONLY | O_TRUNC | O_CREAT);
				}
				execvp(argvv[0][0],argvv[0]);

			}
		if(bg==0){
			wait(NULL);
		}else if (bg==1){
			printf("[%d]\n", pid);
			bg=0;
		}
		

		}
		else if(num_commands>1){
			int tubos[2][2];
			pipe(tubos[0]);
			pipe(tubos[1]);

            
			for(int i=0; i<num_commands;i++){
				int pid;
				pid=fork();
				close(tubos[i][0]);
                    		close(tubos[i][1]);
				if(pid==0){
                    		
                   		
                    		if(i==0){
					dup2(tubos[i][1], STDOUT_FILENO);
					if(filev[0] !=NULL){
						close(0);
						open(filev[0], O_RDONLY);
					}
				}

				

				else if(i==num_commands-1){
					dup2(tubos[i-1][0], STDIN_FILENO);
					if (filev[1] !=NULL){
						close(1);
						open(filev[1], O_WRONLY | O_TRUNC | O_CREAT);
					}
					if (filev[2] !=NULL){
						close(2);
						open(filev[2], O_WRONLY | O_TRUNC | O_CREAT);
					}
				}

				else{
					dup2(tubos[i-1][0], STDIN_FILENO);
					dup2(tubos[i][1], STDOUT_FILENO);
				}
				
				execvp(argvv[i][0],argvv[i]);
					

				}
				if(bg==0){
					wait(NULL);
				}else if (bg==1){
					printf("[%d]\n", pid);
					bg=0;
				}
			}
			
		}
		

	
	} //fin while 

	return 0;

} //end main

