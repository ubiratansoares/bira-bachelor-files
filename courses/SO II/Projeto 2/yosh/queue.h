/*
 *  Arquivo : queue.h
 *  Contém a interface de implementacao para funcao de fila.
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


#ifndef QUEUE_H
#define QUEUE_H


// Definicao do numero maximo de Jobs a serem executados

#define MAXC 16 

// Definicao do tipo de dados job

typedef struct job job;

struct job{
   int pid;
   char *command;
};

// Definicao do tipo de dados fila

typedef struct queue queue;

struct queue{
   job q[MAXC];
   int begin;
   int end;
   int current;
};


// Funcoes para tratamento da fila de jobs

void init_queue(queue *j);
int dequeue(queue *j);
void enqueue(char *command, int pid, queue *j);
job previous_item(queue *j);
job next_item(queue *j);
void free_queue(queue *j);

#endif
