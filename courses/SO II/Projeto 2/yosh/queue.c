/*
 *  Arquivo : queue.c
 *  Contém a implementacao de queue.h.
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
#include <string.h>
#include <unistd.h>

#include "queue.h"

// Funcao : init_queue(j)
// Inicializa a fila de jobs

void init_queue(queue *j){
	
   int i;
   for( i = 0; i < MAXC; i++)
   {
      j->q[i].command = NULL;
      j->q[i].pid = 0;
   }
   j->begin = 0;
   j->end = 0;
   j->current = 0;
}


// Funcao : dequeue(j)
// Remove o elemento da frente da fila, retornando seu valor

int dequeue(queue *j){
	
   int pidret;
   if( j->end != j->begin )
   {
      pidret = j->q[j->begin].pid;
      free(j->q[j->begin].command);
      j->begin = ( j->begin + 1 )%MAXC;
      j->current = j->end;
      return(pidret);
   }
   return(0);   
}


// Funcao: enqueue(command,pid,j)
// Adiciona um job na fila.

void enqueue(char *command, int pid,  queue *j)
{
   int aux = 0;
   aux = (j->end + 1)%MAXC;
   if ( aux == j->begin )
      dequeue(j);
   j->end = aux;
   
   j->q[j->end].command = (char *)malloc(256*sizeof(char));
   strcpy(j->q[j->end].command,command);
   j->q[j->end].pid = pid;
   
   j->current = j->end;
}


job previous_item(queue *j)
{
   job aux;
   if( j->current == j->begin )
   {
      aux.pid = -1;
      aux.command = NULL;
      return(aux);
   }
   else
   {
      if( j->current == 0 )
        j->current = MAXC - 1;
      else  
      j->current = (j->current - 1)%MAXC;
   }
   return (j->q[j->current]);
}




job next_item(queue *j)
{
   if( j->current == j->end )
      return (j->q[j->end]);
   else
      j->current = (j->current + 1)%MAXC;
   return(j->q[j->current]);
}


// Funcao : free_queue(j)
// Desaloca a estrutura de dados fila

void free_queue(queue *j)
{
   while(dequeue(j));
}
