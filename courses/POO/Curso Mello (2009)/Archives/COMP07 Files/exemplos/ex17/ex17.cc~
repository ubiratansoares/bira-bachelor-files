#include "ex17.h"

void Pessoa::set(char *n, char* e) {
	nome = new char[strlen(n)+1];
	endereco = new char[strlen(e)+1];

	strcpy(nome, n);
	strcpy(endereco, e);
}

void Pessoa::print() {
	cout << nome << " " << endereco << endl;
}

int main(int argc, char *argv[]) {
	Pessoa *p = new Pessoa();

	p->set("Joao", "Rua 123");
	p->print();

	Pessoa a;

	a.set("Carlos", "Rua 456");
	a.print();

	return 0;
}
