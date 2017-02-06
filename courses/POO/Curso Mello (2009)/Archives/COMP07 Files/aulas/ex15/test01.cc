#include "test01.h"

int main(int argc, char *argv[]) {

	Stack *stack = new Stack();

	DATA *d1 = (DATA *) malloc(sizeof(DATA));
	d1->flag = 1; // value
	d1->op = 10;

	stack->push(d1);

	DATA *d2 = (DATA *) malloc(sizeof(DATA));
	d2->flag = 1; // value
	d2->op = 20;

	stack->push(d2);

	DATA *d3 = (DATA *) malloc(sizeof(DATA));
	d3->flag = 0; // value
	d3->op = '+';

	stack->push(d3);

	return 0;
}
