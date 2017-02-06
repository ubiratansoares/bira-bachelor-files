#include <stdlib.h>
#include "Vector.h"

int main(int argc, char *argv[]) {
	Vector *v01 = new Vector(); 	// heap
	//Vector *v01 = (Vector *) malloc(sizeof(Vector)); // heap
	//Vector v02;
	//Vector v02();

	int i;
	char isbn  [500];
	char title [500];
	char author[500];

	Livro *l = NULL;

	for (i = 0; i < 10; i++) {
		sprintf(isbn, "%d", i); // l64a(i)
		sprintf(title, "titulo %d", i);
		sprintf(author, "autor %d", i);

		l = new Livro(isbn, title, author);

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
