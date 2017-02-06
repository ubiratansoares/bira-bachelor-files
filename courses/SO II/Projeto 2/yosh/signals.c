/*
 *  Arquivo : signals.c
 *  Contém a implementacao de signals.h.
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
#include <unistd.h>
#include <signal.h>


#ifdef WITH_SIGNAL
void get_signal_SIGINT(int signum);
void get_signal_SIGCONT(int signum);
void get_signal_SIGTSTP(int signum);

void get_signal_SIGINT(int signum){
	
   printf("INTERRUPTED PROCESS = [%d].\n",getpid());
   fflush(stdout);
   signal(SIGINT, get_signal_SIGINT);
}

void get_signal_SIGCONT(int signum){
	
   printf("PROCESS GO ON !\n");
   fflush(stdout);
   signal(SIGCONT,get_signal_SIGCONT);
}

void get_signal_SIGTSTP(int signum){
	
   printf("STOPPED PROCESS = [%d].\n",getpid());
   fflush(stdout);
   signal(SIGTSTP,get_signal_SIGTSTP);
}
#endif

