#include <iostream>

using namespace std;

class Pessoa {
	private:
		static int v;
		int id;
	public:
		Pessoa();
		int getCodigo() { return this->id; }
};
