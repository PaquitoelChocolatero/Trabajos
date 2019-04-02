#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/time.h>

extern int obtain_order();
char ***argvv;
int num_commands;
char *filev[3];
int ret;
int bg;
int pid;
char *getcwd(char *buf, size_t size);
struct timeval start, end;

//Check for background command, otherwise wait for every child
void background(){  
    if(bg){
        printf("[%d]\n", pid);
        bg = 0;
    }else{
        for(int i=0; i<num_commands; i++){
            wait(NULL);
        }
    }
}

//Redirect either to Standard Output(and error), Standard Input or all of them
void redirect(int type){
    if(type==0){
        //If there is an entry we close the current one and redirect to the new one
        if (filev[0] != NULL) {
            close(0);
            open(filev[0], O_RDONLY);
        }
    }else if(type==1){
        //The same but printing on screen
        if (filev[1] != NULL) {
            close(1);
            open(filev[1], O_CREAT|O_TRUNC|O_WRONLY);
        }
        //Error output
        if (filev[2] != NULL) {
            close(2);
            open(filev[2], O_CREAT|O_TRUNC|O_WRONLY);
        }
    }else{ //When redirecting everything
        if (filev[0] != NULL) {
            close(0);
            open(filev[0], O_RDONLY);
        }
        if (filev[1] != NULL) {
            close(1);
            open(filev[1], O_CREAT|O_TRUNC|O_WRONLY);
        }
        if (filev[2] != NULL) {
            close(2);
            open(filev[2], O_CREAT|O_TRUNC|O_WRONLY);
        }
    }
}

//Look for the current working directory
void pwd(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current dir: %s\n", cwd);
    } else {
        perror("​Mypwd error​");
    }
}

//myTime function that returns the time spent in a command
double myTime(){

    gettimeofday(&start, NULL);

    //Do the fork
    pid = fork();
    if (pid == 0) {
        redirect(2); 
        //Do the exec
        execvp(argvv[0][1], argvv[0]+1);
    }

    wait(NULL);

    gettimeofday(&end, NULL);

    //Calculate the final time spent
    return (double)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000000;
}

int main(void) {
    
	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

    int father = getpid();

	while (1) {

        fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		num_commands = ret - 1;		/* Line */
		if (num_commands == 0) continue;	/* Empty line */

/*************************************************     SPECIAL COMMANDS     **************************************************************************************************************************/
        
        //MYTIME
        if (strncmp("mytime", argvv[0][0], 6) == 0) printf("Time spent: %f secs.\n", myTime());
        
        //MYPWD
        else if (strncmp("mypwd", argvv[0][0], 5) == 0) pwd();

/*************************************************     NORMAL COMMANDS     **************************************************************************************************************************/
        
        else if(num_commands > 1){

            //Each command has its pipe inside this array of pipes
            int p[num_commands-1][2];

            //Go command by command
            for(int i=0; i<num_commands; i++){
                
                //Create a pipe
                pipe(p[i]);

                //Mark every end of the pipes as "closable", and so, they will close after an exec
                fcntl(p[i][0], F_SETFD, FD_CLOEXEC);
                fcntl(p[i][1], F_SETFD, FD_CLOEXEC);

                //Create each son
                pid=fork();
                
                //CHILD
                if(pid==0){
                    //First command
                    if(i==0) {
                        dup2(p[i][1], STDOUT_FILENO);//Write
                        redirect(0);
                    }
                    //Last command
                    else if(i==num_commands-1){
                        dup2(p[i-1][0], STDIN_FILENO);//Read
                        redirect(1); 
                    }
                    //Middle commands
                    else{
                        dup2(p[i-1][0], STDIN_FILENO);//Read
                        dup2(p[i][1], STDOUT_FILENO);//Write
                    }
                    printf("%i\n", i);
                    //Exec
				    execvp(argvv[i][0], argvv[i]);
                }   
            }
            if(getpid()==father){
                for(int i=0; i<num_commands; i++){
                    close(p[i][0]);
                    close(p[i][1]);
                }
            }
            //Check for background
            background(); 
        }else if (num_commands == 1) {
            //Do the fork
            pid = fork();
            if (pid == 0) {
                //Redirect with every standard input/output
                redirect(2);
                //Run the command
                execvp(argvv[0][0], argvv[0]);
            }
            //Check for background
            background();
        }
    }//end while 

	return 0;

}
