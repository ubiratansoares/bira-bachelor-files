#ifndef _SISTEMA_H_
#define _SISTEMA_H_
#include <iostream>
using namespace std;
class Pai {
	private:
	      	int a;
	public:
	      	Pai() { this->a = 5; }
	      	Pai(int a) { this->a = a; }
		int getA() { return this->a; }
};
class Filho : public Pai {
	public:
		Filho() : Pai() { }
		Filho(int a) : Pai(a) { }
		void print() { cout << "sou o filho" << endl;}
};
#endif
