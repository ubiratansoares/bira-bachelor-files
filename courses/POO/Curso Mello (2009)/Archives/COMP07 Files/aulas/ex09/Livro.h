#ifndef _LIVRO_H_
#define _LIVRO_H_

#include <iostream>

using namespace std;

class Livro {
	private:
		char *isbn;
		char *title;
		char *author;
	public:
		void setIsbn  (char *v) { isbn  = new char[strlen(v) + 1]; strcpy(isbn  , v);}
		void setTitle (char *v) { title = new char[strlen(v) + 1]; strcpy(title , v);}
		void setAuthor(char *v) { author= new char[strlen(v) + 1]; strcpy(author, v);}
		char *getIsbn  () { return isbn  ;}
		char *getTitle () { return title ;}
		char *getAuthor() { return author;}
};

#endif
