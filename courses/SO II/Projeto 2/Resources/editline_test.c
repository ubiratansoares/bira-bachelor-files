/* This will include all our libedit functions.  If you use C++ don't
forget to use the C++ extern "C" to get it to compile.
*/
#include <histedit.h>
#include <stdio.h>

/* To print out the prompt you need to use a function.  This could be
made to do something special, but I opt to just have a static prompt.
*/

char* prompt(EditLine *e);


int main(int argc, char *argv[]) {

	/* This holds all the state for our line editor */
	EditLine *el;

	/* This holds the info for our history */
	History *myhistory;

	/* Temp variables */
	int count;
	const char *line;
	int keepreading = 1;
	HistEvent ev;
	
	
	int flag, k;
	const LineInfo *linf;
	int argsnumber;
	const char **lineargs;
	Tokenizer *parsedline;
	int cursorc;
	int cursoro;
	

	/* Initialize the EditLine state to use our prompt function and macs style editing. */
    
	el = el_init(argv[0], stdin, stdout, stderr);
	el_set(el, EL_PROMPT, &prompt);
	el_set(el, EL_EDITOR, "emacs");

	/* Initialize the history */

	myhistory = history_init();
	
	if (myhistory == 0) {
    	fprintf(stderr, "history could not be initialized\n");
    	return 1;
	}

	/* Set the size of the history */

	history(myhistory, &ev, H_SETSIZE, 800);

	/* This sets up the call back functions for history functionality */

	el_set(el, EL_HIST, history, myhistory);

	// Iniciando estrutura para tokenizar linha de entrada
	
  while (keepreading) {
	
    /* count is the number of characters read.
       line is a const char* of our command line with the tailing \n */
    line = el_gets(el, &count);

    /* In order to use our history we have to explicitly add commands
    to the history */

    if (count > 0) {
	
			parsedline = tok_init(NULL);
			lineargs = NULL;	
	
	
			history(myhistory, &ev, H_ENTER, line);
			//printf("You typed \"%s\"", line);
			
			linf = el_line(el);
			flag = tok_line(parsedline,linf,&argsnumber,&lineargs,&cursorc,&cursoro);
			
			if(flag == 0){
			
				for(k = 0; k < argsnumber; k++)
					printf("word[%d] = %s  ; ",k,lineargs[k]);
				
				
				//printf("\ncursorc = %d",cursorc);
				//printf("\ncursoro = %d",cursoro);
			}
			

    }// end if

	tok_end(parsedline);
	printf("\n");

  }// end while
  
  /* Clean up our memory */
  history_end(myhistory);
  el_end(el);
  

  return 0;
}


char* prompt(EditLine *e) {
  return "mish > ";
}