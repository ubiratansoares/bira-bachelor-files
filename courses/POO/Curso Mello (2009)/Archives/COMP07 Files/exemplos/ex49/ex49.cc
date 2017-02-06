#include "ex49.h"

Pessoa::Pessoa() {
	this->id = v++;
}

int Pessoa::v= 0;

int main(int argc, char *argv[]) {
	Pessoa p;
	Pessoa *p1 = new Pessoa();

	cout << "p id: " << p.getCodigo() << endl;
	cout << "p1 id: " << p1->getCodigo() << endl;

	return 0;
}
