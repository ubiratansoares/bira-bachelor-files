#include "ex40.h"

int main(int argc, char *argv[]) {
	Ponto a(1,2); // stack
	Ponto *b = new Ponto(2,3); // heap
	Ponto *c = NULL; // heap
	
	c = a + b;

	c->print();

	return 0;
}
