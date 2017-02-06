/*
 *  Arquivo : parser.c
 *  Contém a implementacao da interface parser.h.
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

#include <stdlib.h>
#include <histedit.h>
#include <stddef.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void parse_command_line(char *buffer, command_line_t *command_line, Editline *el){
	

	const char *line;		// Linha de entrada
	int count;				// Quantidade de caracteres lido da entrada	
	int flag, k;			// Variaveis auxiliares
	const LineInfo *linf;	// Variavel associada a linha de comando
	int argsnumber;			// Numero de argumentos da linha de entrada
	const char **lineargs;	// Vetor de argumentos de entrada
	Tokenizer *parsedline;	// Tokenizer fornecido por libedit
	int cursorc;			// Variavel associada a tokenizer
	int cursoro;			// Variavel associada a tokenizer

	// Obtendo variavel de informacao de linha de comando
			
	linf = el_line(el);

	// Inicializando informacoes adicionais para tokenizar linha de entrada

	parsedline = tok_init(NULL);
	lineargs = NULL;
			
	// Parser da linha de comando usando editline
			
	flag = tok_line(parsedline,linf,&argsnumber,&lineargs,&cursorc,&cursoro);

	// flag == 0 indica parser realizado com sucesso

	if(flag){
		
		perror("ERROR AT PARSING\n");
		exit(EXIT_FAILURE);
	}
	
	
	// Atribuindo numero de argumentos lidos na linha de comando
	
	command_line->argc = argsnumber;
	
	if(argsnumber > 0){
		
		//Alocando vetor de comandos
		
		command_line->argv = (char **)malloc((command_line->argc + 1)*sizeof(char *));

		// Atribuindo linha parseada com editline
		
		command_line->argv = lineargs;
	}
	else command_line->argv = NULL;
	
	tok_end(parsedline);

}

