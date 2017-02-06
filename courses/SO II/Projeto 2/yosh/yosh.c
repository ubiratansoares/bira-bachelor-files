/*
 *  Arquivo : misha8.c
 *  Contém a implementacao de um SHELL Unix funcional.
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
#include <signal.h>
#include <pwd.h>
#include <histedit.h>
#include <stddef.h>

#include "parser.h"
#include "set_prompt.h"
#include "exec.h"
#include "signals.h"
#include "queue.h"


int sig_flag = 1; // Variavel global para flag

int main (int argc, char **argv) {
	
	command_line_t command_line; 	// Estrutura para armazenar a linha jah parseada digitada pelo usuario
	char *buffer; 					// Pega a entrada do teclado digitida pelo usuario
   	size_t buffer_size; 			// Tamanho da linha de entrada 
   	size_t read_bytes; 				// Numero de bytes lidos
   	pid_t pid; 						// Numero do processo atual
   	int status;						// Variavel auxiliar
   	int redirect; 					// Indicador se linha comando possui algum redirecionamento de E/S
   	struct passwd *user_info; 		// Informacoes sobre o usuario do SHELL

   	FILE *fpout = NULL;
   	FILE *fperr = NULL;
   	FILE *fpin = NULL;

   	char *prompt = NULL; 			// String de prompt
   	queue qjobs; 					// Fila para armazenar jobs
   	job job;						// Variavel para armazenar um unico job
   
	EditLine *el;					// Linha de comando fornecida por libedit
	int count;						// Quantidade de caracteres lido da entrada
	

	init_queue(&qjobs); 							// Inicializando fila de jobs
	el = el_init(argv[0], stdin, stdout, stderr);	// editline usarah as streams default
	el_set(el, EL_PROMPT, &prompt);					// Prompt eh definida pelo usuario de editline
	el_set(el, EL_EDITOR, "emacs");					// O estilo do editor eh Emacs
	el_set(el, EL_SIGNAL, 0);						// Desabilitamos os handlers de sinais de editline

	// Inicializando o historico de comandos
	myhistory = history_init();
	
	if (myhistory == 0) {
    	fprintf(stderr, "CANT INITIALIZE HISTORY\n");
    	return 1;
	}

	// Ajustando o tamanho do historico

	history(myhistory, &ev, H_SETSIZE, 800);

	// Ajustando funcoes de callback para o historico

	el_set(el, EL_HIST, history, myhistory);
   
	// Obtendo informacoes do usuario
   
   	user_info = getpwnam(getlogin());
   	chdir(user_info->pw_dir);
   
   	/* Build the initial prompt string */  
   	set_prompt(&prompt,user_info);
   	printf("\nHello %s!\n",user_info->pw_name);

	// Associando handlers com seus respectivos sinais

#ifdef WITH_SIGNAL

   signal(SIGINT, get_signal_SIGINT);
   signal(SIGTSTP, get_signal_SIGTSTP);
   signal(SIGCONT, get_signal_SIGCONT);

#endif
 
 
	// Loop principal do SHELL

	do{

		// Leitura da linha de comando
		
      	/* Read the command line. */
		buffer = NULL;


		line = el_gets(el, &count);

#ifdef WITH_READLINE     
      	buffer = readline(prompt);
      	read_bytes = strlen(buffer);
#endif

		// Fazendo parser da linha lida e preenchendo estrutura command_line

		parse_command_line(buffer,&command_line,el) ;
      
      	if( command_line.argc > 0 ){

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
