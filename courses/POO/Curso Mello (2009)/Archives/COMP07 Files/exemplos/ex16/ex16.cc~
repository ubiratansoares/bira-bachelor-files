#include <iostream>
using namespace std;

typedef struct Pessoa {
	char *nome;
	char *endereco;
	void set(char *n, char* e) {
		nome = new char[strlen(n)+1];
		endereco = new char[strlen(e)+1];
		strcpy(nome, n);
		strcpy(endereco, e);
	}
	void print() {
		cout << nome << " " << endereco << endl;
	}
} PESSOA;

int main(int argc, char *argv[]) {
	PESSOA *p = (PESSOA *) malloc(sizeof(PESSOA));

	p->set("Joao", "Rua 123");
	p->print();

	return 0;
}
