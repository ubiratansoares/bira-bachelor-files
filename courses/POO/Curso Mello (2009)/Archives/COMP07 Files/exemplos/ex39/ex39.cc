#include "ex39.h"

int main(int argc, char *argv[]) {
	Ponto a(1,2);
	Ponto b(2,3);
	Ponto c = a + b; // operator+
	Ponto d(0,0);
	Ponto t(0,0);
       
 	d = c; // operator=

	c.print();
	d.print();

	return 0;
}
