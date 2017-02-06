#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "Stack.h"
#include <math.h>

class Calculator {
	private:
		Stack<float> *stk;

	public:
		Calculator() {
			stk = new Stack<float>(100);
		}

		~Calculator() {
			delete stk;
		}

		void input(float value) {
			stk->push(value);
		}

		void opb(char o) {
			float v1 = stk->pop();
			float v2 = stk->pop();

			switch (o) {
				case '+':
					stk->push(v1 + v2);
					break;
				case '-':
					stk->push(v2 - v1);
					break;
			}
		}

		void opu(char o) {
			float v1 = stk->pop();
			switch (o) {
				case 'l':
					stk->push(log(v1));
					break;
			}

		}

		float getResult() {
			return stk->getTop();
		}
};

#endif
