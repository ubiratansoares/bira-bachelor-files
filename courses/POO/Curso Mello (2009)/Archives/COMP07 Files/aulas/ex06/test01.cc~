#include "Vector.h"

int main(int argc, char *argv[]) {
	Vector *v01 = new Vector(); 	// heap
	Vector v02;			// stack
	int i;

	v01->init();
	v02.init();

	Livro *l01 = new Livro();
	l01->setIsbn("123");
	l01->setTitle("titulo");
	l01->setAuthor("autor");

	Livro *l02 = new Livro();
	l02->setIsbn("223");
	l02->setTitle("titulo2");
	l02->setAuthor("autor2");

	v01->add(l01);
	v01->add(l02);

	v02.add(l01);
	v02.add(l02);

	cout << "Vector na Heap" << endl;

	Livro *tmp = NULL;
	for (i = 0; i < v01->size(); i++) {
		tmp = v01->get(i);
		cout << "Isbn: " << tmp->getIsbn() << endl;
		cout << "Title: " << tmp->getTitle() << endl;
		cout << "Author: " << tmp->getAuthor() << endl;
	}

	cout << "Vector na Stack" << endl;

	tmp = NULL;
	for (i = 0; i < v02.size(); i++) {
		tmp = v02.get(i);
		cout << "Isbn: " << tmp->getIsbn() << endl;
		cout << "Title: " << tmp->getTitle() << endl;
		cout << "Author: " << tmp->getAuthor() << endl;
	}

	return 0;
}
