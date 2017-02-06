#include "Pessoa.h"

int main(int argc, char *argv[]) {
	int i;
	char nome[50];
	Pessoa p1; // stack
	Pessoa *p2 = NULL; // heap

	p2 = new Pessoa();

	cout << "Digite o ID: " << endl;
	cin >> i;

	cout << "Digite o Nome: " << endl;
	cin >> nome;

	p1.setId(i);
	p1.setName(nome);

	cout << "Id: " << p1.getId() << " Nome: " << p1.getName() << endl;

	p2->setId(2);
	p2->setName("nome 2");

	cout << "Id: " << p2->getId() << " Nome: " << p2->getName() << endl;

	return 0;
}
