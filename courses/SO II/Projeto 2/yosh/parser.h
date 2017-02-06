/*
 *  Arquivo : parser.h
 *  Contém a interface de implementacao de um parser de linha de comando.
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

#ifndef PARSER_H        
#define PARSER_H

typedef struct command_line_t command_line_t; // Estrutura de dados para armazenar a linha de comando apos parser

struct command_line_t
{
  int argc;    // Numero de argumentos lidos na linha de comando
  char **argv; // Linha de comando lida
};


// Funcao : parse_command_line(buffer,command_line_t)
// Essa funcao aceita um buffer de entrada e armazena o resultado do parser em uma estrutura do tipo command_line_t

void parse_command_line (char *, command_line_t *);

#endif
