#include <iostream>
using namespace std;
class Funcionario {
	private:
		char *nome;
		float salario;
	public:
		Funcionario(char *nome, float salario) {
			this->nome = new char[strlen(nome)+1];
			strcpy(this->nome, nome);
			this->salario = salario;
		}
		~Funcionario() {
			delete this->nome;
		}
		friend void funcao(Funcionario *f);
		void teste() {
			funcao(this);
		}
};

// pode acessar as variáveis-membro da classe como se fosse um método
// da própria classe
void funcao(Funcionario *f) {
	cout << "Dentro da função friend" << endl;
	f->salario = 100;
	cout << "Novo salário = " << f->salario << endl;
}
