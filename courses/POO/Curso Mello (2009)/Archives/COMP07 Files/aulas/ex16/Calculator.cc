#include <iostream>
#include "Stack.h"
#include "Calculator.h"

using namespace std;

int main(int argc, char *argv[]) {
/*
	Calculator *c = new Calculator();
	c->input(100);
	c->input(10);
	c->input(30);
	c->input(50);

	c->opb('+');
	c->opb('-');
	c->opb('+');

	cout << "valor " << c->getResult() << endl;

	delete c;
*/

	Calculator c;
	c.input(100);
	c.input(10);
	c.input(30);
	c.input(50);

	c.opb('+');
	c.opb('-');
	c.opb('+');

	cout << "valor " << c.getResult() << endl;

	return 0;
}
