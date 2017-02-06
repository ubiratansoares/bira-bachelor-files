#include <iostream>

using namespace std;

class Empregado {
	char nome[50];
	float salario;
	public:
		float getSalario() {
			return salario;
		}
};

class Vendedor: public Empregado {
	float comissao;
	public:
		void funcao() {
			cout << "Vendedor::funcao()" << endl;
		}
};

class Gerente: public Empregado {
	int secao;
	public:
		void funcao() {
			cout << "Gerente::funcao()" << endl;
		}
};

class VendedorGerente: public Vendedor, public Gerente {
};
