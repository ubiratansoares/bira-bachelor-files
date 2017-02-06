#include "ex21.h"

Automovel::Automovel(char *pl, char *ma, char *mo, int an) {
	placa = new char[strlen(pl)+1];
	marca = new char[strlen(ma)+1];
	modelo = new char[strlen(mo)+1];

	strcpy(placa, pl);
	strcpy(marca, ma);
	strcpy(modelo, mo);
	ano = an;
}

void Automovel::print() {
	cout << "Placa: " << placa << endl;
	cout << "Marca: " << marca << endl;
	cout << "Modelo: " << modelo << endl;
	cout << "Ano: " << ano << endl;
}

int main(int argc, char *argv[]) {
	Automovel a0("ABC1212", "Ford", "Focus", 2005);
	a0.print();

	cout << endl;

	Automovel *a1 = new Automovel("AAC1133", "Toyota", "Corolla", 2004);
	a1->print();

	return 0;
}
