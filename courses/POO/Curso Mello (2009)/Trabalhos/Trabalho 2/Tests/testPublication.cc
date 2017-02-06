#include <iostream>
#include "item.h"
#include "publication.h"

using namespace std;

int main(void){


	int code;
	string title;

	Publication *pub = new Publication();

	pub->setCode(02);
	pub->setTitle("Publication 02");

	pub->setYear(02);
	pub->setFormat("Format 02");
	pub->setAuthor("Author 02");
	pub->setContent("Contents 02");

	cout << pub->getCode() << endl 
		<< pub->getTitle() << endl
		<< pub->getYear() << endl
		<< pub->getFormat() << endl
		<< pub->getAuthor() << endl
		<< pub->getContent() << endl;

	return 0;
}
