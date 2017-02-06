#include <iostream>

using namespace std;

class Teste {
	public:
		virtual char funcao() { return 'A'; }
		void teste() { cout << "valor = " << funcao() << endl; }
};

class TesteFilha : public Teste {
	public:
		char funcao() { return 'B'; }
		void testeForcando() { cout << "Forçando função da superclasse = " << Teste::funcao() << endl; }
};
