#include <iostream>

using namespace std;

class Automovel {
	private:
		char *placa;
		char *marca;
		char *modelo;
		int ano;
	public:
		Automovel(char *pl, char *ma, char *mo, int an);
		void print();
};
