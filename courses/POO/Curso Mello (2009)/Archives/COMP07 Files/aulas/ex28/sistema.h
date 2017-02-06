#ifndef _SISTEMA_H_
#define _SISTEMA_H_

#include <iostream>

using namespace std;

class A {
	private:
		int a;
	protected:
		int b;
	public:
		int c;
};

class B: public A {
	private:
		int A::b;
		int A::c;
	// PRIVATE
	// a - sem acesso
	// b - private
	// c - private
	public:
		void teste2() {
			//cout << a << endl;
			cout << b << endl;
			cout << c << endl;
		}

	// PROTECTED
	// a - sem acesso
	// b protected -> protected
	// c public -> protected

	// PUBLIC
	// a - não tem acesso
	// b protected -> protected
	// c - public -> public
};

class C: public B {
	public:
		void teste() {
			//cout << a << endl;
			cout << b << endl;
			cout << c << endl;
		}
};

#endif
