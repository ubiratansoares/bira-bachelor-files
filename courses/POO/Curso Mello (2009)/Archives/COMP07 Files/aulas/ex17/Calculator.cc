#include <iostream>
#include "globals.h"
#include "Stack.h"
#include "Calculator.h"

using namespace std;

int main(int argc, char *argv[]) {
	Calculator *c = new Calculator(100);

	c->input(100);
	c->input(10);
	c->input(30);
	c->input(50);

	c->op('+');
	c->op('-');
	c->op('+');
	c->op('=');

	delete c;

	return 0;
}
