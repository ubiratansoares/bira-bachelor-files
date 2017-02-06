#ifndef _BOOK_H_
#define _BOOK_H_

#include <iostream>

using namespace std;

class Book {

	private:
		static int scode;
		int code;
		char *title;
		char *author;

	public:
		Book(char *title, char *author) {
			this->code = this->scode++;
			setTitle(title);
			setAuthor(author);
		}

		void setTitle(char *title) {
			this->title = new char[strlen(title) + 1];
			strcpy(this->title, title);
		}

		void setAuthor(char *author) {
			this->author = new char[strlen(author) + 1];
			strcpy(this->author, author);
		}

		int getCode() { return this->code; }
		char *getTitle() { return this->title; }
		char *getAuthor() { return this->author; }
};

int Book::scode = 0;

#endif
