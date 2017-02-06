#include <iostream>

using namespace std;

class Biblioteca {
	private:
		Vector *vector;
	public:
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
