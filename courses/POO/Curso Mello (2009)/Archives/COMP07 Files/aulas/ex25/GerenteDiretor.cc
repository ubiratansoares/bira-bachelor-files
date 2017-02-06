#include "Gerente.h"
#include "Diretor.h"
#include "GerenteDiretor.h"

int main(int argc, char *argv[]) {
/*
	Gerente *g = new Gerente();
	g->test();

	Diretor *d = new Diretor();
	d->test();
*/
	GerenteDiretor *gd = new GerenteDiretor();
	gd->imprime();

	return 0;
}
