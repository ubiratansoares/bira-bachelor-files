#ifndef _SISTEMA_H_
#define _SISTEMA_H_

#include <iostream>
using namespace std;

class Pai {
	public:
		void m1() { this->m2(); }
		virtual void m2() { cout << "pai" << endl; }
};

class Filho : public Pai {
	public:
		void m2() { cout << "filho" << endl; }
};

#endif
