/*
 *  Arquivo : signals.h
 *  Contém a interface de implementacao para handlers de alguns sinais Unix.
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

#ifndef SIGNALS_H
#define SIGNALS_H


void get_signal_SIGINT(int signum);

void get_signal_SIGCONT(int signum);

void get_signal_SIGTSTP(int signum);
 
#endif

