#ifndef _USER_H_
#define _USER_H_

#include <iostream>

using namespace std;

class User {
	private:
		char *cpf;
		char *nome;
	public:
		User(char *cpf, char *nome) {
			setCpf(cpf);
			setNome(nome);
		}
		void setCpf(char *cpf) {
			this->cpf = new char[strlen(cpf) + 1];
			strcpy(this->cpf, cpf);
		}
		void setNome(char *nome) {
			this->nome = new char[strlen(nome) + 1];
			strcpy(this->nome, nome);
		}
		char* getCpf() { return this->cpf; } 
		char* getNome() { return this->nome; } 

};

#endif
