#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

/* Execute the command using this shell program.  */
#define SHELL "/bin/sh"

int my_system (const char *command)
{
  int status;
  pid_t pid;

pid = fork ();

  if (pid == 0)
    {
      /* This is the child process.  Execute the shell command. */
		printf("\n%s\n","PROCESSO FILHO");
		execl (SHELL, SHELL, "-c", command, NULL);
      _exit (EXIT_FAILURE);	
		//printf("\n%s\n","PROCESSO FILHO");
		//printf("\nCMD =  %s\n",command);

    }
  else if (pid < 0)
    /* The fork failed.  Report failure.  */
    status = -1;
  else
    /* This is the parent process.  Wait for the child to complete.  */
    if (waitpid (pid, &status, 0) != pid) {
		status = -1;
		printf("%s","PROCESSO PAI");
		
	}
	

	
  return status;
}

int main(void){
	
	int result;
	
	result = my_system("ls");
	result = my_system("pwd");
	
	return 0;
}