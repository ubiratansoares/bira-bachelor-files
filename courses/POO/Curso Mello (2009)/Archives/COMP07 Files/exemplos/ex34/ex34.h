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
		void setSalario(float salario) { this->salario = salario; }
};

class Gerente : private Funcionario {
	public:
		Funcionario::setSalario;
		Gerente(char *nome, float salario) : Funcionario(nome, salario) {
		}
		void set(float salario) { // outra forma
			setSalario(salario);
		}
		void testarAcesso() {
			//cout << "Nome (private na superclasse) = " << nome << endl;
			cout << "Salario (protected na superclasse e transformado em private na subclasse) = " << salario << endl;
			cout << "Método printNome (public na superclasse e transformado em private na subclasse): " << endl;
			printNome();
		}
};
