#include "Livro.h"
#include "User.h"
#include "Biblioteca.h"

// biblioteca
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
}
void Biblioteca::del() { }
void Biblioteca::list() { }

// users
void Biblioteca::create() { }
void Biblioteca::remove() { }
void Biblioteca::show() { }

int main(int argc, char* argv[]) {
	Biblioteca *b = new Biblioteca();
	b->add();
}
