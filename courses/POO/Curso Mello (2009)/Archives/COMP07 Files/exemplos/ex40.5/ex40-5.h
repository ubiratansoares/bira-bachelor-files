#ifndef _EX405_H_
#define _EX405_H_

#include <iostream>

using namespace std;

class Inteiro {
	int i;

	public:
		Inteiro() { this->i = 0; }
		Inteiro(int i) { this->i = i; }

		int getValue() { return this->i; }
		void operator=(Inteiro &i) {
			this->i = i.getValue();
		}

};

#endif
