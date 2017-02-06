#ifndef _TESTE_H_
#define _TESTE_H_

#include <iostream>

using namespace std;

class Teste {
	private:
		int a;
	public:
		Teste() { this->a = 10; }
		Teste(int a ) { this->a = a; }

		int soma(int a, int b) { return a + b; }
		double soma(int a, double b) { return a + b; }

		int getA() { return this->a; } 

		// &a -> Stack
		// *a -> ponteiro pra inteiro
		void operator=(int &a) {
			this->a = a;
		}

};

#endif
