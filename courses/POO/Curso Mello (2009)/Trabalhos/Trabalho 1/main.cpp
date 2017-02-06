// Arquivo : main.cpp
// Esse arquivo testa o funcionamento da classe Calculator.

#include <iostream>
#include <string>
#include "Calculator.h"

using namespace std;

int main(void){

	Calculator *calc;

	char action;
	string op;
	float value;
	int flag,eval;
	float *result;

	cout << "\nUSAGE\n";
	cout << "i <value>    - bufferize a new value (float only) "<< endl;
	cout << "o <operand>  - bufferize a new operation " << endl;
	cout << "r            - to eval the current stored expression" << endl << endl;

	flag = 0;
	eval = 0;

	calc = new Calculator();

	do{

		cin >> action;

		switch(action){

			case 'i' : cin >> value; 
				   calc->setOperand(&value);
			break;

			case 'o' : cin >> op;
				   calc->performOperation(op);
			break; 

			case 'r' : eval = 1; 
			break;

			default : flag = 1;
			break;

		}


	cout << "\nTOP = " << *calc->getResult() << endl << endl;

	}while(!eval && !flag); 

	if(flag) cout << "ERROR : INVALID ACTION." << endl;

	else if(eval) {
				result = calc->getResult();
				cout << "RESULT = " << *result << endl;
			}

	calc->clear();

	return 0;
}
