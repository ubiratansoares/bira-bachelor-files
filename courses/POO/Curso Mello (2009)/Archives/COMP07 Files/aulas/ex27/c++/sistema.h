#ifndef _SISTEMA_H_
#define _SISTEMA_H_

class Item {
	private:
		char *nome;
	protected:
		int a;
	public:
		Item(char *nome) {
			this->setNome(nome);
		}

		~Item() {
			delete this->nome;
		}

		void setNome() {
			this->nome = new char[strlen(nome)+1];
			strcpy(this->nome, nome);
		}

		char *getNome() {
			return this->nome;
		}
};

class Livro : public Item {
	private:
		char *isbn;
	public:
		Livro(char *nome, char *isbn) : Item(nome) {
			//this->setNome(nome);
			this->setIsbn(isbn);
		}

		~Livro() {
			delete this->isbn;
		}

		void setIsbn() {
			this->isbn = new char[strlen(isbn)+1];
			strcpy(this->isbn, isbn);
		}

		char *getIsbn() {
			return this->isbn;
		}

};

#endif
