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
#include <stdlib.h>
#include <argz.h>
#include <stdio.h>

#include "parser.h"

void parse_command_line(char *buffer, command_line_t *command_line)
{
   char *argz;
   size_t argz_length;
      
   if( argz_create_sep(buffer,' ',&argz,&argz_length) )
   {
      perror("Failed: can't create argz vector..\n");
      exit (EXIT_FAILURE);
   }
   command_line->argc = argz_count(argz,argz_length);
   if( command_line->argc > 0 )
   {
      command_line->argv = (char **)malloc((command_line->argc + 1)*sizeof(char *));
      argz_extract(argz,argz_length,command_line->argv);
   }
   else
      command_line->argv = NULL;   
}

