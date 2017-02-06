#include "ex07.h"

inline void Teste::funcao() {
	cout << "valor = " << var++ << endl;
}

int Teste::var = 0;

int main(int argc, char *argv[]) {
	Teste t;

	t.funcao();
	t.funcao();
	t.funcao();

	Teste x;

	x.funcao();
	x.funcao();
	x.funcao();

	return 0;
}
