#include "ex45.h"

int main(int argc, char *argv[]) {
	Teste *t = new Teste();
	t->m1();

	cout << endl;

	TesteFilho *tf = new TesteFilho();
	tf->m1();

	return 0;
}
