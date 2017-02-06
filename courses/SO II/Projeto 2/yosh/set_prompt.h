/*
 *  Arquivo : set_prompt.h
 *  Contém a interface de implementacao de funcoes de prompt para o usuario.
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

#ifndef SET_PROMPT_H        
#define SET_PROMPT_H

void set_prompt(char **prompt, struct passwd *user_info);

#endif

