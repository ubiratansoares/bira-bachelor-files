#include "ex15.h"

void Pessoa::set(char *n, char* e) {
	nome = new char[strlen(n)+1];
	endereco = new char[strlen(e)+1];

	strcpy(nome, n);
	strcpy(endereco, e);

	// o que acontece se fizer nome = n??? e endereco = e???
}

void Pessoa::print() {
	//printf("%s %s\n", nome, endereco);
	cout << nome << " " << endereco << endl;
}

int main(int argc, char *argv[]) {

	// Objeto p do tipo Pessoa (classe)
	// na memória Heap
	Pessoa *p = new Pessoa();

	// chamando métodos
	p->set("Joao", "Rua 123");
	p->print();

	// Stack
	Pessoa p1;
	
	p1.set("Paulo", "Rua ABC");
	p1.print();

	return 0;
}
