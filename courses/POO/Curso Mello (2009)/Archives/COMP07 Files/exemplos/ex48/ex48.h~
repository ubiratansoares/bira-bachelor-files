#include <iostream>

using namespace std;

class Item {
	private:
		int codigo;
		char *descricao;
	public:
		Item(int codigo, char *descricao) {
			this->codigo = codigo;
			this->descricao = new char[strlen(descricao)+1];
			strcpy(this->descricao, descricao);
		}
		int getCodigo() { return this->codigo; };
		void setCodigo(int codigo) { this->codigo = codigo; };
		char *getDescricao() { return this->descricao; }
		void setDescricao(char *descricao) { 
			this->descricao = new char[strlen(descricao)+1];
			strcpy(this->descricao, descricao);
		}
		virtual char* getAllInformation() = 0;
};

class Livro: public Item {
	char *isbn;

	public:
		Livro(int codigo, char *descricao, char *isbn) :
			Item(codigo, descricao) {
			this->isbn = new char[strlen(isbn)+1];
			strcpy(this->isbn, isbn);
		}
		char *getIsbn() { return this->isbn; };
		void setIsbn(char *isbn) {
			this->isbn = new char[strlen(isbn)+1];
			strcpy(this->isbn, isbn);
		}
		char *getAllInformation() {
			char *buffer = new char[500];
			sprintf(buffer, "%d - %s - %s", getCodigo(), getDescricao(), getIsbn());
			return buffer;
		}
};
