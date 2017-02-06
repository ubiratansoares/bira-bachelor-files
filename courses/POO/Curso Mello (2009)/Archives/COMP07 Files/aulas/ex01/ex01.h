#include <iostream>
#include <stdio.h>

using namespace std;

class Celular {
	private:
		// atributos, propriedades ou variáveis-membro
		int botoes;
		int connection;
	public:
		// métodos
		void talk() { connection = 1; }
		void cancel() { connection = 0; }
};
