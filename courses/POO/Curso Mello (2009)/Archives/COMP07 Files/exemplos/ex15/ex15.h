#include <iostream>

using namespace std;

class Pessoa { // chaves
	private:
		char *nome;
		char *endereco;
	public:
		void set(char *n, char *e);
		void print();
};
