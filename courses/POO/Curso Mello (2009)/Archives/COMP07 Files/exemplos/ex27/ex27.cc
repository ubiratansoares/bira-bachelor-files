#include "ex27.h"

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

int main(int argc, char *argv[]) {

	Caminhao *c = new Caminhao("BCB1212", "Mercedes", "1113", 1980, 13.1);
	c->teste();

	return 0;
}
