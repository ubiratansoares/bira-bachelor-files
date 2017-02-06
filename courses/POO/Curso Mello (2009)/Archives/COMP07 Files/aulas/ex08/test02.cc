#include <stdlib.h>
#include "Vector.h"

int main(int argc, char *argv[]) {
	Vector<User *> *v01 = new Vector<User *>(); 	// heap
	int i;
	char buffer[500];

	v01->init();

	User *u = NULL;

	for (i = 0; i < 10; i++) {
		u = new User();
		sprintf(buffer, "%d", i); // l64a(i)
		u->setCpf(buffer);
		sprintf(buffer, "nome %d", i);
		u->setName(buffer);

		v01->add(u);
	}

	cout << "Listar users: Antes de Remover" << endl;

	User *tmp = NULL;
	for (i = 0; i < v01->size(); i++) {
		tmp = v01->get(i);
		cout << "Cpf: " << tmp->getCpf() << ", Nome: " << tmp->getName() << endl;
	}

	v01->remove(5);

	cout << "Listar users: Após Remover" << endl;

	for (i = 0; i < v01->size(); i++) {
		tmp = v01->get(i);
		cout << "Cpf: " << tmp->getCpf() << ", Nome: " << tmp->getName() << endl;
	}

	return 0;
}
