// Arquivo : Calculator.cpp
// Esse arquivo contém a implementação da classe Calculator. Para realizar a função de notação polonesa
// reversa, é utilizada uma estrutura de pilha como buffer de operandos e fazendo o tratamento adequado
// a cada tipo de operador.

#include <string>
#include <math.h>
#include <iostream>
#include "Calculator.h"

using namespace std;


Calculator::Calculator(void){

	buffer = new Stack();
}

void Calculator::clear(void){

  while(buffer->size() > 0)
    buffer->pop();

}

void Calculator::setOperand(float *theOperand){

 buffer->push(theOperand);

}

void Calculator::performOperation(string theOperator){

	float *v1,*v2;
	int valid;

	valid = 0; 

	// Chegando a natureza do operador


	if(theOperator.size() == 1){

		// A operação é binária : +,-,*,/

		// Checando os argumentos - devem ao menos dois operandos na pilha

		if(buffer->size() >= 2){

			// Extraindo o primeiro argumento do buffer


			v1 = buffer->getTop();
			buffer->pop();


			// Extraindo o segundo argumento do buffer

			v2 = buffer->getTop();
			buffer->pop();


			// Verificando a operação a ser realizada segundo o argumento do método


			if(theOperator == "+"){ *v1 = *v1 + *v2; valid = 1;}

			if(theOperator == "-"){ *v1 = *v1 - *v2; valid = 1;}

			if(theOperator == "*"){ *v1 = *v1 * *v2; valid = 1;}

			if(theOperator == "/"){ *v1 = *v1 / *v2; valid = 1;}


			// Testando se a operação foi válida e guardando o resultado no buffer

			if(valid) buffer->push(v1);

			else cout << "SINTAX ERROR : INVALID OPERATOR" << endl; 


		}//end checking

		// Resposta caso os argumentos da operação binária sejam insuficientes
		else cout << "SINTAX ERROR : TOO FEW ARGUMENTS FOR :" << theOperator << endl;
	}
  	else{

		// A operação é unária : EXP,LN,SRQT

		// Checando o argumento

		if(!buffer->empty()){ 

		// Extraindo o argumento do buffer

		v1 = buffer->getTop();
		buffer->pop();
  
		// Verificando a operação a ser realizada

		if(theOperator == "exp"){ *v1 = exp(*v1); valid = 1;}

		if(theOperator == "sqrt"){ *v1 = sqrt(*v1); valid = 1;}

		if(theOperator == "ln"){ *v1 = log(*v1); valid = 1;}

		// Testando se a operação foi válida e guardando o resultado no buffer

		if(valid) buffer->push(v1);
		else cout << "SINTAX ERROR : INVALID OPERATOR" << endl; 

	}//end checking

	// Resposta caso os argumentos sejam mais do que dois

	else cout << "SINTAX ERROR : TOO MANY ARGUMENTS FOR : " << theOperator << endl;

  }  // end  if

} // end Calculator::performOperation

float* Calculator::getResult(void){

	if(buffer->size() == 1) return(buffer->getTop());

	else cout << "SINTAX ERROR" << endl;

}

