#include "ex20.h"

void Automovel::set(char *pl, char *ma, char *mo, int an) {
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
	Automovel a0;

	a0.set("ABC1212", "Ford", "Focus", 2005);
	a0.print();

	cout << endl;

	Automovel *a1 = new Automovel();

	a1->set("AAC1133", "Toyota", "Corolla", 2004);
	a1->print();

	return 0;
}
