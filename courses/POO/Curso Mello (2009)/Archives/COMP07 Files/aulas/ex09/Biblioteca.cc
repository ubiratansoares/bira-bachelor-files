#include "Biblioteca.h"

// biblioteca
void Biblioteca::init() { 
	users  = new Vector<User *>();
	livros = new Vector<Livro *>();

	users->init();
	livros->init();
}
void Biblioteca::menu() { }
void Biblioteca::auth() { }
void Biblioteca::search() { }

// livros
void Biblioteca::add() { 
	char isbn[12];
	char title[200];
	char author[100];

	cout << "Digite os dados do Livro" << endl;
	cout << "Isbn: " << endl;
	cin >> isbn;
	cout << "Titulo: " << endl;
	cin >> title;
	cout << "Autor: " << endl;
	cin >> author;

	Livro *l = new Livro();
	l->setIsbn(isbn);
	l->setTitle(title);
	l->setAuthor(author);

	livros->add(l);
}
void Biblioteca::del() { }
void Biblioteca::list() { 
	Livro *tmp = NULL;
	int i;

	for (i = 0; i < livros->size(); i++) {
		tmp = livros->get(i);
		cout << "Isbn: "   << tmp->getIsbn() << endl;
		cout << "Titulo: " << tmp->getTitle() << endl;
		cout << "Autor: "  << tmp->getAuthor() << endl;
	}
}

// users
void Biblioteca::create() { }
void Biblioteca::remove() { }
void Biblioteca::show() { }

int main(int argc, char* argv[]) {
	Biblioteca *b = new Biblioteca();

	b->init();
	b->add();
	b->add();
	b->add();
	b->list();
}
