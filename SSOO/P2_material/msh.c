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

extern int obtain_order();		/* See parser.y for description */

int main(void) {
	char ***argvv;
	int command_counter;
	int num_commands;
	int args_counter;
	char *filev[3];
	int ret;
    int bg
    
    int pid;

	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

	while (1) {
		fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		num_commands = ret - 1;		/* Line */
		if (num_commands == 0) continue;	/* Empty line */

/*
 * THE PART THAT MUST BE REMOVED STARTS HERE
 * THE FOLLOWING LINES ONLY GIVE AN IDEA OF HOW TO USE THE STRUCTURES
 * argvv AND filev. THESE LINES MUST BE REMOVED.
 */

/*		for (command_counter = 0; command_counter < num_commands; command_counter++) {
 *			for (args_counter = 0; (argvv[command_counter][args_counter] != NULL); args_counter++) {
 *				printf("%s ", argvv[command_counter][args_counter]);
 *			}
 *			printf("\n");
 *		}
 *	
 *		if (filev[0] != NULL) printf("< %s\n", filev[0]);// IN
 *
 *		if (filev[1] != NULL) printf("> %s\n", filev[1]);// OUT
 *	
 *		if (filev[2] != NULL) printf(">& %s\n", filev[2]);// ERR
 *	
 *		if (bg) printf("&\n");
 */

/*
 * END OF THE PART TO BE REMOVED
 */

/*
 *  NORMAL COMMANDS
 */	
            if (num_commands == 1) {

				//Do the fork
                pid = fork();
				if (pid == 0) {

					//If there is an entry we close the current one and redirect to the new one
					if (filev[0] != NULL) {
						close(0);
						open(filev[0], O_RDONLY);
					}

					//The same but printing on screen
					if (filev[1] != NULL) {
						close(1);
						open(filev[1], O_CREAT|O_TRUNC|O_RDONLY);
					}

					//The same but with the erros.
					if (filev[2] != NULL) {
						close(2);
						open(filev[2], O_CREAT|O_TRUNC|O_RDONLY);
					}

					//Print child PID
					printf("Hijo: %i\n\n", pid);
					//Do the exec
					execvp(argvv[0][0], argvv[0]);
				}

				//Parent code, so wait for the child to finish
				//if its the only one in the order
				else if (bg == 0) {
					printf("\nEsperando al hijo: %i\n", pid);
					waitpid(pid, NULL, 0);
				}
				else if (bg == 1) bg=0;

				else printf("[%i]\n", pid);
}

    } //fin while 

	return 0;

} //end main

