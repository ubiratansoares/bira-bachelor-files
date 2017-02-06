#include <iostream>

using namespace std;

class Vetor {
	int *valor;
	int tamanho;
	public:
		Vetor(int tamanho) {
			this->tamanho = tamanho;
			this->valor = new int[tamanho];
		}
		Vetor(Vetor &vet) {
			delete this->valor;
			this->valor = new int[vet.tamanho];
			for (int i = 0; i < vet.tamanho; i++)
				this->valor[i] = vet.valor[i];
		}
		void operator=(Vetor& vet) {
			delete this->valor;
			this->valor = new int[vet.tamanho];
			for (int i = 0; i < vet.tamanho; i++)
				this->valor[i] = vet.valor[i];
		}
		void set(int elemento, int valor) {
			this->valor[elemento] = valor;
		}
		void print() {
			cout << "Elementos: " << endl;
			for (int i = 0; i < this->tamanho; i++)
				cout << this->valor[i] << endl;
		}
};
