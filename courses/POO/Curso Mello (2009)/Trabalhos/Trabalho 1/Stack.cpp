// Arquivo : stack.cpp
// Esse arquivo contém as implementações para o tipo abstrato de dados pilha, implementado através
// da classe stack.

#include "Stack.h"
#include <malloc.h>

Stack::Stack(){

	elements = NULL;
	count = 0;

}

Stack::~Stack(){

	free(elements);
	elements = NULL;

}


int Stack::empty(){

	return(count == 0);
}


int Stack::size(){

	return(count);

}

void Stack::push(Item *newItem){

	elements = (Item**) realloc(elements, sizeof(Item *) * (count + 1));
	elements[count++] = newItem;

}

Item* Stack::getTop(){

	if(!this->empty()) return (elements[count - 1]);

}

void Stack::pop(){

	if(!this->empty()){

		elements = (Item**) realloc(elements, sizeof(Item *) * (count - 1));
		count--;
	}

}

