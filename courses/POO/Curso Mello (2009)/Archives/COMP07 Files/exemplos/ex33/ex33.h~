#include <iostream>

using namespace std;

class Funcionario {
	private:
		char *nome;
	protected:
		float salario;
	public:
		Funcionario(char *nome, float salario) {
			this->nome = new char[strlen(nome)+1];
			strcpy(this->nome, nome);
			this->salario = salario;
		}
		void printNome() { cout << "Nome: " << nome << endl; }
};

class Gerente : public Funcionario {
	public:
		Gerente(char *nome, float salario) : Funcionario(nome, salario) {
		}
		void testarAcesso() {
			//cout << "Nome (private na superclasse) = " << nome << endl;
			cout << "Salario (protected na superclasse) = " << salario << endl;
			cout << "Método printNome (public na superclasse): " << endl;
			printNome();
		}
};
