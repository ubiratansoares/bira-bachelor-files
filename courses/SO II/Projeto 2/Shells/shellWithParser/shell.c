
/* -----------------------------------------------------------------------------
FILE: shell.c
 
NAME:

DESCRIPTION: A SHELL SKELETON
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"   // local file include declarations for parse-related structs

enum BUILTIN_COMMANDS { NO_SUCH_BUILTIN=0, EXIT, JOBS };


/* -----------------------------------------------------------------------------
FUNCTION: buildPrompt()
DESCRIPTION:
-------------------------------------------------------------------------------*/
char * buildPrompt()
{
  return  "{osisgreat} ";
}
 
/* -----------------------------------------------------------------------------
FUNCTION: isBuild()
DESCRIPTION:
-------------------------------------------------------------------------------*/
int isBuiltInCommand( char * cmd )
{
  if( strncmp(cmd, "exit", strlen( "exit" ) ) == 0 )
	{
	return EXIT;
  	}
  return NO_SUCH_BUILTIN;
}


/* -----------------------------------------------------------------------------
FUNCTION: main()
DESCRIPTION:
-------------------------------------------------------------------------------*/
int main( int argc, char **argv )
{
  char * cmdLine;
  parseInfo *info; 		// info stores all the information returned by parser.
  struct commandType *com; 	// com stores command name and Arg list for one command.

  fprintf( stdout, "This is the SHELL version 0.1\n" ) ;


  while(1)
  {

    	// insert your code here

    	cmdLine = readline( buildPrompt() ) ;
    	if( cmdLine == NULL ) 
	{
      		fprintf(stderr, "Unable to read command\n");
      		continue;
    	}

    	// insert your code about history and !x !-x here

    	// calls the parser
    	info = parse( cmdLine );
        if( info == NULL )
	{
      		free(cmdLine);
      		continue;
    	}

    	// prints the info struct
    	print_info( info );

    	//com contains the info. of the command before the first "|"
    	com = &info->CommArray[0];
    	if( (com == NULL)  || (com->command == NULL)) 
    	{
      		free_info(info);
      		free(cmdLine);
      		continue;
    	}

    	//com->command tells the command name of com
    	if( isBuiltInCommand( com->command ) == EXIT )
	{
      		exit(1);
    	}

  //insert your code here / commands etc.



  free_info(info);

  free(cmdLine);

  }/* while(1) */



}
  





