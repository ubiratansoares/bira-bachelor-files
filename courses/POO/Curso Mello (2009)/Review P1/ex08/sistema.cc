#include <stdio.h>
#include "sistema.h"

int main(int argc, char *argv[]) {

	A *a = NULL;
	B *b = NULL;
	D *d = new D();

	a = d;
	//b = d; naum posso!!!
	
	b = a;

	return 0;
}
