#include "ex22.h"

Automovel::Automovel(char *pl, char *ma, char *mo, int an) {
	placa = new char[strlen(pl)+1];
	marca = new char[strlen(ma)+1];
	modelo = new char[strlen(mo)+1];

	strcpy(placa, pl);
	strcpy(marca, ma);
	strcpy(modelo, mo);
	ano = an;
}

Automovel::~Automovel() {
	delete placa; // se alocado com malloc utiliza-se free
	delete marca;
	delete modelo;
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
	//delete a0; -> erro, pois não é ponteiro (está na stack e é liberado quando o programa termina)

	cout << endl;

	Automovel *a1 = new Automovel("AAC1133", "Toyota", "Corolla", 2004);
	a1->print();
	delete a1;

	return 0;
}
