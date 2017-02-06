/*
 *  Arquivo : set_prompt.c
 *  Contém a implementacao de set_prompt.h.
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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#include "set_prompt.h"
#define MAXHOSTNAME 256

// Funcao : set_prompt
// Essa funcao constroi a string do prompt de comando em relacao ao usuario que chama o SHELL

void set_prompt(char **prompt, struct passwd *user_info){
	
    char *host;
    char *dir;
    char *user;

    /*user = getlogin();*/
    
	user = user_info->pw_name;
    host = (char *)malloc(MAXHOSTNAME*sizeof(char));
    gethostname(host,MAXHOSTNAME);
    dir = get_current_dir_name();

    // Alocando memoria para a string de prompt

    *prompt = realloc(*prompt,(strlen(user)+strlen(dir)+strlen(host)+15)*sizeof(char));

    strcpy(*prompt,">[");
    strcat(*prompt,user);
    strcat(*prompt,"@");
    strcat(*prompt,host);
    strcat(*prompt,"]:");


	// Tratamento de acordo com o usuario

    if( strcmp(dir,user_info->pw_dir) == 0 ) // Checa se o diretorio corrente eh /home
       strcat(*prompt,"~");
    else
       strcat(*prompt,dir);

    if (user_info->pw_uid == 0) // Checa se o usuario é ROOT
       strcat(*prompt, "# ");
    else
       strcat(*prompt, "$ ");
}
