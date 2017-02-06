#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	time_t timer;
	char *timestr;
	char *aux;
	char *timelog;
	
	timelog = NULL;
	timestr = NULL;
	
	timelog = (char*) malloc(30 * sizeof(char));
	timelog[0] = '<';
	timelog[1] = ' ';
	timelog[27] = '>';
	timelog[29] = '\0';
	
	timer = time(NULL);
	timestr = asctime(localtime(&timer));

	printf("%s",timestr);

	aux = &timelog[2];
	
	strncpy(aux,timestr,25);

	timelog[26]  = ' ';
	printf("%s",timelog);

  return 0;
}