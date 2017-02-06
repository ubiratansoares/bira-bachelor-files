#include "ex38.h"

int main(int argc, char *argv[]) {
	X *x = new X();
	int i;
	float f;
	long l;

	x->solicitacao("\nInforme um inteiro: ", &i);
	x->solicitacao("\nInforme um float: ", &f);
	x->solicitacao("\nInforme um long: ", &l);

	cout << "\n\n" << i << "\n" << f << "\n" << l << endl;
}
