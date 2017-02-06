#ifndef _DIRETOR_H_
#define _DIRETOR_H_

#include "Funcionario.h"
#include <iostream>
using namespace std;

class Diretor: public virtual Funcionario {
	public:
		void test() {
			cout << "Sou o diretor" << endl;
		}
};

#endif
