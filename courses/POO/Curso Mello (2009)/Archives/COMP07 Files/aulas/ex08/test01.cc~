#include <stdlib.h>
#include "Vector.h"

int main(int argc, char *argv[]) {
	Vector<Livro *> *v01 = new Vector<Livro *>(); 	// heap
	int i;
	char buffer[500];

	v01->init();

	Livro *l = NULL;

	for (i = 0; i < 10; i++) {
		l = new Livro();
		sprintf(buffer, "%d", i); // l64a(i)
		l->setIsbn(buffer);
		sprintf(buffer, "titulo %d", i);
		l->setTitle(buffer);
		sprintf(buffer, "autor %d", i);
		l->setAuthor(buffer);

		v01->add(l);
	}

	cout << "Listar livros: Antes de Remover" << endl;

	Livro *tmp = NULL;
	for (i = 0; i < v01->size(); i++) {
		tmp = v01->get(i);
		cout << "Isbn: " << tmp->getIsbn() << ", Title: " << tmp->getTitle() << ", Author: " << tmp->getAuthor() << endl;
	}

	v01->remove(5);

	cout << "Listar livros: Após Remover" << endl;

	for (i = 0; i < v01->size(); i++) {
		tmp = v01->get(i);
		cout << "Isbn: " << tmp->getIsbn() << ", Title: " << tmp->getTitle() << ", Author: " << tmp->getAuthor() << endl;
	}

	return 0;
}
