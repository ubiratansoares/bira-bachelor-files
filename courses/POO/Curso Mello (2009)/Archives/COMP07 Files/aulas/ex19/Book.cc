#include "Book.h"

int main(int argc, char *argv[]) {

	Book *b1 = new Book("titulo 01", "author 01");

	cout << " Code:   " << b1->getCode()    << 
		" Title:  " << b1->getTitle()   <<
		" Author: " << b1->getAuthor() << endl;

	Book *b2 = new Book("titulo 02", "author 02");

	cout << " Code:   " << b2->getCode()    << 
		" Title:  " << b2->getTitle()   <<
		" Author: " << b2->getAuthor() << endl;

	return 0;
}
