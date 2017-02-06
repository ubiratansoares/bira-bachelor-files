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
#include <unistd.h>
#include <signal.h>

/* When we get a signal, we will call these functions to handle them */
#ifdef WITH_SIGNAL
void get_signal_SIGINT(int signum);
void get_signal_SIGCONT(int signum);
void get_signal_SIGTSTP(int signum);

void get_signal_SIGINT(int signum)
{
   printf("Interrupted [%d].\n",getpid());
   fflush(stdout);
   signal(SIGINT, get_signal_SIGINT);
}

void get_signal_SIGCONT(int signum)
{
   printf("Go! Go! Go!\n");
   fflush(stdout);
   signal(SIGCONT,get_signal_SIGCONT);
}

void get_signal_SIGTSTP(int signum)
{
   printf("Stopped [%d].\n",getpid());
   fflush(stdout);
   signal(SIGTSTP,get_signal_SIGTSTP);
}
#endif

