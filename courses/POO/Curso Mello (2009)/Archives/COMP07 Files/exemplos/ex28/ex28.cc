#include "ex28.h"

Veiculo::Veiculo(char *pl, char *ma, char *mo, int an) {
	placa = new char[strlen(pl)+1];
	marca = new char[strlen(ma)+1];
	modelo = new char[strlen(mo)+1];

	strcpy(placa, pl);
	strcpy(marca, ma);
	strcpy(modelo, mo);
	ano = an;
}

void Veiculo::print() {
	cout << "Placa: " << placa << endl;
	cout << "Marca: " << marca << endl;
	cout << "Modelo: " << modelo << endl;
	cout << "Ano: " << ano << endl;
}

Caminhao::Caminhao(char *pl, char *ma, char *mo, int an, float ca) : 
	Veiculo(pl, ma, mo, an) {
		capacidade = ca;
}

Treminhao::Treminhao(char *pl, char *ma, char *mo, int an, float ca) : 
	Caminhao(pl, ma, mo, an, ca) {
}

int main(int argc, char *argv[]) {

	Caminhao *c = new Caminhao("BCB1212", "Mercedes", "1113", 1980, 13.1);
	c->teste();

	cout << endl;

	Treminhao *t = new Treminhao("AAA1212", "Volvo", "VA1", 2005, 35);
	t->imprimir();

	return 0;
}
