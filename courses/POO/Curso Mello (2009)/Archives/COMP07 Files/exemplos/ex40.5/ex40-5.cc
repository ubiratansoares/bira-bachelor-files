#include "ex40-5.h"

int main(int argc, char *argv[]) {
	Inteiro a(2);  // inteiro = 0
	Inteiro b(5); // inteiro = 5

	a = b;

	cout << "Valor A = " << a.getValue() << endl;

	Inteiro c = b;

	cout << "Valor C = " << c.getValue() << endl;

	return 0;
}
