#include "ex06.h"

inline void Teste::funcao(int i) {
	cout << "valor = " << i << endl;
}

int main(int argc, char *argv[]) {
	Teste t;

	t.funcao(5);

	return 0;
}
