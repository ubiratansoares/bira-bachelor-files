#include "sistema.h"

int main(int argc, char *argv[]) {
	Filho *f = new Filho();
	Pai *p = NULL;

	p = f;
	cout << "Chamando metodo do pai = " << p->getA() << endl;

	Filho *o_retorno = (Filho *) p;
	o_retorno->print();

	return 0;
}
