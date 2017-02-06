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

#include "exec.h"
#include "parser.h"

int redirect_io(int redirect, FILE *fpout, FILE *fpin, FILE *fperr, command_line_t command_line)
{
   int i;
   fpout = stdout;
   fperr = stderr;
   fpin = stdin;

   for( i = 1 ; i < command_line.argc; i++ )
   {
      if ( strcmp(command_line.argv[i],">") == 0 )
      {
         fclose(stdout);
         stdout = fopen(command_line.argv[i+1],"w");
         if( !stdout )
            fatal();
         command_line.argv[i] = NULL;
         redirect = 1;
         break;
      }
      else if( strcmp(command_line.argv[i],">>") == 0 )
      {
         fclose(stdout);
         stdout = fopen(command_line.argv[i+1],"a");
         if( !stdout )
            fatal();
         command_line.argv[i] = NULL;
         redirect = 1;
         break;
      }
      else if( strcmp(command_line.argv[i],"2>") == 0 )
      {
         fclose(stderr);
         stderr = fopen(command_line.argv[i+1],"a");
         if( !stderr )
            fatal();
         command_line.argv[i] = NULL;
         redirect = 2;
         break;
      }
      else if( strcmp(command_line.argv[i],"<") == 0 )
      {
         fclose(stdin);
         stdin = fopen(command_line.argv[i+1],"r");
         if( !stdin )
            fatal();
         command_line.argv[i] = NULL;
         redirect = 2;
         break;
      }
      else if( strcmp(command_line.argv[i],"|") == 0 )
      {
         /* Try to put pipes here */
         exec_pipe(command_line);
      }
   }
   return(redirect);
}

/* This function tests if has ocurred redirecting after the execution of the command, then it recovers the standard io */ 
void test_redirecting(int *redirect, FILE *fpout, FILE *fpin, FILE *fperr)
{
   if(*redirect == 1) /* If ocurred redirecting for stdout */
   {
      fclose(stdout);
      stdout = fpout;
   }
   if(*redirect == 2) /* If ocurred redirecting for stderr */
   {
      fclose(stderr);
      stderr = fperr;
   }
   if(*redirect == 3) /* If ocurred redirecting for stdin */
   {
      fclose(stdin);
      stdin = fpin;
   }
   *redirect = 0;
}

/* Exec pipe if exists */
void exec_pipe(command_line_t command_line)
{
   /* array with file descriptors used by pipe */
   int pipefd[2];
   /* iterators */
   int i, j;
   /* stores commands found in command_line.argv */
   char *commands[32];
   command_line_t com_lines[32];
   
   /* initializes variables with null pointer */
   for(i = 0; i < 32; i++)
      commands[i] = NULL;
   
   /* Now we catch each command and stores in commands[i] */
   j = 0;
   commands[j] = (char *)malloc(256*sizeof(char));
   strcpy(commands[j],"");  
   for( i = 0; i < command_line.argc; i++ )
   {
      if( strcmp(command_line.argv[i],"|") == 0 )
      {
         j++;
         commands[j] = (char *)malloc(256*sizeof(char));
         strcpy(commands[j],"");
      }
      else
      {
         strcat(commands[j]," ");
         strcat(commands[j],command_line.argv[i]);
      }
   }
   /* create a struct command_line for each command in array commands */
   for( i = 0; i <= j; i++ )
   {
      /*printf("\n>>>comando %d: %s",i,commands[i]);*/
      parse_command_line(commands[i], &com_lines[i]);
   }
         
   /* create the pipe */
   if (pipe (pipefd) < 0)
      fatal();

   /* exec the sequence of commands with pipes */
   do_pipe(com_lines,0,j+1);
   
   /* free memory allocated */
   for( i = 0; i < j; i++ )
      free(commands[i]); 
}

/* Execute commands with pipe recurely */
void do_pipe(command_line_t com_lines[32], int begin, int num_commands)
{
   pid_t pid;
   int status;
   int pipefd[2];

   if (begin == num_commands - 1) 
      execvp(com_lines[begin].argv[0],com_lines[begin].argv);
   
   else 
   {
      if( pipe(pipefd) < 0)
         fprintf(stdout,"Can't create pipe file descriptors..");

      /* Fork it */
      pid = fork();
      
      if (pid < 0)
         fprintf(stdout,"Can't create pipe..\n");

      if( pid == 0 ) /* the Son :) */
      {
         dup2(pipefd[0],0); /* make stdin the same as pipefd[0] */
         close(pipefd[1]);

         do_pipe(com_lines,begin+1,num_commands);
      }
      else 
      { /* the Father >( */
         dup2(pipefd[1],1);  /* make stdout the same as pipefd(1) */
         close(pipefd[0]);
         
         execvp(com_lines[begin].argv[0],com_lines[begin].argv);
         fflush(stdout);     /* flushes stdout before printing the prompt */
         waitpid(pid, &status, 0);
      }
   }
}
