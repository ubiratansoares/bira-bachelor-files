/* parent.c */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){

	pid_t pid;
	int rv;
	int	commpipe[2];		/* This holds the fd for the input & output of the pipe */

	/* Setup communication pipeline first */
	if(pipe(commpipe)){
		fprintf(stderr,"Pipe error!\n");
		exit(1);
	}

	/* Attempt to fork and check for errors */
	if( (pid=fork()) == -1){
		fprintf(stderr,"Fork error. Exiting.\n");  /* something went wrong */
		exit(1);        
	}

	if(pid){
		/* A positive (non-negative) PID indicates the parent process */
		dup2(commpipe[1],1);	/* Replace stdout with out side of the pipe */
		close(commpipe[0]);		/* Close unused side of pipe (in side) */
		setvbuf(stdout,(char*)NULL,_IONBF,0);	/* Set non-buffered output on stdout */
		sleep(2);
		printf("Hello\n");
		sleep(2);
		printf("Goodbye\n");
		sleep(2);
		printf("exit\n");
		wait(&rv);				/* Wait for child process to end */
		fprintf(stderr,"Child exited with a %d value\n",rv);
	}
	else{
		/* A zero PID indicates that this is the child process */
		dup2(commpipe[0],0);	/* Replace stdin with the in side of the pipe */
		close(commpipe[1]);		/* Close unused side of pipe (out side) */
		/* Replace the child fork with a new process */
		if(execl("child","child",NULL) == -1){
			fprintf(stderr,"execl Error!");
			exit(1);
		}
	}
	return 0;
}




/* child.c */


#include <stdio.h>
#include <string.h>

int main(){
	char string[100];
	
	printf("Child Process\n");
	printf("-------------\n");
	do{
		printf("Enter Command: ");
		fflush(stdout);				/* Must flush to see command prompt */
		fgets(string,100,stdin);
		printf("%s\n",string);		/* No flush necessary because new line flushes */
	}while(!strstr(string,"exit"));
	
	return 0;
}




