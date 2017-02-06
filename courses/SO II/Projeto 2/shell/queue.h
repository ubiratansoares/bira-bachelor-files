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
#ifndef QUEUE_H
#define QUEUE_H

#define MAXC 16

typedef struct job job;
struct job
{
   int pid;
   char *command;
};

typedef struct queue queue;
struct queue
{
   job q[MAXC];
   int begin;
   int end;
   int current;
};

void init_queue(queue *j);
int dequeue(queue *j);
void enqueue(char *command, int pid, queue *j);
job previous_item(queue *j);
job next_item(queue *j);
void free_queue(queue *j);

#endif
