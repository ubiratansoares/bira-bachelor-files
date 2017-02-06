#include "ex18.h"

int main(int argc, char *argv[]) {
	Ponto a(2,5), b(10,8);
	Ponto *c = new Ponto(11, 2);

	cout << "Distancia = " << a.distancia(b) << endl;

	return 0;
}
