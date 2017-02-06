#include <iostream>

using namespace std;

int x = 2;

class X {
	private:
		int x;

	public:
		X(int x) { this->x = x; }
		int f1() { return x; } // this->x
		int f2() { return ::x; } // x global
		int f3() { int x = 3; return x; } // retorna x local
		int f4() { int x = 3; return X::x; } // this->x
		int f5() { int x = 3; return ::x; } // x global
};
