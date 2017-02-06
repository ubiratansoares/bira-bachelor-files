// Arquivo : Calculator.h
// Esse arquivo contém as definições para o objeto Calculadora, que opera sobre notação polonesa reversa.

#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "Stack.h"
#include <string.h>

using namespace std;


class Calculator{

	private :

		Stack *buffer;

	public :

		Calculator();

		void clear();

		void setOperand(float *theOperand);

		void performOperation(string theOperator);

		float* getResult();

}; //end Class Calculator

#endif
