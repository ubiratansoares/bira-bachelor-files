/*************************************************************************
* SSC0141 - Sistemas Operacionais II
* MISH - The Mini Shell
*
*
* Trabalho desenvolvido pelos alunos:
* 
* Ubiratan Soares
* Ulisses Soares
* Vinicius Grippa
*
* Data : 07/10/2009
*************************************************************************/


#include <histedit.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char* prompt(EditLine *e) { return "mishA8 > "; }

int main(int argc, char *argv[]) {

	
	EditLine *el;			// Linha de comando fornecida por libedit
	History *myhistory;		// Historico de comandos fornecido por libedit
	const char *line;		// Linha de entrada
	int count;				// Quantidade de caracteres lido da entrada
	HistEvent ev;			// Variavel associada ao historico	
	int flag, k;			// Variaveis auxiliares
	const LineInfo *linf;	// Variavel associada a linha de comando
	int argsnumber;			// Numero de argumentos da linha de entrada
	const char **lineargs;	// Vetor de argumentos de entrada
	Tokenizer *parsedline;	// Tokenizer fornecido por libedit
	int cursorc;			// Variavel associada a tokenizer
	int cursoro;			// Variavel associada a tokenizer
	int status;				// Status para waitpid
	int childpid;			// Processo filho criado para executar comando

	// Variavel que guarda o diretorio onde serao procurados os comandos
	char path[10] = "/bin/";
	
	//printf("%s",path);

	// Inicializando a linha de comando 
    
	el = el_init(argv[0], stdin, stdout, stderr);	// editline usarah as streams default
	el_set(el, EL_PROMPT, &prompt);	// Prompt eh definida pelo usuario de editline
	el_set(el, EL_EDITOR, "emacs");	// O estilo do editor eh Emacs
	el_set(el, EL_SIGNAL, 0);	// Desabilitamos os handlers de sinais de editline

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

	// Loop principal do shell
	
	while (1) {
	
	// Obtendo a linha de entrada via editline
	
    line = el_gets(el, &count);

	if (count > 0) {
	
	
			// Inicializando variaveis para o tokenizer
			
			parsedline = tok_init(NULL);
			lineargs = NULL;	
	
	
			// Adicionando a linha de comando lida no historcio
			
			history(myhistory, &ev, H_ENTER, line);
			
			// Obtendo variavel de informacao de linha de comando
			
			linf = el_line(el);
			
			// Parser da linha de comando usando editline
			
			flag = tok_line(parsedline,linf,&argsnumber,&lineargs,&cursorc,&cursoro);
			
			// flag == 0 indica parser realizado com sucesso
			
			
			if(flag == 0){


				// Determinando caminho do comando a ser executado
			
				strcat(path,lineargs[0]);
				//printf("%s",path);

				// Criando processo filho	
								
				childpid = fork();
				
				if (childpid == 0) {
					
			      // Executando o comando lido no processo filho			
	
					execv(path,lineargs);
					exit(0);
	
			    }
				else if (childpid < 0){
					
					// Um erro ocorreu na criacao do processo filho
			    
					fprintf(stderr,"\nCANT CREAT CHILD PROCESS !!!\n");
					
					}
					else{

						// Processo pai
						// Aguardando execucao do processo fiho para continuar

						if (waitpid (childpid,NULL, 0) != childpid) fprintf(stderr,"ERROR ON CHILD PROCESS\n");

					}	

				}			
			
    }// end if


	// Restaurando diretorio padrao para buscar programas			
	strcpy(path,"/bin/");

	// Limpando estrutura Tokenizer para a proxima linha de comando

	tok_end(parsedline);
	printf("\n");

	}// end while
  
  
	// Desalocando historico e linha de comando

	history_end(myhistory);
	el_end(el);
  
	return 0;
	
} //end mish




