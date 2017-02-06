/*
 *  Arquivo : exec.h
 *  Contém a interface de implementacao para as funcoes de execucao de redirecionamentos e pipes.
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

#ifndef EXEC_H        
#define EXEC_H
#include "parser.h"

/* GNU Standard way to report fatal system failure. */

// Abordagem GNU padrao para reportar falhas criticas de sistema
#define fatal() \
   do { fprintf (stderr, "%s: %s: %d: %s: %s\n", \
         command_line.argv[0], __FILE__, __LINE__,\
         __PRETTY_FUNCTION__, strerror(errno)); } while (0)


// Funcoes para execucao de E/S e Pipes

int redirect_io(int redirect, FILE *fpout, FILE *fpin, FILE *fperr, command_line_t command_line); 
void test_redirecting(int *redirect, FILE *fpout, FILE *fpin, FILE *fperr);
void exec_pipe(command_line_t command_line);
void do_pipe(command_line_t com_lines[32], int begin, int num_commands);

#endif
