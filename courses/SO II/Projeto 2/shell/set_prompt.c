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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#include "set_prompt.h"
#define MAXHOSTNAME 256

/* This function builds the prompt string */
void set_prompt(char **prompt, struct passwd *user_info)
{
    char *host;
    char *dir;
    char *user;

    /*user = getlogin();*/
    user = user_info->pw_name;
    host = (char *)malloc(MAXHOSTNAME*sizeof(char));
    gethostname(host,MAXHOSTNAME);
    dir = get_current_dir_name();

    /* allocate memory to the string */
    *prompt = realloc(*prompt,(strlen(user)+strlen(dir)+strlen(host)+15)*sizeof(char));

    strcpy(*prompt,">[");
    strcat(*prompt,user);
    strcat(*prompt,"@");
    strcat(*prompt,host);
    strcat(*prompt,"]:");
    if( strcmp(dir,user_info->pw_dir) == 0 ) /* is the user's home */
       strcat(*prompt,"~");
    else
       strcat(*prompt,dir);

    if (user_info->pw_uid == 0) /* User is ROOT */
       strcat(*prompt, "# ");
    else
       strcat(*prompt, "$ "); /* Other kind of user */
}
