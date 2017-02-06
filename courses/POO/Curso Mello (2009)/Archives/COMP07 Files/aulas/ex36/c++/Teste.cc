#include "Teste.h"

int main(int argc, char *argv[]) {
	Teste *t = new Teste();
	cout << t->getA() << endl;

	int a = 25;
	*t = a;
	cout << t->getA() << endl;

	Teste *t2 = new Teste(15);
	cout << t2->getA() << endl;

	cout << t->soma(1,1) << endl;
	cout << t->soma(1,1.5) << endl;

	return 0;
}
