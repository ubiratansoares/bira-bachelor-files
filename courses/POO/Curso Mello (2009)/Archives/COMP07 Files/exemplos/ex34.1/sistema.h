#ifndef _SISTEMA_H_
#define _SISTEMA_H_

#include <iostream>
#include <string.h>

using namespace std;

class Pessoa {
	private:
		char *cpf;
		char *nome;
	protected:
		void setNome(char *nome) {
			if (this->nome != NULL)
				delete this->nome;
			this->nome = new char[sizeof(char) * (strlen(nome) + 1)];
			strcpy(this->nome, nome);
		}
		void setCpf(char *cpf) {
			if (this->cpf != NULL)
				delete this->cpf;
			this->cpf = new char[sizeof(char) * (strlen(cpf) + 1)];
			strcpy(this->cpf, cpf);
		}
	public:
		Pessoa(char *cpf, char *nome) {
			this->cpf = new char[sizeof(char) * (strlen(cpf) + 1)];
			strcpy(this->cpf, cpf);
			this->nome = new char[sizeof(char) * (strlen(nome) + 1)];
			strcpy(this->nome, nome);
		}
		~Pessoa() {
			delete this->cpf;
			delete this->nome;
		}
		char* getCpf() { return this->cpf; }
		char* getNome() { return this->nome; }
};

class Vendedor : private Pessoa {

	public:
		Vendedor(char *cpf, char *nome) : Pessoa(cpf, nome) {

		}

		Pessoa::setCpf;
		Pessoa::setNome;

		Pessoa::getCpf;
		Pessoa::getNome;
};

#endif
