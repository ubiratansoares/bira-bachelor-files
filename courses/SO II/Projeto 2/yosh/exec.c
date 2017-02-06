/*
 *  Arquivo : exec.c
 *  Contém a implementacao para exec.h
 *
 *	Esse arquivo faz parte de uma implementacao basica de um SHELL Unix, parte dos
 *	dos trabalhos de disciplina de Sistemas Operacionais II (SSC 0141)
 *
 *	Autores:
 *	Ubiratan Soares
 *	Ulisses Soares
 *  Vinicius Grippa
 *
 *	Data : 16/12/2009    
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


// Funcao : redirect_io(redirect, fpout,fpin,fperr,command_line)
// Essa funcao faz o redirecionamento de E/S entre processos, de acordo com a especificao de comando


int redirect_io(int redirect, FILE *fpout, FILE *fpin, FILE *fperr, command_line_t command_line){
	
   int i;
   fpout = stdout;
   fperr = stderr;
   fpin = stdin;

   for( i = 1 ; i < command_line.argc; i++ ){
	
      if ( strcmp(command_line.argv[i],">") == 0 ){
	
         fclose(stdout);

         stdout = fopen(command_line.argv[i+1],"w");

         if(!stdout) fatal();

         command_line.argv[i] = NULL;

         redirect = 1;

         break;
      }

      else if( strcmp(command_line.argv[i],">>") == 0 ){
	
         fclose(stdout);

         stdout = fopen(command_line.argv[i+1],"a");

         if(!stdout) fatal();

         command_line.argv[i] = NULL;

         redirect = 1;

         break;
      }

      else if( strcmp(command_line.argv[i],"2>") == 0 ){
	
         fclose(stderr);

         stderr = fopen(command_line.argv[i+1],"a");

         if(!stderr) fatal();


         command_line.argv[i] = NULL;

         redirect = 2;

         break;
      }

      else if( strcmp(command_line.argv[i],"<") == 0 ){
	
         fclose(stdin);

         stdin = fopen(command_line.argv[i+1],"r");

         if(!stdin) fatal();

         command_line.argv[i] = NULL;

         redirect = 2;

         break;
      }

      else if( strcmp(command_line.argv[i],"|") == 0 ){
	
         exec_pipe(command_line);
      }

   }//end for

   return(redirect);

}//end redirect_io


// Funcao : test_redirecting(redirect,fpout,fpin,fperr)
// Testa se ocorreu redirecionamento depois da execucao e recupera a entrada padrao

void test_redirecting(int *redirect, FILE *fpout, FILE *fpin, FILE *fperr){

	// O redirecionamento ocorreu para stdout

   if(*redirect == 1) {
	
      fclose(stdout);
      stdout = fpout;
   }


	// O redirecionamento ocorreu para stderr

   if(*redirect == 2){
	
      fclose(stderr);
      stderr = fperr;
   }


	// O redirecionamento ocorreu para stdin

   if(*redirect == 3){
	
      fclose(stdin);
      stdin = fpin;
   }

   *redirect = 0;

}//end test_redirecting

/* Exec pipe if exists */

// Funcao : exec_pipe(command_line)
// Faz a separacao da linha de comando digita de usuario para identificar comandos e os pipes entre eles

void exec_pipe(command_line_t command_line){
	
   int pipefd[2];	// Descritores de arquivo usados pelo pipe
   int i, j;
   char *commands[32];	// Comandos obtidos da linha de comando
   command_line_t com_lines[32];	// Vetor auxiliar de linhas de comando
   

   for(i = 0; i < 32; i++)
      commands[i] = NULL;
   
   /* Now we catch each command and stores in commands[i] */

	// Obtendo cada comando e separando para o processo de pipe 

   j = 0;

   commands[j] = (char *)malloc(256*sizeof(char));

   strcpy(commands[j],""); // Primeiro comando eh a string vazia  

   for(i = 0; i < command_line.argc; i++){
	
      if(strcmp(command_line.argv[i],"|") == 0 ){ // Se  o comando for um pipe, pula e coloca uma string vazia no lugar
	
         j++;
         commands[j] = (char *)malloc(256*sizeof(char));
         strcpy(commands[j],"");
      }

      else{
	
		// Armazena o comando no vetor
	
         strcat(commands[j]," ");
         strcat(commands[j],command_line.argv[i]);
      }

   }//end for


   // Criando uma estrutura command_line para cada comando no vetor de comandos

   for( i = 0; i <= j; i++ ){
	
      // printf("\n>>>comando %d: %s",i,commands[i]);
      parse_command_line(commands[i], &com_lines[i]);
   }

	// Criando o pipe
         
   if (pipe (pipefd) < 0) fatal();

   // Executando a sequencia de comandos com pipes

   do_pipe(com_lines,0,j+1);
   
   // Liberando vetor de comandos

   for( i = 0; i < j; i++ ) free(commands[i]); 

}//end exec_pipe

// Funcao : do_pipe(com_lines,begin,num_commands)
// Executa o pipe entre os comandos recursivamente

void do_pipe(command_line_t com_lines[32], int begin, int num_commands)
{
   pid_t pid;
   int status;
   int pipefd[2];

   if (begin == num_commands - 1) 
      execvp(com_lines[begin].argv[0],com_lines[begin].argv);
   
   else {
	
      if( pipe(pipefd) < 0)
         fprintf(stdout,"CANT CREATA PIPE FILE DESCRIPTORS.");

      pid = fork();
      
      if (pid < 0) fprintf(stdout,"CANT CREAT A PIPE\n");

      if( pid == 0 ){ // Processo filho
      
         dup2(pipefd[0],0); /* make stdin the same as pipefd[0] */

         close(pipefd[1]);

         do_pipe(com_lines,begin+1,num_commands);
      }
      else { // Processo pai
	 
         dup2(pipefd[1],1);  /* make stdout the same as pipefd(1) */
         close(pipefd[0]);
         
         execvp(com_lines[begin].argv[0],com_lines[begin].argv);
         fflush(stdout);     // Flush na saida padrao antes de imprimir o prompt
         waitpid(pid, &status, 0);

      }//end else

   }//end else

}//end do_pipe
