#include <iostream>

using namespace std;

class Veiculo {
	private:
		char *placa;
		char *marca;
	protected:
		char *modelo;

	public:
		int ano;

		Veiculo(char *pl, char *ma, char *mo, int an);
		void print();
};

class Caminhao: public Veiculo {
	private:
		float capacidade;
	public:
		Caminhao(char *pl, char *ma, char *mo, int an, float ca);

		void teste() {
			//cout << "placa = " << placa << endl; --> não permite acesso
			cout << "modelo = " << modelo << endl;
			cout << "ano = " << ano << endl;
		}
};
