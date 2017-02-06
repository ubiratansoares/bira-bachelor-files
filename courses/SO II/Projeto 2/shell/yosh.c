/*
 *  This file is part of Yosh.
 *
 *  Yosh is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Yosh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>
#ifdef WITH_READLINE
 #include <readline/history.h>
 #include <readline/readline.h>
#endif
#include "parser.h"
#include "set_prompt.h"
#include "exec.h"
#include "signals.h"
#include "queue.h"

/* global signal flag */
int sig_flag = 1;

int main (int argc, char **argv) 
{
   /* Structure that stores tokens in the argv field, and the number of tokens in the argc field */
   command_line_t command_line;
   /* Stores the string with the command typed by the user */
   char *buffer;
   /* Stores the size of the buffer */
   size_t buffer_size; 
   size_t read_bytes;
   /* Number of the current process */
   pid_t pid;
   int status;
   /* Flag indicates if the command line has redirecting like pipe */
   int redirect;
   /* Structure that stores information about the user */
   struct passwd *user_info;
   /* file pointers used for redirecting */
   FILE *fpout = NULL;
   FILE *fperr = NULL;
   FILE *fpin = NULL;
   /* stores the prompt string */
   char *prompt = NULL;
   /* queue that stores jobs */
   queue qjobs;
   /* stores one job structure */
   job job;
   
   init_queue(&qjobs);
   
   /* Introducing the shell */
   printf("\nYOSH - Your Open SHell");
   printf("\nMaking your world better..");
   
   /* Catch user information */
   user_info = getpwnam(getlogin());
   chdir(user_info->pw_dir);
   
   /* Build the initial prompt string */  
   set_prompt(&prompt,user_info);
   printf("\nHello %s!\n",user_info->pw_name);

#ifdef WITH_SIGNAL
   /* This is where we associate the signals to their functions */
   signal(SIGINT, get_signal_SIGINT);
   signal(SIGTSTP, get_signal_SIGTSTP);
   signal(SIGCONT, get_signal_SIGCONT);
#endif
 
   /* This is the main infinity loop */
   do
   {
      /* Read the command line. */
      buffer = NULL;
#ifdef WITH_READLINE     
      buffer = readline(prompt);
      read_bytes = strlen(buffer);
#endif
#ifndef WITH_READLINE
       buffer_size = 0; 
       fflush(stdout);   
       printf("%s", prompt);
       read_bytes = getline(&buffer, &buffer_size, stdin);
       buffer[read_bytes-1] = 0;   /* Strip newline. */
#endif
      /* Call the parser to fill in the command_line structure. */
      parse_command_line(buffer,&command_line) ;
      
      /* If there is no command to run, then there is nothing to do */
      if( command_line.argc > 0 )
      {
#ifdef WITH_READLINE         
         /* Add this command to the history */
         add_history(buffer);
#endif
         /* Built-in commands - Needs to put in another module */
         if( strcmp(command_line.argv[0],"exit") == 0 ) /* exit - shell is finished */
            exit(0);
         else if( strcmp(command_line.argv[0],"cd") == 0 ) /* cd - changes the current directory */
         {
            if( command_line.argc == 1 )
               chdir(user_info->pw_dir);
            else if( strcmp(command_line.argv[1],"~") == 0 )
               chdir(user_info->pw_dir);
            else   
               chdir(command_line.argv[1]);
	         /* Needs to change prompt */
            set_prompt(&prompt,user_info);
         }
         else if( strcmp(command_line.argv[0],"fg") == 0 ) /* fg - brings process to foreground */
         {
            if( command_line.argc == 1 )
               kill(dequeue(&qjobs),SIGCONT); 
            else
               kill(atoi(command_line.argv[1]),SIGCONT);
         }
         else if( strcmp(command_line.argv[0],"bg") == 0 ) /* bg - brings process to background */
         {
            if( command_line.argc == 1 )
            {
               enqueue(command_line.argv[0],getpid(),&qjobs);
               kill(getpid(),SIGTSTP);
            }   
            else
            {
               enqueue("job",atoi(command_line.argv[1]),&qjobs);
               kill(atoi(command_line.argv[1]),SIGTSTP);
            } 
         }
         else if( strcmp(command_line.argv[0],"jobs") == 0 ) /* execute jobs */
         {
            job.pid = 1;
            printf("Jobs:\n");
            while( job.pid != -1 )
            {  
               printf("%s\t%d\n",qjobs.q[qjobs.current].command,qjobs.q[qjobs.current].pid);
               job = previous_item(&qjobs);
            }
         }
  
         
         /* Create a child process to execute the user command. */ 
         else 
         {
            pid = fork();
            if ( pid == -1 )      /* pid < 0 means error */
               fatal();
            else if ( pid == 0 )  /* This is the child process */
            {                                           

               /* changes I/O if has redirecting */
               redirect = redirect_io(redirect,fpout,fpin,fperr,command_line);
               
               /* if there is background command */
               if (strcmp(command_line.argv[command_line.argc - 1],"&") == 0 )
               {
                  command_line.argv[command_line.argc - 1] = NULL;
                  raise(SIGTTOU);
               } 
               
               /* association with predefined signal handlers */
               signal(SIGINT,get_signal_SIGINT);
               signal(SIGTSTP,get_signal_SIGTSTP);
               
               /* execute the command using the PATH environmnent variable, if the command exists */
               if (execvp(command_line.argv[0] , command_line.argv) == -1)
               {  
                  fatal(); 
                  exit(0); /* Exit the son process created, there is no program to run */
               }
               
               /* test if ocurred redirecting and recover standard i/o */
               test_redirecting(&redirect,fpout,fpin,fperr);
                          
            }
            else  /* if pid > 0 then this is the father. */
            { 
               /* Normally, father waits for the child to terminate.
                * If we want the user command to be executed in background,
                * we may bypass the waitpid */
               signal(SIGTSTP,get_signal_SIGTSTP);
               if( strcmp(command_line.argv[command_line.argc - 1],"&") == 0 )
                  printf("Background [%d]\t%s\n",getpid(),command_line.argv[0]);
               else   
                  waitpid(pid, &status, WUNTRACED);
            }
         }
      }
      free(buffer);    /* free memory allocated to buffer */
   } while(sig_flag);  /* while there is not ^C signal */
   
   free_queue(&qjobs); /* free memory allocated to the queue of jobs */
   free(prompt);       /* free memory allocated to the prompt string */
   return EXIT_SUCCESS;
}
/* End of Main Program */
