/*
 *  This file is part of Yosh.
 *
 *  Yosh is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Yosh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "queue.h"

void init_queue(queue *j)
{
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

int dequeue(queue *j)
{
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

void free_queue(queue *j)
{
   while(dequeue(j));
}
