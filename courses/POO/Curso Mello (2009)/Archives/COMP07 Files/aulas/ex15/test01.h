#include <iostream>

using namespace std;

typedef union {
	int flag:1;
	char op;
	float value; 
} DATA;

class Stack {
	private:
		DATA **stack;
		int count;
		DATA **base;
	public:
		Stack() {
			stack = base = NULL;
			count = 0;
		}

		~Stack() {
			free(stack);
		}

		void push(DATA *d) {
			stack = (DATA **) realloc(stack, sizeof(DATA *) * (count + 1));
			stack[count] = d;
			count++;
		}

		DATA *pop() {
			DATA *ret = stack[count-1];
			count--;
			stack = (DATA **) realloc(stack, sizeof(DATA *) * count);
			return ret;
		}
};

class Calc {
	private:
		Stack *stack;
	public:
		Calc() {
			stack = new Stack();
		}

		~Calc() {
			delete stack;
		}

		void addValue(float value) {
			DATA *d = (DATA *) malloc(sizeof(DATA));
			d->flag = 1; // value
			d->op = value;
			stack->push(d);
		}

		void addOp(char value) {
			DATA *d = (DATA *) malloc(sizeof(DATA));
			d->flag = 0; // value
			d->op = value;
			stack->push(d);
		}

		void process() { // alguem pressionou igual
		}
};
