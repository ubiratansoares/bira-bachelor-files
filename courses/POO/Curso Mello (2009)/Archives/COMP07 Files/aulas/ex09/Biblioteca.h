#ifndef _BIBLIOTECA_H_
#define _BIBLIOTECA_H_

#include <iostream>

#include "User.h"
#include "Livro.h"
#include "Vector.h"

using namespace std;

class Biblioteca {
	private:
		Vector<User *>  *users;
		Vector<Livro *> *livros;
	public:
		void init();

		// sobre a biblioteca
		void menu();
		void auth();
		void search();

		// Livros
		void add();
		void del();
		void list();

		// Users
		void create();
		void remove();
		void show();
};

#endif
