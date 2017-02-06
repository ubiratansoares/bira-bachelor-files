// Arquivo : stack.h
// Esse arquivo contém as definições para o tipo abstrato de dados pilha, implementado através
// da classe stack. Essa classe tem como base a classe Vector dada em sala de aula.


#ifndef _STACK_H_
#define _STACK_H_

#define Item float

class Stack{

	private:

		Item **elements;	// Array de elementos
		int count;	// Número de elementos armazenados


	public:

		Stack();

		~Stack();

		int empty();

		void push(Item *newItem);

		Item* getTop();

		void pop();

		int size();


};


#endif
