#ifndef _teste_h_
#define _teste_h_

#include <iostream>
using namespace std;

class Teste {
	public:
		void metodo2() { cout << "tahtahtah" << endl; }
		virtual void metodo1() = 0; // naum posso instanciar essa classe!
};

class Teste2 : public Teste {
	public:
		void metodo1() {
			cout << "corpo do metodo1()" << endl;
		}
};

#endif
